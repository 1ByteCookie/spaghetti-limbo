#include "Application.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <iostream>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Shader.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"
#include "Framebuffer.hpp"

int Application::OnStart()
{
	glEnable(GL_CULL_FACE); glEnable(GL_DEPTH_TEST);
	std::unique_ptr<Framebuffer> RenderTarget( Framebuffer::FBOMultisample(0, 0 , m_Width, m_Height, 8) );
	std::unique_ptr<Framebuffer> RenderTarget2( Framebuffer::FBOIntermediate(1, m_Width, m_Height) );
	std::unique_ptr<Framebuffer> ShadowMap( Framebuffer::DepthMap(2, 1024, 1024) );


	Renderer::Instance.InitPostprocess("Res/Shaders/PostprocessVS.glsl", "Res/Shaders/PostprocessFS.glsl");
	Renderer::Instance.Postprocess()->GetShader()->Uniform1i("Scene", RenderTarget2->GetColor()->Properties().Slot);


	glm::vec3 CamPosition(-6.0f, 2.0f, 6.0f);
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)m_Width/m_Height, 0.001f, 100.0f);
	glm::mat4 View = glm::lookAt(CamPosition,
								 glm::vec3(0.0f),
								 glm::vec3(0.0f, 1.0f, 0.0f));


	glm::vec3 PointLight(-3.0f, 4.0f, 3.0f);



	glm::mat4 LightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.01f, 10.0f);
	glm::mat4 LightPerspective = glm::lookAt(PointLight,
											 glm::vec3(0.0f),
											 glm::vec3(0.0f, 1.0f, 0.0f));

	std::unique_ptr<Shader> WriteToShadowMap( Shader::CreateVF("Res/Shaders/ShadowMapVS.glsl", "Res/Shaders/ShadowMapFS.glsl") );
	{
		WriteToShadowMap->UniformMatrix4fv("LightSpace", LightProjection * LightPerspective);
	}

	Model Suzanne("Res/Models/Suzanne/Suzanne.obj");
	glm::vec3 SuzanneColor = glm::vec3(0.75f, 0.0f, 1.0f);
	glm::vec3 SuzannePosition = glm::vec3(0.0f);
	glm::vec3 SuzanneRotation = glm::vec3(0.0f);
	Suzanne.Transform() = glm::translate(Suzanne.Transform(), SuzannePosition);
	std::unique_ptr<Shader> SuzanneShader (Shader::CreateVF( "Res/Shaders/DefaultVS.glsl", "Res/Shaders/DefaultFS.glsl" ) );
	{
		SuzanneShader->UniformMatrix4fv("Projection", Projection);
		SuzanneShader->UniformMatrix4fv("View", View);
		SuzanneShader->UniformMatrix4fv("Model", Suzanne.Transform());
		SuzanneShader->UniformMatrix4fv("LightSpace", LightProjection * LightPerspective);
		SuzanneShader->Uniform3fv("CamPosition", CamPosition);
	
		SuzanneShader->Uniform3fv("Material.Diffuse", SuzanneColor);
		SuzanneShader->Uniform3fv("Material.Specular", glm::vec3(0.5f));
		SuzanneShader->Uniform1f("Material.Luster", 64.0f);
	
		SuzanneShader->Uniform3fv("Light.Position", PointLight);
		SuzanneShader->Uniform3fv("Light.Ambient", glm::vec3(1.0f));
		SuzanneShader->Uniform3fv("Light.Diffuse", glm::vec3(1.0f));
		SuzanneShader->Uniform3fv("Light.Specular", glm::vec3(1.0f));

		SuzanneShader->Uniform1i("shadowMap", ShadowMap->GetDepth()->Properties().Slot);
	}



	Model Walls("Res/Models/Walls/Walls.obj");
	Walls.Transform() = glm::translate(Walls.Transform(), glm::vec3(0.0f));
	Walls.Transform() = glm::scale(Walls.Transform(), glm::vec3(0.5f));
	std::unique_ptr<Shader> WallsShader(Shader::CreateVF("Res/Shaders/DefaultVS.glsl", "Res/Shaders/DefaultFS.glsl"));
	{
		WallsShader->UniformMatrix4fv("Projection", Projection);
		WallsShader->UniformMatrix4fv("View", View);
		WallsShader->UniformMatrix4fv("Model", Walls.Transform());
		WallsShader->UniformMatrix4fv("LightSpace", LightProjection * LightPerspective);
		WallsShader->Uniform3fv("CamPosition", CamPosition);

		WallsShader->Uniform3fv("Material.Diffuse", glm::vec3(1.0f));
		WallsShader->Uniform3fv("Material.Specular", glm::vec3(0.5f));
		WallsShader->Uniform1f("Material.Luster", 8.0f);

		WallsShader->Uniform3fv("Light.Position", PointLight);
		WallsShader->Uniform3fv("Light.Ambient", glm::vec3(0.1f));
		WallsShader->Uniform3fv("Light.Diffuse", glm::vec3(1.0f));
		WallsShader->Uniform3fv("Light.Specular", glm::vec3(1.0f));

		WallsShader->Uniform1i("shadowMap", ShadowMap->GetDepth()->Properties().Slot);
	}


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	while (!glfwWindowShouldClose(m_Window))
	{
		Renderer::Instance.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();

		Suzanne.Transform() = glm::translate(glm::mat4(1.0f), SuzannePosition);

		ShadowMap->Bind(Framebuffer::READ_WRITE);
		{
			glViewport( 0,
						0,
						ShadowMap->GetDepth()->Properties().Width,
						ShadowMap->GetDepth()->Properties().Height);
			Renderer::Instance.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glCullFace(GL_FRONT);

			WriteToShadowMap->UniformMatrix4fv("Model", Suzanne.Transform());
			Renderer::Instance.Draw(GL_TRIANGLES, Suzanne, WriteToShadowMap.get());
		
			WriteToShadowMap->UniformMatrix4fv("Model", Walls.Transform());
			Renderer::Instance.Draw(GL_TRIANGLES, Walls, WriteToShadowMap.get());
		}
		ShadowMap->Unbind();


		RenderTarget->Bind(Framebuffer::READ_WRITE);
		{
			glViewport(0, 0, m_Width, m_Height);
			Renderer::Instance.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glCullFace(GL_BACK);

			SuzanneShader->UniformMatrix4fv("Model", Suzanne.Transform());
			SuzanneShader->Uniform3fv("Light.Position", PointLight);
			WallsShader->Uniform3fv("Light.Position", PointLight);

			Renderer::Instance.Draw(GL_TRIANGLES, Suzanne, SuzanneShader.get());
			Renderer::Instance.Draw(GL_TRIANGLES, Walls, WallsShader.get());
		}
		RenderTarget->Unbind();

		Framebuffer::Blit(RenderTarget.get(), RenderTarget2.get());


		Renderer::Instance.Present();
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Transform");
			ImGui::DragFloat3("Position", &SuzannePosition.x, 0.01f, -100, 100);
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}



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

	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
		ImGui_ImplOpenGL3_Init("#version 460 core");
	}
}

Application::~Application()
{
	glfwTerminate();
}
