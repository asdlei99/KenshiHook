#include "Renderer.h"

HRESULT Renderer::GetDeviceAndContextFromSwapChain(IDXGISwapChain *swapChain, ID3D11Device **device, ID3D11DeviceContext **context)
{
	HRESULT get = swapChain->GetDevice(__uuidof(ID3D11Device), (void**)device);

	if (SUCCEEDED(get)) (*device)->GetImmediateContext(context);

	return get;
}

bool Renderer::Render(IDXGISwapChain *swapChain, UINT syncInterval, UINT flags, DebugConsole* consoleRef)
{

	if (!initialized)
	{
		consoleRef->PrintDebugMsg("Initializing renderer...", nullptr, consoleRef->STARTPROCESS);

		if (FAILED(GetDeviceAndContextFromSwapChain(swapChain, &device, &context)))
		{
			consoleRef->PrintDebugMsg("Failed to initialize renderer", nullptr, consoleRef->FAILED);
			return false;
		}

		DXGI_SWAP_CHAIN_DESC desc;
		swapChain->GetDesc(&desc);

		ID3D11Texture2D* backBuffer;

		swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
		device->CreateRenderTargetView(backBuffer, NULL, &mainRenderTargetView);
		backBuffer->Release();

		initialized = true;
		consoleRef->PrintDebugMsg("Successfully initialized the renderer, drawing...", nullptr, consoleRef->PROGRESS);
	}

	context->OMSetRenderTargets(1, &mainRenderTargetView, NULL);

	return true;
}
