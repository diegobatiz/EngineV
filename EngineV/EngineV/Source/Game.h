#include "Application.h"
#include "Window.h"
#include "Renderer.h"

class Sandbox : public EngineV::Application
{
public:
	Sandbox()
	{
		mWindow.InitWindow(800, 600, this);
		mRenderer = new EngineV::Renderer("Sandbox", mWindow);
		mRenderer->Initialize();
	}

	~Sandbox()
	{
		mRenderer->Terminate();
		delete mRenderer;
		mWindow.Terminate();
	}

	void Run() override
	{
		while (!mWindow.GetWindowClosed())
		{
			mWindow.PollEvents();
			mRenderer->DrawFrame();
		}
	}

	void ResizeWindow() override
	{
		mRenderer->WindowResized(true);
	}

private:
	EngineV::Window mWindow;
	EngineV::Renderer* mRenderer;
};