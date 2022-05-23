#include "Application.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <iostream>

#include "Shader.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"
#include "Framebuffer.hpp"

int Application::OnStart()
{
	glEnable(GL_CULL_FACE); glEnable(GL_DEPTH_TEST);
	std::unique_ptr<Framebuffer> RenderTarget( Framebuffer::FBOMultisample(0, 0 , m_Width, m_Height, 8) );
	std::unique_ptr<Framebuffer> RenderTarget2( Framebuffer::FBOIntermediate(1, m_Width, m_Height) );

	Renderer::Instance.InitPostprocess("Res/Shaders/PostprocessVS.glsl", "Res/Shaders/PostprocessFS.glsl");
	Renderer::Instance.Postprocess()->GetShader()->Uniform1i("Scene", RenderTarget2->GetColor()->Properties().Slot);

	glm::vec3 CamPosition(-6.0f, 2.0f, 6.0f);
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)m_Width/m_Height, 0.001f, 100.0f);
	glm::mat4 View = glm::lookAt(CamPosition, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


	Model Suzanne("Res/Models/Suzanne/Suzanne.obj");
	Suzanne.Transform() = glm::translate(Suzanne.Transform(), glm::vec3(0.0f, 0.0f, 0.0f));

	Model Walls("Res/Models/Walls/Walls.obj");
	Walls.Transform() = glm::translate(Walls.Transform(), glm::vec3(0.0f));
	Walls.Transform() = glm::scale(Walls.Transform(), glm::vec3(0.5f));

	std::unique_ptr<Shader> FooShader (Shader::CreateVF( "Res/Shaders/DefaultVS.glsl", "Res/Shaders/DefaultFS.glsl" ) );
	FooShader->UniformMatrix4fv("Projection", Projection);
	FooShader->UniformMatrix4fv("View", View);

	glm::vec3 DirectionalLight (-1.0f, 1.0f, 0.5f);
	glm::vec3 SuzanneColor (0.5f, 0.0f, 1.0f);
	glm::vec3 WallsColor (1.0f);

	FooShader->Uniform3fv("DirectionalLight", DirectionalLight);
	FooShader->Uniform3fv("CamPosition", CamPosition);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	while (!glfwWindowShouldClose(m_Window))
	{
		Renderer::Instance.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		

		RenderTarget->Bind(Framebuffer::READ_WRITE);

		Renderer::Instance.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		FooShader->UniformMatrix4fv("Model", Suzanne.Transform());
		FooShader->Uniform3fv("Color", SuzanneColor);
		Renderer::Instance.Draw(GL_TRIANGLES, Suzanne, FooShader.get());
		
		FooShader->UniformMatrix4fv("Model", Walls.Transform());
		FooShader->Uniform3fv("Color", WallsColor);
		Renderer::Instance.Draw(GL_TRIANGLES, Walls, FooShader.get());


		RenderTarget->Unbind();

		Framebuffer::Blit(RenderTarget.get(), RenderTarget2.get());

		Renderer::Instance.Present();
		Renderer::Instance.EndFrame(m_Window);
	}

	return 0;
}

Application::Application()
	:m_Title("wish i had something clever to put here :("), m_Width(1600), m_Height(900)
{
	// GLFW
	if (!glfwInit())
		std::cout << "GLFW initialization failed!" << std::endl;


	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	glfwWindowHint(GLFW_RESIZABLE, 0);


	// WINDOW CREATION
	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
	if (!m_Window)
		std::cout << "Window creation failed!" << std::endl;

	// GLAD
	glfwMakeContextCurrent(m_Window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cout << "GLAD initialization failed!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl
		<< glGetString(GL_RENDERER) << std::endl;
}

Application::~Application()
{
	glfwTerminate();
}
