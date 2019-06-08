#pragma once
#include "Mesh.h"

class TexturedBox : public Mesh
{
public:
	TexturedBox(float normDevX, float normDevY, float size, int textureIndex);
};