#pragma once
#include "Core.h"

namespace EngineV
{
	class ENGINEV_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// Defined in client
	Application* CreateApplication();
}
