#pragma once
#include "Core.h"

namespace EngineV
{
	class ENGINEV_API Window
	{
	public:
		Window();
		~Window();

		void InitWindow(int width, int height);

	private:
		GLFWwindow* mWindow = nullptr;
	};
}

