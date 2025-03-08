#pragma once

namespace EngineV
{
	class Application; 

	class Window
	{
	public:
		Window();
		~Window();

		void InitWindow(int width, int height, Application* app);
		void Terminate();
		GLFWwindow* GetWindowPointer() const { return mWindow; }
		GLFWwindow* GetWindowPointer() { return mWindow; }
		int GetWindowHeight() const;
		int GetWindowWidth() const;
		bool GetWindowClosed();
		void PollEvents();

	private:
		GLFWwindow* mWindow = nullptr;
	};
}

