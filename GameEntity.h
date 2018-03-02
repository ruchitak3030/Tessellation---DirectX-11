#pragma once
#include "DXCore.h"
#include "Mesh.h"
#include <DirectXMath.h>

using namespace DirectX;

class GameEntity
{
public:
	GameEntity(Mesh* mesh);
	~GameEntity();

	void UpdateWorldMatrix();

	void Move(float x, float y, float z) { position.x += x;	position.y += y;	position.z += z; }
	void Rotate(float x, float y, float z) { rotation.x += x;	rotation.y += y;	rotation.z += z; }

	void SetPosition(float x, float y, float z) { position.x = x;	position.y = y;		position.z = z; }
	void SetRotation(float x, float y, float z) { rotation.x = x;	rotation.y = y;		rotation.z = z; }
	void SetScale(float x, float y, float z) { scale.x = x;		scale.y = y;		scale.z = z; }

	XMFLOAT3 GetPosition();

	Mesh* GetMesh() { return _mesh; }
	//Material* GetMaterial() { return _material; }
	XMFLOAT4X4* GetWorldMatrix() { return &worldMatrix; }

private:
	Mesh* _mesh;
	//Material* _material;

	XMFLOAT4X4 worldMatrix;
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;
};

