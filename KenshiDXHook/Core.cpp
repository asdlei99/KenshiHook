#include "Core.h"

// Variables for use with assembly code
extern "C" int JmpToHookAndJmpBack();
extern "C" QWORD presentAddr = NULL;
extern "C" QWORD jmpBackAddr = NULL;

// Pointer to original Present
PresentFunction originalPresentFunction;

// Where our Present will jump to after its done
QWORD newPresentReturn = NULL;

std::vector<std::string> pushToConsole = std::vector<std::string>();
bool first = true;

DebugConsole* consoleRef;
Renderer* rendererRef;


HRESULT __fastcall Present(IDXGISwapChain *swapChain, UINT syncInterval, UINT flags)
{

	if (first)
	{
		consoleRef->PrintDebugMsg("Hello from function hook", nullptr, consoleRef->PROGRESS);
		consoleRef->PrintDebugMsg("Swap chain address: %p", (void*)swapChain, consoleRef->PROGRESS);
		first = false;
	}

	rendererRef->Render(swapChain, syncInterval, flags, consoleRef);

	return ((PresentFunction)newPresentReturn)(swapChain, syncInterval, flags);
}

void Core::Init()
{
	console = DebugConsole("KenshiDXHook");
	consoleRef = &console;
	console.PrintDebugMsg("Initializing hook...", nullptr, console.MsgType::STARTPROCESS);

	Renderer renderer = Renderer();
	rendererRef = &renderer;

	originalDllBaseAddress = (QWORD)GetModuleHandleA("dxgi_.dll");
	originalPresentFunctionOffset = 0x5070;
	originalPresentFunction = (PresentFunction)(originalDllBaseAddress + (QWORD)originalPresentFunctionOffset);

	console.PrintDebugMsg("dxgi.dll base address: %p", (void*)originalDllBaseAddress, console.MsgType::PROGRESS);
	console.PrintDebugMsg("Present offset: %p", (void*)originalPresentFunctionOffset, console.MsgType::PROGRESS);
	console.PrintDebugMsg("Present address: %p", (void*)originalPresentFunction, console.MsgType::PROGRESS);

	Hook((QWORD)originalPresentFunction, (QWORD)JmpToHookAndJmpBack, 14);

	console.PrintDebugMsg("Present function hooked successfully", nullptr, console.MsgType::PROGRESS);

	Update();
}

void Core::Update()
{

	while (true)
	{

		if (pushToConsole.size() > 0)
		{
			console.PrintDebugMsg(pushToConsole.at(0), nullptr, console.MsgType::PROGRESS);
			pushToConsole.erase(pushToConsole.begin());
		}

		Sleep(20);
	}

}

void Core::Hook(QWORD originalFunction, QWORD newFunction, int length)
{
	DWORD oldProtection;

	VirtualProtect((void*)originalFunction, length, PAGE_EXECUTE_READWRITE, &oldProtection);

	memset((void*)originalFunction, 0x90, length);

	// Place an absolute 64-bit jump (FF 25 00000000)
	// Bytes are flipped (because of endianness), so we use _byteswap
	// Fill the _byteswap function with 8 bytes so it doesn't return garbage
	*(QWORD*)originalFunction = _byteswap_uint64(0xFF25000000000000);


	// We placed an absolute 64-bit jump, that instruction uses only 6 bytes
	// on its own, but then reads an 8-byte address from the subsequent memory address
	// which we place now
	*(QWORD*)((QWORD)originalFunction + 6) = (QWORD)newFunction;

	DWORD temp;
	VirtualProtect((void*)originalFunction, length, oldProtection, &temp);

	originalPresentFunction = (PresentFunction)((QWORD)originalFunction + length);

	presentAddr = (QWORD)Present;
	jmpBackAddr = (QWORD)originalPresentFunction;

	// We want a jump from our new present back to the original instructions
	newPresentReturn = (QWORD)JmpToHookAndJmpBack + 6;

	return;
}

Core::~Core()
{
	FreeConsole();
}
