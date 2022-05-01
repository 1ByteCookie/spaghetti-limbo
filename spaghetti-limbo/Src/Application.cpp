#include "Application.hpp"
#include <iostream>

Application Application::Instance;

int Application::OnStart()
{
	float VertexData[] =
	{
		 0.0f,   1.0f,
		-1.0f,  -1.0f,
		 1.0f,  -1.0f
	};

	uint32_t IndexData[] =
	{
		0, 1, 2
	};


	GLuint VAO, VBO, IBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);


	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndexData), IndexData, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glClearColor(0.5f, 0.0f, 1.0f, 1.0f);
	while (!glfwWindowShouldClose(m_Window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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
