#include "Renderer.h"

/** 
Load the shaders at compile time into a constant by converting
the shader code into a string. By doing this we don't have to
place the shaders next to our .dll (or embed in some other way).

Found how to do this here: 
https://stackoverflow.com/questions/20443560/how-to-practically-ship-glsl-shaders-with-your-c-software
**/
const char* shaderData = {
#include "Shaders.hlsl"
};



Renderer::Renderer(DebugConsole* console)
{
	this->console = console;
}

bool Renderer::Init(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags)
{
	console->PrintDebugMsg("Initializing renderer...", nullptr, console->MsgType::STARTPROCESS);

	HRESULT getDevice = swapChain->GetDevice(__uuidof(ID3D11Device), (void**)&device);

	if (SUCCEEDED(getDevice))
	{
		device->GetImmediateContext(&context);
	}
	else
	{
		console->PrintDebugMsg("Failed to initialize renderer", nullptr, console->MsgType::FAILED);
		return false;
	}

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChain->GetDesc(&desc);

	HWND window = GetForegroundWindow();
	RECT hwndRect;
	GetWindowRect(window, &hwndRect);
	windowWidth = hwndRect.right - hwndRect.left;
	windowHeight = hwndRect.bottom - hwndRect.top;

	console->PrintDebugMsg("Window width: %i", (void*)windowWidth, console->MsgType::PROGRESS);
	console->PrintDebugMsg("Window height: %i", (void*)windowHeight, console->MsgType::PROGRESS);

	ID3D11Texture2D* backbuffer;

	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backbuffer);
	device->CreateRenderTargetView(backbuffer, nullptr, &mainRenderTargetView);
	console->PrintDebugMsg("Backbuffer address: %p", &backbuffer, console->MsgType::PROGRESS);
	backbuffer->Release();

	CreatePipeline();
	CreateExampleTriangle();

	console->PrintDebugMsg("Successfully initialized the renderer, ready to draw", nullptr, console->MsgType::COMPLETE);
	initialized = true;
	return true;
}

bool Renderer::Render(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags, std::vector<Mesh> thingsToDraw)
{
	context->OMSetRenderTargets(1, &mainRenderTargetView, nullptr);
	context->RSSetViewports(1, &viewport);

	context->IASetInputLayout(inputLayout);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->VSSetShader(vertexShader, nullptr, 0);
	context->PSSetShader(pixelShader, nullptr, 0);

	context->PSSetSamplers(0, 1, &samplerState);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	for (int i = 0; i < thingsToDraw.size(); i++)
	{
		Mesh mesh = thingsToDraw.at(i);
		ID3D11ShaderResourceView* texture = textures->GetTexture(mesh.GetTextureIndex());
		context->PSSetShaderResources(0, 1, &texture);
		ID3D11Buffer** meshVB = mesh.GetVertexBuffer();
		ID3D11Buffer* meshIB = *mesh.GetIndexBuffer();
		context->IASetVertexBuffers(0, 1, meshVB, &stride, &offset);
		context->IASetIndexBuffer(meshIB, DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(mesh.GetNumIndices(), 0, 0);
	}

	//context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	//context->Draw(3, 0);
	return true;
}

void Renderer::CreatePipeline()
{
	ID3DBlob* vertexShaderBlob = LoadShader(shaderData, "vs_5_0", "VS");
	ID3DBlob* pixelShaderBlob = LoadShader(shaderData, "ps_5_0", "PS");

	device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader);

	device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(),
		pixelShaderBlob->GetBufferSize(), nullptr, &pixelShader);

	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[3] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	device->CreateInputLayout(inputLayoutDesc, ARRAYSIZE(inputLayoutDesc), vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize(), &inputLayout);

	vertexShaderBlob->Release();
	pixelShaderBlob->Release();

	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.Width = windowWidth;
	viewport.Height = windowHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &samplerState);
}

ID3DBlob* Renderer::LoadShader(const char* shader, std::string targetShaderVersion, std::string shaderEntry)
{
	console->PrintDebugMsg("Loading shader: %s", (void*)shaderEntry.c_str(), console->MsgType::PROGRESS);
	ID3DBlob* errorBlob = nullptr;
	ID3DBlob* shaderBlob;

	D3DCompile(shader, strlen(shader), 0, nullptr, nullptr, shaderEntry.c_str(), targetShaderVersion.c_str(), D3DCOMPILE_ENABLE_STRICTNESS, 0, &shaderBlob, &errorBlob);

	if (errorBlob)
	{
		char error[256]{ 0 };
		memcpy(error, errorBlob->GetBufferPointer(), errorBlob->GetBufferSize());
		console->PrintDebugMsg("Shader error: %s", (void*)error, console->MsgType::FAILED);
		return nullptr;
	}

	if(errorBlob != nullptr) errorBlob->Release();

	console->PrintDebugMsg("Shader loaded", nullptr, console->MsgType::PROGRESS);
	return shaderBlob;
}

HRESULT Renderer::CreateBufferForMesh(D3D11_BUFFER_DESC desc, D3D11_SUBRESOURCE_DATA data, ID3D11Buffer** buffer)
{
	HRESULT hr = device->CreateBuffer(&desc, &data, buffer);
	return hr;
}

void Renderer::CreateExampleTriangle()
{
	Vertex vertices[] =
	{
		{ XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.5f, 0.0f) },
		{ XMFLOAT3(0.45f, -0.5f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.5f) },
		{ XMFLOAT3(-0.45f, -0.5f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 0.5f) }
	};

	D3D11_BUFFER_DESC vbDesc = { 0 };
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));
	vbDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA subData = { vertices, 0, 0 };

	device->CreateBuffer(&vbDesc, &subData, &vertexBuffer);
}

bool Renderer::IsInitialized()
{
	return initialized;
}

bool Renderer::IsFirstRender()
{
	return firstRender;
}

void Renderer::SetFirstRender(bool isFirstRender)
{
	firstRender = isFirstRender;
}

ID3D11Device * Renderer::GetDevice()
{
	return device;
}

void Renderer::SetTextureManager(Textures* textures)
{
	this->textures = textures;
}

void Renderer::Cleanup()
{
	if (vertexBuffer != nullptr) vertexBuffer->Release();
	if (vertexShader != nullptr) vertexShader->Release();
	if (pixelShader != nullptr) pixelShader->Release();
	if (inputLayout != nullptr) inputLayout->Release();
	if (context != nullptr) context->Release();
	if (device != nullptr) device->Release();
	if (mainRenderTargetView != nullptr) mainRenderTargetView->Release();
}
