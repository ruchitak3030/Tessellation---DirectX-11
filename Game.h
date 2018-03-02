#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Camera.h"
#include "Mesh.h"
#include "GameEntity.h"


class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateMatrices();
	void CreateBasicGeometry();
	void LoadTextures();
	void LoadMaterials();
	void LoadSkyBox();

	Mesh* sphereMesh1;
	Mesh* skyMesh;
	GameEntity* sphereEntity1;
	GameEntity* sphereEntity2;
	GameEntity* sphereEntity3;
	GameEntity* sphereEntity4;
	GameEntity* sphereEntity5;
	GameEntity* sphereEntity6;
	GameEntity* skyEntity;
	Camera* camera;


	//Texture Stuff
	ID3D11SamplerState* sampler;
	ID3D11SamplerState* heightSampler;
	ID3D11ShaderResourceView* sphereTextureSRV;
	ID3D11ShaderResourceView* sphereNormalMapSRV;
	ID3D11ShaderResourceView* sphereHeightMapSRV;
	ID3D11ShaderResourceView* skyTextureSRV;



	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	SimpleVertexShader* skyVertexShader;
	SimplePixelShader* skyPixelShader;
	SimpleVertexShader* tessVertexShader;
	SimplePixelShader* tessPixelShader;
	SimpleHullShader* hullShader;
	SimpleDomainShader* domainShader;

	ID3D11RasterizerState* rsStateSolid;
	ID3D11RasterizerState* rsStateWire;
	ID3D11RasterizerState* skyRasterizerState;
	ID3D11DepthStencilState* skyDepthState;

	// The matrices to go from model space to screen space
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
};

