#pragma once
#include "DXCore.h"
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
public:
	Camera(float x, float y, float z);
	~Camera();

	// Transformations
	void MoveRelative(float x, float y, float z);
	void MoveAbsolute(float x, float y, float z);
	void Rotate(float x, float y);

	// Updating
	void Update(float dt);
	void UpdateViewMatrix();
	void UpdateProjectionMatrix(float aspectRatio);

	// Getters
	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT4X4 GetView() { return viewMatrix; }
	XMFLOAT4X4 GetProjection() { return projMatrix; }

private:
	// Camera matrices
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projMatrix;

	// Transformations
	XMFLOAT3 startPosition;
	XMFLOAT3 position;
	XMFLOAT4 rotation;
	float xRotation;
	float yRotation;
};

