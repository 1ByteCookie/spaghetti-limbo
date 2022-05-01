#include "Application.hpp"
#include <iostream>

Application Application::Instance;

int Application::OnStart()
{
	glClearColor(0.5f, 0.0f, 1.0f, 1.0f);
	while (!glfwWindowShouldClose(m_Window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();

		glfwSwapBuffers(m_Window);
	}

	return 0;
}

Application::Application()
	:m_Title("wish i had something clever to put here :("), m_Width(1600), m_Height(900)
{
	// GLFW
	if (!glfwInit())
		std::cout << "GLFW initialization failed!" << std::endl;

	// WINDOW CREATION
	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
	if (!m_Window)
		std::cout << "Window creation failed!" << std::endl;

	// GLAD
	glfwMakeContextCurrent(m_Window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cout << "GLAD initialization failed!" << std::endl;
}

Application::~Application()
{
	glfwTerminate();
}
