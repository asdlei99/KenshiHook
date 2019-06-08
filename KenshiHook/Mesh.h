#pragma once
#include "Vertex.h"
#include <vector>
#include <d3d11.h>
#include <typeinfo>

using namespace DirectX;

class Mesh
{
protected:
	XMFLOAT3 normDevCoords;

	std::vector<Vertex> vertices;
	ID3D11Buffer* vertexBuffer;
	D3D11_BUFFER_DESC vertexDesc = { 0 };
	D3D11_SUBRESOURCE_DATA vertexSubData = { 0 };

	std::vector<unsigned int> indices;
	ID3D11Buffer* indexBuffer;
	D3D11_BUFFER_DESC indexDesc = { 0 };
	D3D11_SUBRESOURCE_DATA indexSubData = { 0 };

	float size; // An arbitrary size value for the mesh
	int textureIndex = -1;
	std::string meshClassName;

public:
	XMFLOAT3 GetPos();

	ID3D11Buffer** GetVertexBuffer();
	D3D11_BUFFER_DESC GetVertexDesc();
	D3D11_SUBRESOURCE_DATA GetVertexSubData();

	ID3D11Buffer** GetIndexBuffer();
	D3D11_BUFFER_DESC GetIndexDesc();
	D3D11_SUBRESOURCE_DATA GetIndexSubData();

	int GetNumIndices();
	float GetSize();
	int GetTextureIndex();
	std::string GetMeshClassName();
};