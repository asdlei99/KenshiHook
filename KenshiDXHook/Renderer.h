#pragma once

#include <Windows.h>
#include <d3d11.h>
#include "DebugConsole.h"
#include <fstream>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "Vertex.h"
#include "Mesh.h"
#include "TexturedBox.h"
#include "Textures.h"

class Renderer
{
private:
	ID3D11Buffer* vertexBuffer;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* inputLayout;
	ID3D11DeviceContext* context = NULL;
	ID3D11Device* device = NULL;
	ID3D11RenderTargetView* mainRenderTargetView;
	D3D11_VIEWPORT viewport;
	ID3D11SamplerState* samplerState;

	DebugConsole* console;
	Textures* textures;
	bool initialized = false;
	bool firstRender = true;
	int windowWidth, windowHeight;

	void CreateExampleTriangle();
	void CreatePipeline();
	ID3DBlob* LoadShader(const char* shaderData, std::string targetShaderVersion, std::string shaderEntry);

public:
	Renderer() {};
	Renderer(DebugConsole* console);
	bool Init(IDXGISwapChain *swapChain, UINT syncInterval, UINT flags);
	bool Render(IDXGISwapChain *swapChain, UINT syncInterval, UINT flags, std::vector<Mesh> thingsToDraw);
	HRESULT CreateBufferForMesh(D3D11_BUFFER_DESC desc, D3D11_SUBRESOURCE_DATA data, ID3D11Buffer** buffer);
	bool IsInitialized();
	bool IsFirstRender();
	void SetFirstRender(bool isFirstRender);
	ID3D11Device* GetDevice();
	void SetTextureManager(Textures* textures);
	void Cleanup();
};