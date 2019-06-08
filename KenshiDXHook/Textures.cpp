#include "Textures.h"

Textures::Textures(DebugConsole* console)
{
	this->console = console;
	textures = std::vector<ID3D11ShaderResourceView*>();
}

void Textures::SetDevice(ID3D11Device* device)
{
	this->device = device;
}

bool Textures::LoadTexture(std::string filepath)
{
	if (device == nullptr) return false;

	console->PrintDebugMsg("Loading texture: %s", (void*)filepath.c_str(), console->MsgType::PROGRESS);

	ID3D11ShaderResourceView* texture;

	wchar_t wchar[200];
	mbstowcs(wchar, filepath.c_str(), filepath.length());
	if (FAILED(CreateDDSTextureFromFile(device, wchar, nullptr, &texture)))
	{
		console->PrintDebugMsg("Texture loading failed", (void*)filepath.c_str(), console->MsgType::FAILED);
		return false;
	}
	
	textures.push_back(texture);

	console->PrintDebugMsg("Texture loaded", nullptr, console->MsgType::PROGRESS);
	return true;
}

ID3D11ShaderResourceView* Textures::GetTexture(int textureIndex)
{
	return textures.at(textureIndex);
}

Textures::~Textures()
{
}
