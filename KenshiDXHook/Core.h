#pragma once

#include <iostream>
#include <Windows.h>
#include <intrin.h>
#include <dxgi.h>
#include <d3d11.h>
#include <fstream>
#include <string>
#include "DebugConsole.h"
#include "Renderer.h"
#include "Mesh.h"
#include "TexturedBox.h"
#include "Textures.h"
#include <comdef.h>

// An unsigned __int64 consists of 8 bytes in memory,
// and we are dealing with a 64-bit program, where the memory addresses are
// 8 bytes long, so we use QWORD (the same as unsigned __int64) to hold memory addresses
typedef unsigned __int64 QWORD; // Seems my C++ doesn't have QWORD predefined

// Definition of the structure of the DXGI present function, so we can treat memory addresses
// as a function, and pass parameters to the registers and stack at the location
typedef HRESULT (__fastcall* PresentFunction)(IDXGISwapChain *pSwapChain, UINT SyncInterval, UINT Flags);

class Core
{
private:
	PresentFunction originalPresentFunction;
	QWORD originalDllBaseAddress;
	QWORD originalPresentFunctionOffset;
	bool texturesLoaded = false;
	bool meshesCreated = false;
	std::vector<Mesh> thingsToDraw = std::vector<Mesh>();

public:
	QWORD newPresentReturn;
	DebugConsole console;
	Renderer renderer;
	Textures textures;
	void Init();
	void Hook(QWORD originalFunction, QWORD newFunction, int bytes);
	void Update();
	void Render(IDXGISwapChain *swapChain, UINT syncInterval, UINT flags);
	void AddMesh(Mesh mesh);
	~Core();
};