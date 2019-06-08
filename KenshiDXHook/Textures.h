#pragma once
#include <vector>
#include <d3d11.h>
#include <DDSTextureLoader.h>
#include "DebugConsole.h"

using namespace DirectX;

class Textures
{
private:
	DebugConsole* console;
	std::vector<ID3D11ShaderResourceView*> textures;
	ID3D11Device* device = nullptr;

public:
	Textures() {};
	Textures(DebugConsole* console);
	void SetDevice(ID3D11Device* device);
	bool LoadTexture(std::string filepath);
	ID3D11ShaderResourceView* GetTexture(int textureIndex);
	~Textures();
};