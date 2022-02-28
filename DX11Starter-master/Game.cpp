#include "Game.h"
#include "Vertex.h"
#include "Input.h"
#include <memory>

// Needed for a helper function to read compiled shader files from the hard drive
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true),			   // Show extra stats (fps) in title bar?
	vsync(false)
{
#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Note: Since we're using smart pointers (ComPtr),
	// we don't need to explicitly clean up those DirectX objects
	// - If we weren't using smart pointers, we'd need
	//   to call Release() on each DirectX object created in Game
	for (Entity* entity : entities) {
		delete entity;
	}
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateBasicGeometry();
	
	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Get size as the next multiple of 16 (instead of hardcoding a size here!) 
	//unsigned int size = sizeof(VertexShaderExternalData);
	//size = (size + 15) / 16 * 16; // This will work even if your struct size changes

	// Describe the constant buffer  
	/*D3D11_BUFFER_DESC cbDesc = {}; // Sets struct to all zeros  
	cbDesc.BindFlags  = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.ByteWidth  = size; // Must be a multiple of 16  
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;  
	cbDesc.Usage    = D3D11_USAGE_DYNAMIC;*/

	worldCam = std::make_shared<Camera>((float)this->width / this->height, XMFLOAT3(0, 0, -5));
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// and also created the Input Layout that describes our 
// vertex data to the rendering pipeline. 
// - Input Layout creation is done here because it must 
//    be verified against vertex shader byte code
// - We'll have that byte code already loaded below
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = std::make_shared<SimpleVertexShader>(device, context, GetFullPathTo_Wide(L"VertexShader.cso").c_str());
	pixelShader = std::make_shared<SimplePixelShader>(device, context,
	GetFullPathTo_Wide(L"PixelShader.cso").c_str());
}


void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	// - Note: Since we don't have a camera or really any concept of
	//    a "3d world" yet, we're simply describing positions within the
	//    bounds of how the rasterizer sees our screen: [-1 to +1] on X and Y
	// - This means (0,0) is at the very center of the screen.
	// - These are known as "Normalized Device Coordinates" or "Homogeneous 
	//    Screen Coords", which are ways to describe a position without
	//    knowing the exact size (in pixels) of the image/window/etc.  
	// - Long story short: Resizing the window also resizes the triangle,
	//    since we're describing the triangle in terms of the window itself
	Vertex vertices[] =
	{
		{ XMFLOAT3(+0.0f, +0.5f, +0.0f), red },
		{ XMFLOAT3(+0.5f, -0.5f, +0.0f), blue },
		{ XMFLOAT3(-0.5f, -0.5f, +0.0f), green },
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	unsigned int indices[] = { 0, 1, 2 };
	triangle = std::make_shared<Mesh>(vertices, 3, indices, 3, device, context);

	Vertex vertices2[] = {
		{ XMFLOAT3(+0.9f, +0.9f, +0.0f), red },
		{ XMFLOAT3(+0.9f, -0.5f, +0.0f), blue },
		{ XMFLOAT3(+0.8f, -0.5f, +0.0f), green },
		{ XMFLOAT3(+0.8f, +0.9f, +0.0f), green }
	};
	unsigned int indices2[] = { 0, 1, 3, 3, 1, 2 };
	square = std::make_shared<Mesh>(vertices2, 4, indices2, 6, device, context);

	Vertex vertices3[] = {
		{ XMFLOAT3(-0.9f, +0.5f, +0.0f), red },
		{ XMFLOAT3(-0.9f, -0.5f, +0.0f), blue },
		{ XMFLOAT3(-0.8f, -0.5f, +0.0f), green },
		{ XMFLOAT3(-0.8f, +0.5f, +0.0f), green },
		{ XMFLOAT3(-0.7f, +0.0f, +0.0f), blue }
	};
	unsigned int indices3[] = { 0, 2, 1, 3, 2, 0, 3, 4, 2 };
	pentagon = std::make_shared<Mesh>(vertices3, 5, indices3, 9, device, context);

	this->blue = std::make_shared<Material>(blue, vertexShader, pixelShader);
	this->red = std::make_shared<Material>(red, vertexShader, pixelShader);
	this->green = std::make_shared<Material>(green, vertexShader, pixelShader);

	entities = std::vector<Entity*>();
	Entity* square1 = new Entity(square, this->red);
	entities.push_back(square1);

	Entity* square2 = new Entity(square, this->blue);
	square2->GetTransform()->MoveAbsolute(5, 0, 0);
	entities.push_back(square2);

	Entity* triangle1 = new Entity(triangle, this->green);
	entities.push_back(triangle1);

	Entity* triangle2 = new Entity(triangle, this->red);
	triangle2->GetTransform()->MoveAbsolute(5, 0, 0);
	entities.push_back(triangle2);

	Entity* pentagonEnt = new Entity(pentagon, this->blue);
	entities.push_back(pentagonEnt);
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();
	if(worldCam != nullptr) {
		worldCam->UpdateProjectionMatrix((float)this->width / this->height);
	}
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Example input checking: Quit if the escape key is pressed
	if (Input::GetInstance().KeyDown(VK_ESCAPE))
		Quit();

	worldCam->Update(deltaTime);

	for(Entity* entity : entities) {
		entity->GetTransform()->MoveAbsolute(0.1f * deltaTime, 0.0f, 0.0f);
		entity->GetTransform()->Rotate(0.0f, 0.0f, 0.1f * deltaTime);
	}
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV.Get(), color);
	context->ClearDepthStencilView(
		depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);


	// Ensure the pipeline knows how to interpret the data (numbers)
	// from the vertex buffer.  
	// - If all of your 3D models use the exact same vertex layout,
	//    this could simply be done once in Init()
	// - However, this isn't always the case (but might be for this course)
	//context->IASetInputLayout(inputLayout.Get());

	for(Entity* entity : entities) {
		entity->Draw(context, worldCam);
	}

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(vsync ? 1 : 0, 0);

	// Due to the usage of a more sophisticated swap chain,
	// the render target must be re-bound after every call to Present()
	context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthStencilView.Get());
}