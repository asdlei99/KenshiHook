#pragma once

#include <Windows.h>
#include <d3d11.h>
#include "DebugConsole.h"
#include <fstream>
#include <d3dcompiler.h>
#include <DirectXMath.h>

class Renderer
{
private:
	ID3D11Buffer* vertexBufferObject;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* inputLayout;
	ID3D11DeviceContext *context = NULL;
	ID3D11Device *device = NULL;
	ID3D11RenderTargetView *mainRenderTargetView;

	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
	};

	DebugConsole* console;
	bool initialized = false;
	int windowWidth, windowHeight;

	HRESULT GetDeviceAndContextFromSwapChain(IDXGISwapChain *swapChain, ID3D11Device **device, ID3D11DeviceContext **context);
	void CreateRectangleModel();
	void CreatePipeline();
	ID3DBlob* LoadShader(const char* shaderData, std::string targetShaderVersion);

public:
	Renderer() {};
	Renderer(DebugConsole* console);
	bool Render(IDXGISwapChain *swapChain, UINT syncInterval, UINT flags);
};