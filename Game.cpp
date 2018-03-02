#include "Game.h"
#include "Vertex.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"

// For the DirectX Math library
using namespace DirectX;


Game::Game(HINSTANCE hInstance)
	: DXCore( 
		hInstance,		   // The application's handle
		"Tessellation Demo",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	// Initialize fields
	// vertexBuffer = 0;
	//indexBuffer = 0;
	vertexShader = 0;
	pixelShader = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
}


Game::~Game()
{

	delete vertexShader;
	delete pixelShader;
	delete skyVertexShader;
	delete skyPixelShader;
	delete tessVertexShader;
	delete tessPixelShader;
	delete hullShader;
	delete domainShader;

	delete sphereMesh1;
	delete skyMesh;
	delete sphereEntity1;
	delete sphereEntity2;
	delete sphereEntity3;
	delete sphereEntity4;
	delete sphereEntity5;
	delete sphereEntity6;
	delete skyEntity;
	delete camera;
	

	sampler->Release();
	heightSampler->Release();
	sphereTextureSRV->Release();
	sphereNormalMapSRV->Release();
	sphereHeightMapSRV->Release();
	skyTextureSRV->Release();
	

	rsStateSolid->Release();
	rsStateWire->Release();
	skyRasterizerState->Release();
	skyDepthState->Release();
}


void Game::Init()
{
	LoadShaders();
	CreateMatrices();
	CreateBasicGeometry();
	LoadTextures();
	LoadMaterials();
	LoadSkyBox();


	D3D11_RASTERIZER_DESC rasterDescSolid;
	rasterDescSolid.AntialiasedLineEnable = false;
	rasterDescSolid.CullMode = D3D11_CULL_NONE;
	rasterDescSolid.DepthBias = 0;
	rasterDescSolid.DepthBiasClamp = 0.0f;
	rasterDescSolid.DepthClipEnable = false;
	rasterDescSolid.FillMode = D3D11_FILL_SOLID;
	rasterDescSolid.FrontCounterClockwise = false;
	rasterDescSolid.MultisampleEnable = false;
	rasterDescSolid.ScissorEnable = false;
	rasterDescSolid.SlopeScaledDepthBias = 0.0f;

	device->CreateRasterizerState(&rasterDescSolid, &rsStateSolid);

	D3D11_RASTERIZER_DESC rasterDescWireframe;
	rasterDescWireframe.AntialiasedLineEnable = false;
	rasterDescWireframe.CullMode = D3D11_CULL_NONE;
	rasterDescWireframe.DepthBias = 0;
	rasterDescWireframe.DepthBiasClamp = 0.0f;
	rasterDescWireframe.DepthClipEnable = false;
	rasterDescWireframe.FillMode = D3D11_FILL_WIREFRAME;
	rasterDescWireframe.FrontCounterClockwise = false;
	rasterDescWireframe.MultisampleEnable = false;
	rasterDescWireframe.ScissorEnable = false;
	rasterDescWireframe.SlopeScaledDepthBias = 0.0f;

	device->CreateRasterizerState(&rasterDescWireframe, &rsStateWire);

	


	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


}


