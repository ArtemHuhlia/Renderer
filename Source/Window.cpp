#include "Window.h"
//#include "imgui.h"
//#include "ImGui/ImGuiSDL.h"
//#include "Imgui/ImGuiOpenGL.h"



void Window::GetWindowWMInfo(HWND & OutputHandler) const
{
	CHECK(WindowHandler);
	SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(WindowHandler, &wmInfo);
	OutputHandler = (HWND)wmInfo.info.win.window;
}

static bool SDLProcessErrors(const char * FunctionName, const char* FileName, int Line)
{
	const char* ErrorMessage = SDL_GetError();
	if (*ErrorMessage)
	{
		SDL_Log("Error: %s [File: %s] [Line: %s] [Func: %s]", SDL_GetError(), FileName, Line, FunctionName);
		return false;
	}
	return true;
}

Window::Window(const char* Title, int Width, int Height, int XPos /*= SDL_WINDOWPOS_CENTERED*/, int YPos /*= SDL_WINDOWPOS_CENTERED*/) :
	bIsWindowCLosed(false)
{
	//SDL Initialization
	int Result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
	if (Result != 0)
	{
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
	}

	//Window creation
	WindowHandler = SDL_CreateWindow(Title, XPos, YPos, Width, Height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (!WindowHandler)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
	}

	WindowEventManager.ApplicationEvents.BindOnQuitEvent([this]() {
		bIsWindowCLosed = true;
	});

	WindowEventManager.KeybordEvents.BindOnKeyDownEvent(SDLK_ESCAPE,[this]() {
		bIsWindowCLosed = true;
	});

	// DX11 
	
	InitDxRenderer();

	////ImGui initialization
	//ImGui::CreateContext();
	//if (!ImGui_ImplSDL2_InitForOpenGL(WindowHandler, GLContex))
	//{
	//	SDL_Log("Failed to Initialise ImGui!");
	//}
	//ImGui_ImplOpenGL3_Init("#version 430");
	//ImGui::StyleColorsDark();
	//ImGui::GetIO();
}

void Window::InitDxRenderer()
{
	CHECK(WindowHandler);
	HWND Handler;
	GetWindowWMInfo(Handler);
	Renderer.InitRendererState(Handler);
}

bool Window::IsWindowClosed() const
{
	return bIsWindowCLosed;
}

void Window::Tick(float InDelta)
{
	Renderer.RenderFrame(InDelta);
}
