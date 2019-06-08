#include "TexturedBox.h"

TexturedBox::TexturedBox(float normDevX, float normDevY, float size, int textureIndex)
{
	this->normDevCoords = XMFLOAT3(normDevX, normDevY, 1.0f);
	this->textureIndex = textureIndex;
	this->size = size;
	this->textureIndex = textureIndex;
	this->meshClassName = typeid(this).name();

	vertices =
	{
		{ XMFLOAT3(normDevX - size, normDevY - size, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(normDevX - size, normDevY + size, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
		{ XMFLOAT3(normDevX + size, normDevY + size, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(normDevX + size, normDevY - size, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }
	};

	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.ByteWidth = sizeof(Vertex) * vertices.size();
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.StructureByteStride = sizeof(Vertex);

	vertexSubData = { vertices.data(), 0, 0 };

	indices = 
	{
		0, 1, 2,
		0, 2, 3
	};

	ZeroMemory(&indexDesc, sizeof(indexDesc));
	indexDesc.ByteWidth = sizeof(unsigned int) * indices.size();
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.CPUAccessFlags = 0;
	indexDesc.MiscFlags = 0;

	indexSubData = { indices.data(), 0, 0 };
	//indexSubData.pSysMem = indices.data();
}
