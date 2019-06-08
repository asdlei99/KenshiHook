#include "Mesh.h"


XMFLOAT3 Mesh::GetPos()
{
	return normDevCoords;
}

ID3D11Buffer* Mesh::GetVertexBuffer()
{
	return vertexBuffer;
}

D3D11_BUFFER_DESC Mesh::GetVertexDesc()
{
	return vertexDesc;
}

D3D11_SUBRESOURCE_DATA Mesh::GetVertexSubData()
{
	return vertexSubData;
}

ID3D11Buffer* Mesh::GetIndexBuffer()
{
	return indexBuffer;
}

D3D11_BUFFER_DESC Mesh::GetIndexDesc()
{
	return indexDesc;
}

D3D11_SUBRESOURCE_DATA Mesh::GetIndexSubData()
{
	return indexSubData;
}

int Mesh::GetNumIndices()
{
	return indices.size();
}

float Mesh::GetSize()
{
	return size;
}

int Mesh::GetTextureIndex()
{
	return textureIndex;
}

std::string Mesh::GetMeshClassName()
{
	return meshClassName;
}
