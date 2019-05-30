#pragma once
#include <iostream>
#include <Windows.h>
#include <intrin.h>
#include <dxgi.h>
#include <d3d11.h>
#include <fstream>

#define PrintDebugMsg(msg) std::cout << msg << std::endl;

typedef unsigned __int64 QWORD; // Seems my C++ doesn't have QWORD
// An unsigned __int64 consists of 8 bytes in memory,
// and we are dealing with a 64-bit program, where the memory addresses are
// 8 bytes long, so we use QWORD (the same as unsigned __int64) to hold memory addresses

// Definition of the structure of the DXGI present function
typedef __int64 (__fastcall* PresentFunction)(IDXGISwapChain *pSwapChain, UINT SyncInterval, UINT Flags);

class Core
{
private:
	QWORD originalDllBaseAddress;
	QWORD originalPresentFunctionOffset;

public:
	void Init();
	bool Hook(PresentFunction originalFunction, QWORD newFunction, int bytes);
	~Core();
};