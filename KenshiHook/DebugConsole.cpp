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

	std::string preMsg = "";

	for (int i = 1; i < counter; i++)
	{
		preMsg += "-";
	}

	if(counter > 0) preMsg += ">";

	switch (msgType)
	{
	case(STARTPROCESS):
		printf(std::string(preMsg + "[+] " + msg + "\n").c_str(), value);
		counter++;
		break;
	case(PROGRESS):
		printf(std::string(preMsg + " " + msg + "\n").c_str(), value);
		break;
	case(COMPLETE):
		printf(std::string(preMsg + " " + msg + "\n").c_str(), value);
		counter--;
		break;
	case(FAILED): 
		printf(std::string("[!] " + msg + "\n").c_str(), value);
		break;
	}

}