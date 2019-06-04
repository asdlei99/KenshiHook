#include "Renderer.h"


/** 
Load the shaders at compile time into a constant by converting
the shader code into a string. By doing this we don't have to
place the shaders next to our .dll (or embed in some other way).

Found how to do this here: 
https://stackoverflow.com/questions/20443560/how-to-practically-ship-glsl-shaders-with-your-c-software
**/
const char* vertexShaderData = {
#include "VertexShader.hlsl"
};

const char* pixelShaderData = { 
#include "PixelShader.hlsl"
};



Renderer::Renderer(DebugConsole* console)
{
	this->console = console;
}

HRESULT Renderer::GetDeviceAndContextFromSwapChain(IDXGISwapChain *swapChain, ID3D11Device **device, ID3D11DeviceContext **context)
{
	HRESULT get = swapChain->GetDevice(__uuidof(ID3D11Device), (void**)device);

	if (SUCCEEDED(get)) (*device)->GetImmediateContext(context);

	return get;
}

bool Renderer::Render(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags)
{

	if (!initialized)
	{
		console->PrintDebugMsg("Initializing renderer...", nullptr, console->MsgType::STARTPROCESS);

		if (FAILED(GetDeviceAndContextFromSwapChain(swapChain, &device, &context)))
		{
			console->PrintDebugMsg("Failed to initialize renderer", nullptr, console->MsgType::FAILED);
			return false;
		}

		DXGI_SWAP_CHAIN_DESC desc;
		ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
		swapChain->GetDesc(&desc);

		HWND* window = &desc.OutputWindow;
		RECT hwndRect;
		GetWindowRect(*window, &hwndRect);
		windowWidth = hwndRect.right;
		windowHeight = hwndRect.bottom;

		console->PrintDebugMsg("Window width: %i", (void*)windowWidth, console->MsgType::PROGRESS);
		console->PrintDebugMsg("Window height: %i", (void*)windowHeight, console->MsgType::PROGRESS);

		ID3D11Texture2D* backbuffer;

		swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backbuffer);
		device->CreateRenderTargetView(backbuffer, nullptr, &mainRenderTargetView);
		console->PrintDebugMsg("Backbuffer address: %p", &backbuffer, console->MsgType::PROGRESS);
		backbuffer->Release();

		context->OMSetRenderTargets(1, &mainRenderTargetView, NULL);

		CreateRectangleModel();
		CreatePipeline();

		initialized = true;
		console->PrintDebugMsg("Successfully initialized the renderer, ready to draw", nullptr, console->MsgType::COMPLETE);
	}
	
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vertexBufferObject, &stride, &offset);
	context->IASetInputLayout(inputLayout);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->VSSetShader(vertexShader, nullptr, 0);
	context->PSSetShader(pixelShader, nullptr, 0);

	context->Draw(3, 0);

	return true;
}

void Renderer::CreateRectangleModel()
{

	Vertex vertices[] = 
	{
		{ DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.45f, -0.5f, 0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.45f, -0.5f, 0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }
	};

	D3D11_BUFFER_DESC vbdesc = { 0 };
	ZeroMemory(&vbdesc, sizeof(D3D11_BUFFER_DESC));
	vbdesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertices);
	vbdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbdesc.Usage = D3D11_USAGE_DEFAULT;
	vbdesc.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA subdata = { vertices, 0, 0 };

	device->CreateBuffer(&vbdesc, &subdata, &vertexBufferObject);
}

void Renderer::CreatePipeline()
{
	ID3DBlob* vertexShaderBlob = LoadShader(vertexShaderData, "vs_5_0");
	ID3DBlob* pixelShaderBlob = LoadShader(pixelShaderData, "ps_5_0");

	device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), 
		vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader);

	device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), 
		pixelShaderBlob->GetBufferSize(), nullptr, &pixelShader);

	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	
	device->CreateInputLayout(inputLayoutDesc, ARRAYSIZE(inputLayoutDesc), vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize(), &inputLayout);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.Width = 800;
	viewport.Height = 600;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	context->RSSetViewports(1, &viewport);
}

ID3DBlob* Renderer::LoadShader(const char* shaderData, std::string targetShaderVersion)
{
	console->PrintDebugMsg("Loading shader...", nullptr, console->MsgType::PROGRESS);
	ID3DBlob* errorBlob = nullptr;
	ID3DBlob* shaderBlob;

	D3DCompile(shaderData, strlen(shaderData), 0, nullptr, nullptr, "main", targetShaderVersion.c_str(), D3DCOMPILE_ENABLE_STRICTNESS, 0, &shaderBlob, &errorBlob);

	if (errorBlob)
	{
		char error[256]{ 0 };
		memcpy(error, errorBlob->GetBufferPointer(), errorBlob->GetBufferSize());
		console->PrintDebugMsg("Shader error: %s", (void*)error, console->MsgType::FAILED);
		return nullptr;
	}

	console->PrintDebugMsg("Shader loaded", nullptr, console->MsgType::PROGRESS);
	return shaderBlob;
}
