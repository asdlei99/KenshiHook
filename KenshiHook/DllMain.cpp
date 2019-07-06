#include <Windows.h>
#include <iostream>
#include "Core.h"

// Set injectable to 1 to make this .dll injectable instead of a proxy
#define injectable 0

// Check if 64 bit
#ifdef _WIN64
#define is64Bit 1
#endif

// Dynamically change which assembly files we use
#ifdef is64Bit
#define JmpToAddr JmpToAddr64() // 64-bit
#else
#define JmpToAddr JmpToAddr() // 32-bit
#endif

// Assembly function defined in JmpToAddr64.asm or JmpToAddr.asm 
extern "C" int JmpToAddr;

HMODULE thisModule = 0; // Handle for this .dll
HMODULE originalDll = 0; // Handle for original .dll
FARPROC procAddresses[21]; // Original .dll function addresses

Core core = Core();

DWORD WINAPI MainThread(LPVOID lpParam)
{
	core.Init(originalDll);

	return S_OK;
}

BOOL WINAPI DllMain(HMODULE module, DWORD reason, LPVOID)
{

	if (reason == DLL_PROCESS_ATTACH)
	{

		// A way to pause the execution of the .dll when it first loads
		bool paused = true;
		while (paused)
		{
			if (GetAsyncKeyState(VK_F4)) // F4 button
			{
				paused = false;
			}
		}

		thisModule = module;

#if injectable == 1
		originalDll = GetModuleHandleA("dxgi.dll");
		CreateThread(0, 0x1000, &MainThread, 0, 0, NULL);
		return 1;
#endif

		// Load the correct version of dxgi.dll
#ifdef is64Bit
		originalDll = LoadLibrary("C:\\Windows\\System32\\dxgi.dll");
#else
		originalDll = LoadLibrary("C:\\Windows\\SysWOW64\\dxgi.dll");
#endif

		if(!originalDll) return false;

		// Find function addresses we need for forward exporting
		procAddresses[0] = GetProcAddress(originalDll, "ApplyCompatResolutionQuirking");
		procAddresses[1] = GetProcAddress(originalDll, "CompatString");
		procAddresses[2] = GetProcAddress(originalDll, "CompatValue");
		procAddresses[3] = GetProcAddress(originalDll, "CreateDXGIFactory");
		procAddresses[4] = GetProcAddress(originalDll, "CreateDXGIFactory1");
		procAddresses[5] = GetProcAddress(originalDll, "CreateDXGIFactory2");
		procAddresses[6] = GetProcAddress(originalDll, "DXGID3D10CreateDevice");
		procAddresses[7] = GetProcAddress(originalDll, "DXGID3D10CreateLayeredDevice");
		procAddresses[8] = GetProcAddress(originalDll, "DXGID3D10ETWRundown");
		procAddresses[9] = GetProcAddress(originalDll, "DXGID3D10GetLayeredDeviceSize");
		procAddresses[10] = GetProcAddress(originalDll, "DXGID3D10RegisterLayers");
		procAddresses[11] = GetProcAddress(originalDll, "DXGIDeclareAdapterRemovalSupport");
		procAddresses[12] = GetProcAddress(originalDll, "DXGIDumpJournal");
		procAddresses[13] = GetProcAddress(originalDll, "DXGIGetDebugInterface1");
		procAddresses[14] = GetProcAddress(originalDll, "DXGIReportAdapterConfiguration");
		procAddresses[15] = GetProcAddress(originalDll, "DXGIRevertToSxS");
		procAddresses[16] = GetProcAddress(originalDll, "PIXBeginCapture");
		procAddresses[17] = GetProcAddress(originalDll, "PIXEndCapture");
		procAddresses[18] = GetProcAddress(originalDll, "PIXGetCaptureState");
		procAddresses[19] = GetProcAddress(originalDll, "SetAppCompatStringPointer");
		procAddresses[20] = GetProcAddress(originalDll, "UpdateHMDEmulationStatus");

		// Start hooking on a new thread
		CreateThread(0, 0x1000, &MainThread, 0, 0, NULL);
	}
	else if (reason == DLL_PROCESS_DETACH)
	{

#if injectable == 0
		FreeLibrary(originalDll);
#endif
		return 1;
	}

	return 1;
}


/* 
* These are the functions our .dll exports, 
* we jump to the real .dll's functions to do the job for us.
* I used a tool to automatically create a template proxy .dll,
* https://www.codeproject.com/Articles/1179147/ProxiFy-Automatic-Proxy-DLL-Generation
* This way you don't have to manually write all the exports
*/
extern "C"
{
	FARPROC procAddr = NULL;

	void PROXY_ApplyCompatResolutionQuirking() {
		procAddr = procAddresses[0];
		JmpToAddr;
	}
	void PROXY_CompatString() {
		procAddr = procAddresses[1];
		JmpToAddr;
	}
	void PROXY_CompatValue() {
		procAddr = procAddresses[2];
		JmpToAddr;
	}
	void PROXY_CreateDXGIFactory() {
		procAddr = procAddresses[3];
		JmpToAddr;
	}
	void PROXY_CreateDXGIFactory1() {
		procAddr = procAddresses[4];
		JmpToAddr;
	}
	void PROXY_CreateDXGIFactory2() {
		procAddr = procAddresses[5];
		JmpToAddr;
	}
	void PROXY_DXGID3D10CreateDevice() {
		procAddr = procAddresses[6];
		JmpToAddr;
	}
	void PROXY_DXGID3D10CreateLayeredDevice() {
		procAddr = procAddresses[7];
		JmpToAddr;
	}
	void PROXY_DXGID3D10ETWRundown() {
		procAddr = procAddresses[8];
		JmpToAddr;
	}
	void PROXY_DXGID3D10GetLayeredDeviceSize() {
		procAddr = procAddresses[9];
		JmpToAddr;
	}
	void PROXY_DXGID3D10RegisterLayers() {
		procAddr = procAddresses[10];
		JmpToAddr;
	}
	void PROXY_DXGIDeclareAdapterRemovalSupport() {
		procAddr = procAddresses[11];
		JmpToAddr;
	}
	void PROXY_DXGIDumpJournal() {
		procAddr = procAddresses[12];
		JmpToAddr;
	}
	void PROXY_DXGIGetDebugInterface1() {
		procAddr = procAddresses[13];
		JmpToAddr;
	}
	void PROXY_DXGIReportAdapterConfiguration() {
		procAddr = procAddresses[14];
		JmpToAddr;
	}
	void PROXY_DXGIRevertToSxS() {
		procAddr = procAddresses[15];
		JmpToAddr;
	}
	void PROXY_PIXBeginCapture() {
		procAddr = procAddresses[16];
		JmpToAddr;
	}
	void PROXY_PIXEndCapture() {
		procAddr = procAddresses[17];
		JmpToAddr;
	}
	void PROXY_PIXGetCaptureState() {
		procAddr = procAddresses[18];
		JmpToAddr;
	}
	void PROXY_SetAppCompatStringPointer() {
		procAddr = procAddresses[19];
		JmpToAddr;
	}
	void PROXY_UpdateHMDEmulationStatus() {
		procAddr = procAddresses[20];
		JmpToAddr;
	}
}
