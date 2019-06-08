#include "Core.h"

// Variables for use with assembly code
extern "C" int JmpToHookAndJmpBack();
extern "C" QWORD presentAddr = NULL;
extern "C" QWORD jmpBackAddr = NULL;

bool first = true;
Core* coreRef;


HRESULT __fastcall Present(IDXGISwapChain *swapChain, UINT syncInterval, UINT flags)
{

	if (first)
	{
		DebugConsole* consoleRef = &coreRef->console;
		consoleRef->PrintDebugMsg("Hello from function hook", nullptr, consoleRef->MsgType::PROGRESS);
		consoleRef->PrintDebugMsg("Swap chain address: %p", (void*)swapChain, consoleRef->MsgType::COMPLETE);
		first = false;
	}

	coreRef->Render(swapChain, syncInterval, flags);

	return ((PresentFunction)coreRef->newPresentReturn)(swapChain, syncInterval, flags);
}

void Core::Init()
{
	coreRef = this;
	console = DebugConsole("KenshiDXHook");
	console.PrintDebugMsg("Initializing hook...", nullptr, console.MsgType::STARTPROCESS);

	renderer = Renderer(&console);
	textures = Textures(&console);

	originalDllBaseAddress = (QWORD)GetModuleHandleA("dxgi_.dll");
	originalPresentFunctionOffset = 0x5070;
	originalPresentFunction = (PresentFunction)(originalDllBaseAddress + (QWORD)originalPresentFunctionOffset);

	console.PrintDebugMsg("dxgi.dll base address: %p", (void*)originalDllBaseAddress, console.MsgType::PROGRESS);
	console.PrintDebugMsg("Present offset: %p", (void*)originalPresentFunctionOffset, console.MsgType::PROGRESS);
	console.PrintDebugMsg("Present address: %p", (void*)originalPresentFunction, console.MsgType::PROGRESS);

	Hook((QWORD)originalPresentFunction, (QWORD)JmpToHookAndJmpBack, 14);

	console.PrintDebugMsg("Present function hooked successfully", nullptr, console.MsgType::PROGRESS);

	//Update();
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

	VirtualProtect((void*)originalFunction, length, oldProtection, &oldProtection);

	originalPresentFunction = (PresentFunction)((QWORD)originalFunction + length);

	presentAddr = (QWORD)Present;
	jmpBackAddr = (QWORD)originalPresentFunction;

	// We want a jump from our new present back to the original instructions
	newPresentReturn = (QWORD)JmpToHookAndJmpBack + 6;

	return;
}

void Core::Update()
{
	// Do things here if you want to
}

void Core::Render(IDXGISwapChain *swapChain, UINT syncInterval, UINT flags)
{

	if (!renderer.IsInitialized())
	{
		if(!renderer.Init(swapChain, syncInterval, flags)) return;
	}

	if (!texturesLoaded)
	{
		console.PrintDebugMsg("Loading textures...", nullptr, console.MsgType::STARTPROCESS);
		textures.SetDevice(renderer.GetDevice());
		textures.LoadTexture(".\\textures\\test.dds");
		renderer.SetTextureManager(&textures);
		texturesLoaded = true;
		console.PrintDebugMsg("All textures loaded", nullptr, console.MsgType::COMPLETE);
	}

	if (!meshesCreated)
	{
		console.PrintDebugMsg("Loading meshes...", nullptr, console.MsgType::STARTPROCESS);
		AddMesh(TexturedBox(0.0f, 0.0f, 0.2f, 0));
		meshesCreated = true;
		console.PrintDebugMsg("All meshes loaded", nullptr, console.MsgType::COMPLETE);
	}

	renderer.Render(swapChain, syncInterval, flags, &thingsToDraw);
}

void Core::AddMesh(Mesh mesh)
{

	if (FAILED(renderer.CreateBufferForMesh(mesh.GetVertexDesc(), mesh.GetVertexSubData(), mesh.GetVertexBuffer())))
	{
		console.PrintDebugMsg("Failed to create vertex buffer for mesh", nullptr, console.MsgType::FAILED);
		return;
	}

	console.PrintDebugMsg("SJEKK %p", (void*)mesh.GetVertexBuffer(), console.MsgType::PROGRESS);

	if (FAILED(renderer.CreateBufferForMesh(mesh.GetIndexDesc(), mesh.GetIndexSubData(), mesh.GetIndexBuffer())))
	{
		console.PrintDebugMsg("Failed to create index buffer for mesh", nullptr, console.MsgType::FAILED);
		return;
	}

	console.PrintDebugMsg("Loaded a mesh for drawing: %s", (void*)mesh.GetMeshClassName().c_str(), console.MsgType::PROGRESS);
	thingsToDraw.push_back(mesh);
}

// Destructor doesn't seem to get called ever, oh well
Core::~Core()
{
	renderer.Cleanup();
	FreeConsole();
}
