#include "DebugConsole.h"

DebugConsole::DebugConsole()
{
}

DebugConsole::DebugConsole(std::string consoleName)
{
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	SetWindowText(GetConsoleWindow(), consoleName.c_str());
}

void DebugConsole::PrintDebugMsg(std::string msg, void* value, MsgType msgType)
{
	if (msg == "") return;

	switch (msgType)
	{
	case(0): // STARTPROCESS
		printf(std::string("[?] " + msg + "\n").c_str(), value);
		break;
	case(1): // PROGRESS
		printf(std::string("[+] " + msg + "\n").c_str(), value);
		break;
	case(2): // FAILED
		printf(std::string("[-] " + msg + "\n").c_str(), value);
		break;
	}

}