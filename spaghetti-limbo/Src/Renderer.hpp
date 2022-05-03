#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"

class Renderer
{
public:
	// singleton design, used to access member methods. e.g Renderer::Instance.Clear();
	static Renderer Instance;

	void Clear(unsigned int Mask);
	void EndFrame(GLFWwindow* Window);

	void Draw(GLuint Primitive, GLuint VAO, GLuint IBO, const Shader* ShaderProgram, uint32_t Indices);

	inline float GetDeltaTime() { return m_DeltaTime; }

private:
	Renderer();
	~Renderer() = default;

	float			m_DeltaTime;
	float			m_CurrentFrame;
	float			m_PreviousFrame;
};