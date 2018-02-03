#include "Mesh.h"


Mesh::Mesh(Vertex * vertices, int numVertices, unsigned int * indices, int numIndices, ID3D11Device * device)
{
	CreateBuffers(vertices, numVertices, indices, numIndices, device);
}

Mesh::Mesh(const char * objFile, ID3D11Device * device)
{
}

Mesh::~Mesh()
{
	
}

ID3D11Buffer * Mesh::GetVertexBuffer()
{
	return vertexBuffer;
}

ID3D11Buffer * Mesh::GetIndexBuffer()
{
	return indexBuffer;
}

int Mesh::GetIndexCount()
{
	return indexCount;
}

void Mesh::CreateBuffers(Vertex * vertices, int numVertices, unsigned int * indices, int numIndices, ID3D11Device * device)
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * numVertices;       // 3 = number of vertices in the buffer
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;			// Tells DirectX this is a vertex buffer
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertices;

	device->CreateBuffer(&vbd, &initialVertexData, &vertexBuffer);


	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned int) * numIndices;         // 3 = number of indices in the buffer
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;					// Tells DirectX this is an index buffer
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indices;

	indexCount = numIndices;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	device->CreateBuffer(&ibd, &initialIndexData, &indexBuffer);
}

