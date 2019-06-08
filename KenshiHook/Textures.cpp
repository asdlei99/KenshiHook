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

	// Convert our filepath string to a widechar array, because Windows likes widechars
	wchar_t wchar[200];
	mbstowcs(wchar, filepath.c_str(), filepath.length());

	HRESULT texResult = CreateDDSTextureFromFile(device, wchar, nullptr, &texture);
	_com_error texErr(texResult);
	console->PrintDebugMsg("Texture HRESULT: %s", (void*)texErr.ErrorMessage(), console->PROGRESS);

	if (FAILED(texResult))
	{
		console->PrintDebugMsg("Texture loading failed: %s - invalid .DDS format?", (void*)filepath.c_str(), console->MsgType::FAILED);
		return false;
	}
	
	textures.push_back(texture);

	return true;
}

ID3D11ShaderResourceView* Textures::GetTexture(int textureIndex)
{
	return textures.at(textureIndex);
}

Textures::~Textures()
{
}
