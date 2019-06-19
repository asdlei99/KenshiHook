#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <Windows.h>

enum MsgType
{
	STARTPROCESS,
	PROGRESS,
	COMPLETE,
	FAILED
};

class DebugConsole
{
private:
	std::vector<std::string> msgQueue;
	int counter = 0;

public:

	DebugConsole();
	DebugConsole(std::string consoleName);
	void PrintDebugMsg(std::string msg, void* variable, MsgType msgType);
};