#include "Application.hpp"
#include <memory>
#include <iostream>
#include "Shader.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"
#include "Framebuffer.hpp"

Application Application::Instance;

int Application::OnStart()
{
	glEnable(GL_CULL_FACE);
	std::unique_ptr<Framebuffer> RenderTarget( Framebuffer::FBOMultisample(0, 0 , m_Width, m_Height, 8) );
	std::unique_ptr<Framebuffer> RenderTarget2( Framebuffer::FBOIntermediate(1, m_Width, m_Height) );

	Renderer::Instance.InitPostprocess("Res/Shaders/PostprocessVS.glsl", "Res/Shaders/PostprocessFS.glsl");
	Renderer::Instance.Postprocess()->GetShader()->Uniform1i("Scene", RenderTarget2->GetColor()->Properties().Slot);


	float VertexData[] =
	{
		-1.0f,   1.0f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		-1.0f,  -1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		 1.0f,  -1.0f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
		 1.0f,   1.0f,		0.0f, 1.0f, 0.3f,	1.0f, 1.0f
	};

	uint32_t IndexData[] =
	{
		0, 1, 2,
		0, 2, 3
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


	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)) );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)) );
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);


	TEXTURE_DESC tDescriptor = { };
	tDescriptor.Slot			= 3;
	tDescriptor.Target			= GL_TEXTURE_2D;
	tDescriptor.InternalFormat	= GL_SRGB;
	tDescriptor.Format			= GL_RGB;
	tDescriptor.BufferType		= GL_UNSIGNED_BYTE;
	tDescriptor.Path			= "Res/Texture2D/fromReddit.jpg";
	std::unique_ptr<Texture> Image ( Texture::LoadFromFile(tDescriptor, true) );
	Image->Bind();

	std::unique_ptr<Shader> FooShader (Shader::CreateVF( "Res/Shaders/DefaultVS.glsl", "Res/Shaders/DefaultFS.glsl" ) );
	FooShader->Uniform1i("Diffuse", Image->Properties().Slot);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	while (!glfwWindowShouldClose(m_Window))
	{
		Renderer::Instance.Clear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
		

		RenderTarget->Bind(Framebuffer::READ_WRITE);

		Renderer::Instance.Clear(GL_COLOR_BUFFER_BIT);
		Renderer::Instance.Draw(GL_TRIANGLES, VAO, IBO, FooShader.get(), 6);
		
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
