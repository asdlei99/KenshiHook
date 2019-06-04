#include <Windows.h>
#include <iostream>
#include "Core.h"

HINSTANCE thisInstance = 0;
HINSTANCE originalDll = 0;
FARPROC procAddresses[21];

DWORD WINAPI MainThread(LPVOID lpParam)
{
	Core core = Core();
	core.Init();

	return S_OK;
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{

	if (reason == DLL_PROCESS_ATTACH)
	{
		thisInstance = hInst;
		originalDll = LoadLibrary("dxgi_.dll");
		if(!originalDll) return false;

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

		CreateThread(0, 0x1000, &MainThread, 0, 0, NULL);
	}
	else if (reason == DLL_PROCESS_DETACH)
	{
		FreeLibrary(originalDll);
		return 1;
	}

	return 1;
}

extern "C"
{
	FARPROC procAddr = NULL;

	//Assembly function defined in JmpToAddr.asm
	int JmpToAddr();

	void PROXY_ApplyCompatResolutionQuirking() {
		procAddr = procAddresses[0];
		JmpToAddr();
	}
	void PROXY_CompatString() {
		procAddr = procAddresses[1];
		JmpToAddr();
	}
	void PROXY_CompatValue() {
		procAddr = procAddresses[2];
		JmpToAddr();
	}
	void PROXY_CreateDXGIFactory() {
		procAddr = procAddresses[3];
		JmpToAddr();
	}
	void PROXY_CreateDXGIFactory1() {
		procAddr = procAddresses[4];
		JmpToAddr();
	}
	void PROXY_CreateDXGIFactory2() {
		procAddr = procAddresses[5];
		JmpToAddr();
	}
	void PROXY_DXGID3D10CreateDevice() {
		procAddr = procAddresses[6];
		JmpToAddr();
	}
	void PROXY_DXGID3D10CreateLayeredDevice() {
		procAddr = procAddresses[7];
		JmpToAddr();
	}
	void PROXY_DXGID3D10ETWRundown() {
		procAddr = procAddresses[8];
		JmpToAddr();
	}
	void PROXY_DXGID3D10GetLayeredDeviceSize() {
		procAddr = procAddresses[9];
		JmpToAddr();
	}
	void PROXY_DXGID3D10RegisterLayers() {
		procAddr = procAddresses[10];
		JmpToAddr();
	}
	void PROXY_DXGIDeclareAdapterRemovalSupport() {
		procAddr = procAddresses[11];
		JmpToAddr();
	}
	void PROXY_DXGIDumpJournal() {
		procAddr = procAddresses[12];
		JmpToAddr();
	}
	void PROXY_DXGIGetDebugInterface1() {
		procAddr = procAddresses[13];
		JmpToAddr();
	}
	void PROXY_DXGIReportAdapterConfiguration() {
		procAddr = procAddresses[14];
		JmpToAddr();
	}
	void PROXY_DXGIRevertToSxS() {
		procAddr = procAddresses[15];
		JmpToAddr();
	}
	void PROXY_PIXBeginCapture() {
		procAddr = procAddresses[16];
		JmpToAddr();
	}
	void PROXY_PIXEndCapture() {
		procAddr = procAddresses[17];
		JmpToAddr();
	}
	void PROXY_PIXGetCaptureState() {
		procAddr = procAddresses[18];
		JmpToAddr();
	}
	void PROXY_SetAppCompatStringPointer() {
		procAddr = procAddresses[19];
		JmpToAddr();
	}
	void PROXY_UpdateHMDEmulationStatus() {
		procAddr = procAddresses[20];
		JmpToAddr();
	}
}