void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	if (!vertexShader->LoadShaderFile(L"Debug/VertexShader.cso"))
		vertexShader->LoadShaderFile(L"VertexShader.cso");		

	pixelShader = new SimplePixelShader(device, context);
	if(!pixelShader->LoadShaderFile(L"Debug/PixelShader.cso"))	
		pixelShader->LoadShaderFile(L"PixelShader.cso");

	skyVertexShader = new SimpleVertexShader(device, context);
	if (!skyVertexShader->LoadShaderFile(L"Debug/SkyBoxVertexShader.cso"))
		skyVertexShader->LoadShaderFile(L"SkyBoxVertexShader.cso");

	skyPixelShader = new SimplePixelShader(device, context);
	if (!skyPixelShader->LoadShaderFile(L"Debug/SkyBoxPixelShader.cso"))
		skyPixelShader->LoadShaderFile(L"SkyBoxPixelShader.cso");

	tessVertexShader = new SimpleVertexShader(device, context);
	if (!tessVertexShader->LoadShaderFile(L"Debug/TessVertexShader.cso"))
		tessVertexShader->LoadShaderFile(L"TessVertexShader.cso");

	tessPixelShader = new SimplePixelShader(device, context);
	if (!tessPixelShader->LoadShaderFile(L"Debug/TessPixelShader.cso"))
		tessPixelShader->LoadShaderFile(L"TessPixelShader.cso");

	hullShader = new SimpleHullShader(device, context);
	if (!hullShader->LoadShaderFile(L"Debug/HullShader.cso"))
		hullShader->LoadShaderFile(L"HullShader.cso");

	domainShader = new SimpleDomainShader(device, context);
	if (!domainShader->LoadShaderFile(L"Debug/DomainShader.cso"))
		domainShader->LoadShaderFile(L"DomainShader.cso");

}




void Game::CreateMatrices()
{
	camera = new Camera(0, 0, -5);
	camera->UpdateProjectionMatrix((float)width / height);
}


void Game::CreateBasicGeometry()
{

	sphereMesh1 = new Mesh("Models/sphere.obj", device);
	sphereEntity1 = new GameEntity(sphereMesh1);
	sphereEntity1->SetPosition(-4.0f, 2.0f, 0.0f);
	sphereEntity1->SetRotation(0.0f, 0.0f, 0.0f);
	sphereEntity1->SetScale(0.5f, 0.5f, 0.5f);

	sphereEntity2 = new GameEntity(sphereMesh1);
	sphereEntity2->SetPosition(0.0f, 2.0f, 0.0f);
	sphereEntity2->SetRotation(0.0f, 0.0f, 0.0f);
	sphereEntity2->SetScale(0.5f, 0.5f, 0.5f);

	sphereEntity3 = new GameEntity(sphereMesh1);
	sphereEntity3->SetPosition(4.0f, 2.0f, 0.0f);
	sphereEntity3->SetRotation(0.0f, 0.0f, 0.0f);
	sphereEntity3->SetScale(0.5f, 0.5f, 0.5f);

	sphereEntity4 = new GameEntity(sphereMesh1);
	sphereEntity4->SetPosition(-4.0f, -2.0f, 0.0f);
	sphereEntity4->SetRotation(0.0f, 0.0f, 0.0f);
	sphereEntity4->SetScale(0.5f, 0.5f, 0.5f);

	sphereEntity5 = new GameEntity(sphereMesh1);
	sphereEntity5->SetPosition(0.0f, -2.0f, 0.0f);
	sphereEntity5->SetRotation(0.0f, 0.0f, 0.0f);
	sphereEntity5->SetScale(0.5f, 0.5f, 0.5f);

	sphereEntity6 = new GameEntity(sphereMesh1);
	sphereEntity6->SetPosition(4.0f, -2.0f, 0.0f);
	sphereEntity6->SetRotation(0.0f, 0.0f, 0.0f);
	sphereEntity6->SetScale(0.5f, 0.5f, 0.5f);

	skyMesh = new Mesh("Models/cube.obj", device);
	skyEntity = new GameEntity(skyMesh);
	skyEntity->SetPosition(0.0f, 0.0f, 0.0f);
	skyEntity->SetRotation(0.0f, 0.0f, 0.0f);
	skyEntity->SetScale(1.0f, 1.0f, 1.0f);

	
}

void Game::LoadTextures()
{
	HRESULT r = CreateWICTextureFromFile(device, context, L"Textures/sphereAlbedo.tif", 0, &sphereTextureSRV);
	HRESULT l = CreateWICTextureFromFile(device, context, L"Textures/sphereNormalMap.tif", 0, &sphereNormalMapSRV);
	HRESULT s = CreateWICTextureFromFile(device, context, L"Textures/sphereHeightMap.tif", 0, &sphereHeightMapSRV);

	
}

