#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <Windows.h>

class DebugConsole
{
private:
	std::vector<std::string> msgQueue;

public:
	enum MsgType
	{
		STARTPROCESS,
		PROGRESS,
		FAILED
	};
	DebugConsole();
	DebugConsole(std::string consoleName);
	void PrintDebugMsg(std::string msg, void* value, MsgType msgType);
};