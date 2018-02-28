#pragma once

#include "Vertex.h"
#include "DXCore.h"

class Mesh
{
public:
	Mesh(Vertex* vertices, int numVertices, unsigned int* indices, int numIndices, ID3D11Device* device );
	Mesh(const char* objFile, ID3D11Device* device);
	~Mesh();

	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetIndexCount();

private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int indexCount;

	void CreateBuffers(Vertex* vertices, int numVertices, unsigned int* indices, int numIndices, ID3D11Device* device);
	void CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices);


};