void Game::LoadMaterials()
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &sampler);

	D3D11_SAMPLER_DESC heightSamplerDesc = {};
	heightSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	heightSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	heightSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	heightSamplerDesc.BorderColor[0] = 0;
	heightSamplerDesc.BorderColor[1] = 0;
	heightSamplerDesc.BorderColor[2] = 0;
	heightSamplerDesc.BorderColor[3] = 0;
	heightSamplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	heightSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	heightSamplerDesc.MaxAnisotropy = 16;
	heightSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	heightSamplerDesc.MinLOD = 0.0f;
	heightSamplerDesc.MipLODBias = 0.0f;

	device->CreateSamplerState(&heightSamplerDesc, &heightSampler);

	// = new Material(pixelShader, vertexShader, sphereTextureSRV, sphereNormalMapSRV, sampler);
	
}

void Game::LoadSkyBox()
{
	HRESULT m = CreateDDSTextureFromFile(device, L"Textures/Stormy.dds", 0, &skyTextureSRV);

	//Sky Box Setup
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	rasterizerDesc.DepthClipEnable = true;
	device->CreateRasterizerState(&rasterizerDesc, &skyRasterizerState);

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateDepthStencilState(&depthStencilDesc, &skyDepthState);
}

void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	if (camera)
		camera->UpdateProjectionMatrix((float)width / height);
}

void Game::Update(float deltaTime, float totalTime)
{
	camera->Update(deltaTime);

	sphereEntity1->UpdateWorldMatrix();
	sphereEntity2->UpdateWorldMatrix();
	sphereEntity3->UpdateWorldMatrix();
	sphereEntity4->UpdateWorldMatrix();
	sphereEntity5->UpdateWorldMatrix();
	sphereEntity6->UpdateWorldMatrix();

	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();
}

