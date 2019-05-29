#include "Core.h"

// Variables for use with assembly code
extern "C" int FixAndReturn();
extern "C" int JmpToAbs64AddrPushPop();
extern "C" QWORD absAddr = NULL;
extern "C" QWORD presentAddr = NULL;
extern "C" QWORD jmpBackAddr = NULL;

// Pointers to functions
PresentFunction originalPresentFunction;
PresentFunction newPresentFunction;

extern "C" __int64 __fastcall Present(IDXGISwapChain *pSwapChain, UINT SyncInterval, UINT Flags)
{
	//std::ofstream file;
	//file.open("HELLO FROM PRESENT.txt");
	//file << pSwapChain;
	//file.close();

	return originalPresentFunction(pSwapChain, SyncInterval, Flags);
}

void Core::Init()
{
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON", "w", stdout);
	SetWindowText(GetConsoleWindow(), "KenshiDXHook");

	PrintDebugMsg("test");

	originalDllBaseAddress = (QWORD)GetModuleHandleA("dxgi_.dll");
	originalPresentFunctionOffset = 0x5070;
	originalPresentFunction = (PresentFunction)(originalDllBaseAddress + (QWORD)originalPresentFunctionOffset);

	PrintDebugMsg((void*)originalDllBaseAddress);
	PrintDebugMsg((void*)originalPresentFunctionOffset);
	PrintDebugMsg(originalPresentFunction);

	PrintDebugMsg("----------------");

	Hook(originalPresentFunction, FixAndReturn, 14);
}

bool Core::Hook(PresentFunction originalFunction, void* newFunction, int length)
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
	*(QWORD*)((QWORD)originalFunction + 6) = (QWORD)newFunction;

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
