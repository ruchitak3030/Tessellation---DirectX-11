#include "Game.h"
#include "Vertex.h"

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
	
	/*if (vertexBuffer) { vertexBuffer->Release(); }
	if (indexBuffer) { indexBuffer->Release(); }*/

	delete vertexShader;
	delete pixelShader;
	delete hullShader;
	delete domainShader;

	delete triangleMesh1;
	delete triangleMesh2;

	rsState->Release();
}


void Game::Init()
{
	LoadShaders();
	CreateMatrices();
	CreateBasicGeometry();


	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	device->CreateRasterizerState(&rasterDesc, &rsState);
	

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
}


void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	if (!vertexShader->LoadShaderFile(L"Debug/VertexShader.cso"))
		vertexShader->LoadShaderFile(L"VertexShader.cso");		

	pixelShader = new SimplePixelShader(device, context);
	if(!pixelShader->LoadShaderFile(L"Debug/PixelShader.cso"))	
		pixelShader->LoadShaderFile(L"PixelShader.cso");

	hullShader = new SimpleHullShader(device, context);
	if (!hullShader->LoadShaderFile(L"Debug/HullShader.cso"))
		hullShader->LoadShaderFile(L"HullShader.cso");

	domainShader = new SimpleDomainShader(device, context);
	if (!domainShader->LoadShaderFile(L"Debug/DomainShader.cso"))
		domainShader->LoadShaderFile(L"DomainShader.cso");

}




void Game::CreateMatrices()
{
	
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up  = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V   = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}


void Game::CreateBasicGeometry()
{
	
	XMFLOAT4 red	= XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green	= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue	= XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	Vertex vertices1[] = 
	{
		{ XMFLOAT3(-2.0f, +1.0f, +0.0f), green },
		{ XMFLOAT3( -1.0f, -1.0f, +0.0f), green },
		{ XMFLOAT3(-3.0f, -1.0f, +0.0f), green },
	};
		
	unsigned int indices1[] = { 0, 1, 2 };

	Vertex vertices2[] =
	{
		{ XMFLOAT3(+2.0f, +1.0f, +0.0f), green },
		{ XMFLOAT3(+3.0f, -1.0f, +0.0f), green },
		{ XMFLOAT3(+1.0f, -1.0f, +0.0f), green },
	};

	unsigned int indices2[] = { 0, 1, 2 };

	
	triangleMesh1 = new Mesh(vertices1, 3, indices1, 3, device);
	triangleMesh2 = new Mesh(vertices2, 3, indices2, 3, device);

	
}


void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		(float)width / height,	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

void Game::Update(float deltaTime, float totalTime)
{
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

	ID3D11Buffer* vertexBuffer = triangleMesh1->GetVertexBuffer();
	ID3D11Buffer* indexBuffer = triangleMesh1->GetIndexBuffer();

	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	vertexShader->CopyAllBufferData();
	vertexShader->SetShader();

	hullShader->SetFloat("tessellationAmount", 12.0f);
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
	

	context->DrawIndexed(triangleMesh1->GetIndexCount(), 0, 0);


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


	context->DrawIndexed(triangleMesh2->GetIndexCount(), 0, 0);



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