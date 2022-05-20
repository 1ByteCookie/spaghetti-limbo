#include "Renderer.hpp"

Renderer Renderer::Instance;

void Renderer::InitPostprocess(const std::string& VS, const std::string& FS)
{
	m_Postprocess = std::make_unique<PostProcess>(VS, FS);
}

void Renderer::Present()
{
	m_Postprocess->Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

Renderer::Renderer()
	:	m_DeltaTime(0)
	,	m_CurrentFrame(0)
	,	m_PreviousFrame(0) { }

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

void Renderer::Draw(GLuint Primitve, Model& Model, const Shader* ShaderProgram)
{
	ShaderProgram->Bind();
	for (uint32_t i = 0; i < Model.GetMeshes().size(); i++)
	{
		Model.GetMeshes()[i].Bind();
		glDrawElements(Primitve, Model.GetMeshes()[i].GetIndices(), GL_UNSIGNED_INT, nullptr);
	}
}