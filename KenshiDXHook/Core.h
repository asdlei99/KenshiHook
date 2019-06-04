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
	QWORD originalDllBaseAddress;
	QWORD originalPresentFunctionOffset;

public:
	DebugConsole console;
	Renderer renderer;
	void Init();
	void Update();
	void Hook(QWORD originalFunction, QWORD newFunction, int bytes);
	~Core();
};