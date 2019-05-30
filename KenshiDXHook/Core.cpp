#include "Core.h"

// Variables for use with assembly code
extern "C" int FixAndJmpBack();
extern "C" int JmpToAbs64AddrPushPop();
extern "C" QWORD absAddr = NULL;
extern "C" QWORD presentAddr = NULL;
extern "C" QWORD jmpBackAddr = NULL;

// Pointers to functions
PresentFunction originalPresentFunction;
PresentFunction newPresentFunction;

BOOL g_bInitialised = false;
ID3D11DeviceContext *pContext = NULL;
ID3D11Device *pDevice = NULL;
ID3D11RenderTargetView *mainRenderTargetView;

bool first = true;

HRESULT GetDeviceAndCtxFromSwapchain(IDXGISwapChain *pSwapChain, ID3D11Device **ppDevice, ID3D11DeviceContext **ppContext)
{
	HRESULT ret = pSwapChain->GetDevice(__uuidof(ID3D11Device), (PVOID*)ppDevice);

	if (SUCCEEDED(ret))
		(*ppDevice)->GetImmediateContext(ppContext);

	return ret;
}

__int64 __fastcall Present(IDXGISwapChain *pSwapChain, UINT SyncInterval, UINT Flags)
{

	// This crashes the code, if this function is empty, it does not crash
	if (first)
	{
		std::ofstream file;
		file.open("HELLO FROM PRESENT.txt");
		file << "HELLO";
		file.close();
		first = false;
	}

	// Below is some DirectX code I want to test later

	//first = false;
	//if (!g_bInitialised)
	//{

		//if (FAILED(GetDeviceAndCtxFromSwapchain(pSwapChain, &pDevice, &pContext)))
			//return 0;

		//DXGI_SWAP_CHAIN_DESC sd;
		//pSwapChain->GetDesc(&sd);

		//ImGui_ImplWin32_Init(sd.OutputWindow);
		//ImGui_ImplDX11_Init(pDevice, pContext);

		//ID3D11Texture2D* pBackBuffer;

		//pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		//pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
		//pBackBuffer->Release();

		//g_bInitialised = true;
	//}

	//ImGui_ImplWin32_NewFrame();
	//ImGui_ImplDX11_NewFrame();

	//ImGui::NewFrame();
	//bool bShow = true;
	//ImGui::ShowDemoWindow(&bShow);
	//ImGui::EndFrame();

	//ImGui::Render();

	//pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);

	return 0;
}

void Core::Init()
{
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	SetWindowText(GetConsoleWindow(), "KenshiDXHook");

	PrintDebugMsg("test");

	originalDllBaseAddress = (QWORD)GetModuleHandleA("dxgi_.dll");
	originalPresentFunctionOffset = 0x5070;
	originalPresentFunction = (PresentFunction)(originalDllBaseAddress + (QWORD)originalPresentFunctionOffset);

	PrintDebugMsg((void*)originalDllBaseAddress);
	PrintDebugMsg((void*)originalPresentFunctionOffset);
	PrintDebugMsg(originalPresentFunction);

	PrintDebugMsg("----------------");

	Hook(originalPresentFunction, (QWORD)FixAndJmpBack, 14);
}

bool Core::Hook(PresentFunction originalFunction, QWORD newFunction, int length)
{
	PrintDebugMsg(originalFunction);
	PrintDebugMsg(newFunction);
	DWORD oldProtection;

	VirtualProtect(originalFunction, length, PAGE_EXECUTE_READWRITE, &oldProtection);

	memset(originalFunction, 0x90, length);

	// Bytes are flipped (because of endianness), could alternatively use _byteswap_uint64()
	*(QWORD*)originalFunction = 0x0000000025FF;

	// The kind of jump I'm doing here seems to only use 6 bytes,
	// and then grabs the subsequent memory address,
	// I'm not quite sure if I'm doing this right
	*(QWORD*)((QWORD)originalFunction + 6) = newFunction;

	DWORD temp;
	VirtualProtect(originalFunction, length, oldProtection, &temp);

	originalPresentFunction = (PresentFunction)((QWORD)originalFunction + length);

	presentAddr = (QWORD)Present;
	jmpBackAddr = (QWORD)originalPresentFunction;

	return true;
}

Core::~Core()
{
	FreeConsole();
}
