#include <EngineV.h>

class Sandbox : public EngineV::Application
{
public: 
	Sandbox()
	{
		mWindow.InitWindow(800, 600, this);
	}

	~Sandbox()
	{

	}

	void Run() override
	{
		while (!mWindow.GetWindowClosed())
		{
			mWindow.PollEvents();
		}
	}

private:
	EngineV::Window mWindow;
};

EngineV::Application* EngineV::CreateApplication()
{
	return new Sandbox();
}