void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.0f, 0.0f, 0.0f, 0.0f};

	
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	//context->RSSetState(rsState);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	/**********************************************************************************/
	/*DRAW SPHERE*/
	/**********************************************************************************/

	context->RSSetState(rsStateSolid);

	ID3D11Buffer* vertexBuffer = sphereEntity1->GetMesh()->GetVertexBuffer();
	ID3D11Buffer* indexBuffer = sphereEntity1->GetMesh()->GetIndexBuffer();

	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	vertexShader->SetShader();
	vertexShader->CopyAllBufferData();
	pixelShader->SetShader();
	pixelShader->CopyAllBufferData();

	tessVertexShader->SetMatrix4x4("world", *sphereEntity1->GetWorldMatrix());	
	tessVertexShader->SetShader();
	tessVertexShader->CopyAllBufferData();

	hullShader->SetFloat("tessellationAmount", 20.0f);
	hullShader->SetFloat3("padding", XMFLOAT3(0.0f, 0.0f, 0.0f));
	hullShader->SetShader();
	hullShader->CopyAllBufferData();

	
	domainShader->SetMatrix4x4("view", camera->GetView());
	domainShader->SetMatrix4x4("projection", camera->GetProjection());
	domainShader->SetFloat("Hscale", 0.5f);
	domainShader->SetFloat("Hbias", 1.0f);
	domainShader->SetShaderResourceView("heightSRV", 0);
	domainShader->SetSamplerState("heightSampler", 0);
	domainShader->SetShader();
	domainShader->CopyAllBufferData();

	//tessPixelShader->SetData("dirLight", &DirLight1, sizeof(DirectionalLight));
	tessPixelShader->SetShaderResourceView("textureSRV", sphereTextureSRV );
	tessPixelShader->SetShaderResourceView("normalMapSRV", sphereNormalMapSRV );
	tessPixelShader->SetSamplerState("basicSampler", sampler);
	tessPixelShader->SetShader();
	tessPixelShader->CopyAllBufferData();

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	context->DrawIndexed(sphereEntity1->GetMesh()->GetIndexCount(), 0, 0);
	/*******************************************************************************************/
	/*SPHERE2*/
	/********************************************************************************************/
	context->RSSetState(rsStateSolid);

	vertexBuffer = sphereEntity2->GetMesh()->GetVertexBuffer();
	indexBuffer = sphereEntity2->GetMesh()->GetIndexBuffer();

	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	vertexShader->SetShader();
	vertexShader->CopyAllBufferData();
	pixelShader->SetShader();
	pixelShader->CopyAllBufferData();

	tessVertexShader->SetMatrix4x4("world", *sphereEntity2->GetWorldMatrix());
	tessVertexShader->SetShader();
	tessVertexShader->CopyAllBufferData();

	hullShader->SetFloat("tessellationAmount", 20.0f);
	hullShader->SetFloat3("padding", XMFLOAT3(0.0f, 0.0f, 0.0f));
	hullShader->SetShader();
	hullShader->CopyAllBufferData();


	domainShader->SetMatrix4x4("view", camera->GetView());
	domainShader->SetMatrix4x4("projection", camera->GetProjection());
	domainShader->SetFloat("Hscale", 0.5f);
	domainShader->SetFloat("Hbias", 1.0f);
	domainShader->SetShaderResourceView("heightSRV", sphereHeightMapSRV);
	domainShader->SetSamplerState("heightSampler", heightSampler);
	domainShader->SetShader();
	domainShader->CopyAllBufferData();

	tessPixelShader->SetShaderResourceView("textureSRV", sphereTextureSRV);
	tessPixelShader->SetShaderResourceView("normalMapSRV", sphereNormalMapSRV);
	tessPixelShader->SetSamplerState("basicSampler", sampler);
	tessPixelShader->SetShader();
	tessPixelShader->CopyAllBufferData();

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	context->DrawIndexed(sphereEntity2->GetMesh()->GetIndexCount(), 0, 0);

	/*******************************************************************************************/
	/*SPHERE3*/
	/********************************************************************************************/
	context->RSSetState(rsStateWire);

	vertexBuffer = sphereEntity3->GetMesh()->GetVertexBuffer();
	indexBuffer = sphereEntity3->GetMesh()->GetIndexBuffer();

	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	vertexShader->SetShader();
	vertexShader->CopyAllBufferData();
	pixelShader->SetShader();
	pixelShader->CopyAllBufferData();

	tessVertexShader->SetMatrix4x4("world", *sphereEntity3->GetWorldMatrix());
	tessVertexShader->SetShader();
	tessVertexShader->CopyAllBufferData();

	hullShader->SetFloat("tessellationAmount", 20.0f);
	hullShader->SetFloat3("padding", XMFLOAT3(0.0f, 0.0f, 0.0f));
	hullShader->SetShader();
	hullShader->CopyAllBufferData();


	domainShader->SetMatrix4x4("view", camera->GetView());
	domainShader->SetMatrix4x4("projection", camera->GetProjection());
	domainShader->SetFloat("Hscale", 0.5f);
	domainShader->SetFloat("Hbias", 1.0f);
	domainShader->SetShaderResourceView("heightSRV", sphereHeightMapSRV);
	domainShader->SetSamplerState("heightSampler", heightSampler);
	domainShader->SetShader();
	domainShader->CopyAllBufferData();

	tessPixelShader->SetShaderResourceView("textureSRV", sphereTextureSRV);
	tessPixelShader->SetShaderResourceView("normalMapSRV", sphereNormalMapSRV);
	tessPixelShader->SetSamplerState("basicSampler", sampler);
	tessPixelShader->SetShader();
	tessPixelShader->CopyAllBufferData();

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	context->DrawIndexed(sphereEntity3->GetMesh()->GetIndexCount(), 0, 0);
	/*******************************************************************************************/
	/*SPHERE4*/
	/********************************************************************************************/
	context->RSSetState(rsStateSolid);

	vertexBuffer = sphereEntity4->GetMesh()->GetVertexBuffer();
	indexBuffer = sphereEntity4->GetMesh()->GetIndexBuffer();

	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	vertexShader->SetShader();
	vertexShader->CopyAllBufferData();
	pixelShader->SetShader();
	pixelShader->CopyAllBufferData();

	tessVertexShader->SetMatrix4x4("world", *sphereEntity4->GetWorldMatrix());
	tessVertexShader->SetShader();
	tessVertexShader->CopyAllBufferData();

	hullShader->SetFloat("tessellationAmount", 2.0f);
	hullShader->SetFloat3("padding", XMFLOAT3(0.0f, 0.0f, 0.0f));
	hullShader->SetShader();
	hullShader->CopyAllBufferData();


	domainShader->SetMatrix4x4("view", camera->GetView());
	domainShader->SetMatrix4x4("projection", camera->GetProjection());
	domainShader->SetFloat("Hscale", 0.5f);
	domainShader->SetFloat("Hbias", 1.0f);
	domainShader->SetShaderResourceView("heightSRV", 0);
	domainShader->SetSamplerState("heightSampler", 0);
	domainShader->SetShader();
	domainShader->CopyAllBufferData();

	tessPixelShader->SetShaderResourceView("textureSRV", sphereTextureSRV);
	tessPixelShader->SetShaderResourceView("normalMapSRV", sphereNormalMapSRV);
	tessPixelShader->SetSamplerState("basicSampler", sampler);
	tessPixelShader->SetShader();
	tessPixelShader->CopyAllBufferData();

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	context->DrawIndexed(sphereEntity4->GetMesh()->GetIndexCount(), 0, 0);
	/*******************************************************************************************/
	/*SPHERE5*/
	/********************************************************************************************/
	context->RSSetState(rsStateSolid);

	vertexBuffer = sphereEntity5->GetMesh()->GetVertexBuffer();
	indexBuffer = sphereEntity5->GetMesh()->GetIndexBuffer();

	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	vertexShader->SetShader();
	vertexShader->CopyAllBufferData();
	pixelShader->SetShader();
	pixelShader->CopyAllBufferData();

	tessVertexShader->SetMatrix4x4("world", *sphereEntity5->GetWorldMatrix());
	tessVertexShader->SetShader();
	tessVertexShader->CopyAllBufferData();

	hullShader->SetFloat("tessellationAmount", 2.0f);
	hullShader->SetFloat3("padding", XMFLOAT3(0.0f, 0.0f, 0.0f));
	hullShader->SetShader();
	hullShader->CopyAllBufferData();


	domainShader->SetMatrix4x4("view", camera->GetView());
	domainShader->SetMatrix4x4("projection", camera->GetProjection());
	domainShader->SetFloat("Hscale", 0.5f);
	domainShader->SetFloat("Hbias", 1.0f);
	domainShader->SetShaderResourceView("heightSRV", sphereHeightMapSRV);
	domainShader->SetSamplerState("heightSampler", heightSampler);
	domainShader->SetShader();
	domainShader->CopyAllBufferData();

	tessPixelShader->SetShaderResourceView("textureSRV", sphereTextureSRV);
	tessPixelShader->SetShaderResourceView("normalMapSRV", sphereNormalMapSRV);
	tessPixelShader->SetSamplerState("basicSampler", sampler);
	tessPixelShader->SetShader();
	tessPixelShader->CopyAllBufferData();

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	context->DrawIndexed(sphereEntity5->GetMesh()->GetIndexCount(), 0, 0);
	/*******************************************************************************************/
	/*SPHERE6*/
	/********************************************************************************************/
	context->RSSetState(rsStateWire);

	vertexBuffer = sphereEntity6->GetMesh()->GetVertexBuffer();
	indexBuffer = sphereEntity6->GetMesh()->GetIndexBuffer();

	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	vertexShader->SetShader();
	vertexShader->CopyAllBufferData();
	pixelShader->SetShader();
	pixelShader->CopyAllBufferData();

	tessVertexShader->SetMatrix4x4("world", *sphereEntity6->GetWorldMatrix());
	tessVertexShader->SetShader();
	tessVertexShader->CopyAllBufferData();

	hullShader->SetFloat("tessellationAmount", 2.0f);
	hullShader->SetFloat3("padding", XMFLOAT3(0.0f, 0.0f, 0.0f));
	hullShader->SetShader();
	hullShader->CopyAllBufferData();


	domainShader->SetMatrix4x4("view", camera->GetView());
	domainShader->SetMatrix4x4("projection", camera->GetProjection());
	domainShader->SetFloat("Hscale", 0.5f);
	domainShader->SetFloat("Hbias", 1.0f);
	domainShader->SetShaderResourceView("heightSRV", sphereHeightMapSRV);
	domainShader->SetSamplerState("heightSampler", heightSampler);
	domainShader->SetShader();
	domainShader->CopyAllBufferData();

	tessPixelShader->SetShaderResourceView("textureSRV", sphereTextureSRV);
	tessPixelShader->SetShaderResourceView("normalMapSRV", sphereNormalMapSRV);
	tessPixelShader->SetSamplerState("basicSampler", sampler);
	tessPixelShader->SetShader();
	tessPixelShader->CopyAllBufferData();

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	context->DrawIndexed(sphereEntity6->GetMesh()->GetIndexCount(), 0, 0);

	/********************************************************************************************/
	/*DRAW SKYBOX*/
	/*******************************************************************************************/

	context->HSSetShader(0, 0,0);
	context->DSSetShader(0, 0, 0);

	vertexBuffer = skyEntity->GetMesh()->GetVertexBuffer();
	indexBuffer = skyEntity->GetMesh()->GetIndexBuffer();

	skyVertexShader->SetMatrix4x4("view", camera->GetView());
	skyVertexShader->SetMatrix4x4("projection", camera->GetProjection());

	skyVertexShader->SetShader();
	skyVertexShader->CopyAllBufferData();

	skyPixelShader->SetShaderResourceView("Sky", skyTextureSRV);

	skyPixelShader->SetShader();
	skyPixelShader->CopyAllBufferData();

	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->RSSetState(skyRasterizerState);
	context->OMSetDepthStencilState(skyDepthState, 0);

	context->DrawIndexed(skyMesh->GetIndexCount(), 0, 0);

	context->RSSetState(0);
	context->OMSetDepthStencilState(0, 0);

	/*context->DrawIndexed(triangleMesh1->GetIndexCount(), 0, 0);


	vertexBuffer = triangleMesh2->GetVertexBuffer();
	indexBuffer = triangleMesh2->GetIndexBuffer();

	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	vertexShader->CopyAllBufferData();
	vertexShader->SetShader();

	hullShader->SetFloat("tessellationAmount", 5.0f);
	hullShader->SetFloat3("padding", XMFLOAT3(0.0f, 0.0f, 0.0f));
	hullShader->SetShader();
	hullShader->CopyAllBufferData();

	domainShader->SetMatrix4x4("world", worldMatrix);
	domainShader->SetMatrix4x4("view", viewMatrix);
	domainShader->SetMatrix4x4("projection", projectionMatrix);
	domainShader->SetShader();
	domainShader->CopyAllBufferData();

	pixelShader->SetShader();
	pixelShader->CopyAllBufferData();


	context->DrawIndexed(triangleMesh2->GetIndexCount(), 0, 0);*/



	swapChain->Present(0, 0);
}


#pragma region Mouse Input

void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion