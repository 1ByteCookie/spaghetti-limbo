#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Application
{
public:
	static Application Instance;

	int OnStart();


private:
	Application();
	~Application();


	GLFWwindow* m_Window;
	std::string		m_Title;
	int				m_Width;
	int				m_Height;
};
