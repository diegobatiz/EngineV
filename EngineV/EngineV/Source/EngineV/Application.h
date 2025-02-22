#pragma once
#include "Core.h"

namespace EngineV
{
	class ENGINEV_API Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void Run();
	
		bool mFrameBufferResized = false;
	};

	// Defined in client
	Application* CreateApplication();
}
