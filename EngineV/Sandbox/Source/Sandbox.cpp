#include <EngineV.h>

class Sandbox : public EngineV::Application
{
public: 
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

EngineV::Application* EngineV::CreateApplication()
{
	return new Sandbox();
}