#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Application
{
public:
	int OnStart();

	Application();
	~Application();


private:

	GLFWwindow* m_Window;
	std::string		m_Title;
	int				m_Width;
	int				m_Height;
};
