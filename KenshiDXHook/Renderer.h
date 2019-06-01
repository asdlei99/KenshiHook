#pragma once

#include <Windows.h>
#include <d3d11.h>
#include "DebugConsole.h"

class Renderer
{
private:
	ID3D11DeviceContext *context = NULL;
	ID3D11Device *device = NULL;
	ID3D11RenderTargetView *mainRenderTargetView;
	bool initialized = false;

	HRESULT GetDeviceAndContextFromSwapChain(IDXGISwapChain *swapChain, ID3D11Device **device, ID3D11DeviceContext **context);
public:
	bool Render(IDXGISwapChain *swapChain, UINT syncInterval, UINT flags, DebugConsole* consoleRef);
};