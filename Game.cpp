#include "Game.h"
#include "Vertex.h"
#include "WICTextureLoader.h"

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
	delete tessVertexShader;
	delete tessPixelShader;
	delete hullShader;
	delete domainShader;

	delete sphereMesh1;
	delete sphereEntity1;
	delete camera;
	delete sphereMaterial;

	

	sampler->Release();
	sphereTextureSRV->Release();
	sphereNormalMapSRV->Release();

	

	rsState->Release();
}


void Game::Init()
{
	LoadShaders();
	CreateMatrices();
	CreateBasicGeometry();
	LoadTextures();
	LoadMaterials();


	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = false;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	device->CreateRasterizerState(&rasterDesc, &rsState);


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
	camera = new Camera(0, 0, -10);
	camera->UpdateProjectionMatrix((float)width / height);
}


void Game::CreateBasicGeometry()
{

	sphereMesh1 = new Mesh("Models/sphere.obj", device);
	sphereEntity1 = new GameEntity(sphereMesh1, sphereMaterial);
	sphereEntity1->SetPosition(0.0f, 0.0f, 0.0f);
	sphereEntity1->SetRotation(0.0f, 0.0f, 0.0f);
	sphereEntity1->SetScale(0.5f, 0.5f, 0.5f);

	
}

void Game::LoadTextures()
{
	HRESULT r = CreateWICTextureFromFile(device, context, L"Textures/sphereRoughAlbedo.tif", 0, &sphereTextureSRV);
	HRESULT l = CreateWICTextureFromFile(device, context, L"Textures/sphereRoughNormal.tif", 0, &sphereNormalMapSRV);
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

	sphereMaterial = new Material(pixelShader, vertexShader, sphereTextureSRV, sphereNormalMapSRV, sampler);
	
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

	context->RSSetState(rsState);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

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
	domainShader->SetShader();
	domainShader->CopyAllBufferData();

	tessPixelShader->SetShaderResourceView("textureSRV", sphereTextureSRV );
	tessPixelShader->SetShaderResourceView("normalMapSRV", sphereNormalMapSRV );
	tessPixelShader->SetSamplerState("basicSampler", sampler );
	tessPixelShader->SetShader();
	tessPixelShader->CopyAllBufferData();

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	context->DrawIndexed(sphereEntity1->GetMesh()->GetIndexCount(), 0, 0);
	

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