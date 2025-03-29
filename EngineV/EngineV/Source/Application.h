#pragma once

namespace EngineV
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void Run();
	
		virtual void ResizeWindow() {}
	};
}
