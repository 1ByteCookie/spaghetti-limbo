#include "Renderer.hpp"

Renderer Renderer::Instance;

Renderer::Renderer()
	:m_DeltaTime(0), m_CurrentFrame(0), m_PreviousFrame(0)
{

}

void Renderer::Clear(unsigned int Mask)
{
	glClear(Mask);
}

void Renderer::EndFrame(GLFWwindow* Window)
{
	m_CurrentFrame = glfwGetTime();
	m_DeltaTime = m_CurrentFrame - m_PreviousFrame;
	m_PreviousFrame = m_CurrentFrame;

	glfwSwapBuffers(Window);
}

void Renderer::Draw(GLuint Primitive, GLuint VAO, GLuint IBO, const Shader* ShaderProgram, uint32_t Indices)
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	ShaderProgram->Bind();

	glDrawElements(Primitive, Indices, GL_UNSIGNED_INT, nullptr);
}
