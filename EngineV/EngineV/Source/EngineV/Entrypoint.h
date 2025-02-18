#pragma once

#ifdef EV_PLATFORM_WINDOWS

extern EngineV::Application* EngineV::CreateApplication();

int main(int argc, char** argv)
{
	auto app = EngineV::CreateApplication();
	app->Run();
	delete app;
}

#endif