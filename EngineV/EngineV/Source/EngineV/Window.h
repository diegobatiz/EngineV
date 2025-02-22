#pragma once
#include "Core.h"

struct GLFWwindow;

namespace EngineV
{
	class Application; 

	class ENGINEV_API Window
	{
	public:
		Window();
		~Window();

		void InitWindow(int width, int height, Application* app);
		void Terminate();
		const GLFWwindow* GetWindowPointer() const { return mWindow; }
		int GetWindowHeight();
		int GetWindowWidth();
		bool GetWindowClosed();
		void PollEvents();

	private:
		GLFWwindow* mWindow = nullptr;
	};
}

