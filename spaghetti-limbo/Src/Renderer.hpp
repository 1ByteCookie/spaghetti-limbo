#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "Shader.hpp"
#include "Postprocess.hpp"
#include "Model.hpp"

class Renderer
{
public:
	// singleton design, used to access member methods. e.g Renderer::Instance.Clear();
	static Renderer Instance;

	void Clear(unsigned int Mask);
	void EndFrame(GLFWwindow* Window);
	
	void InitPostprocess(const std::string& VS, const std::string& FS);
	void Present();

	void Draw(GLuint Primitive, GLuint VAO, GLuint IBO, const Shader* ShaderProgram, uint32_t Indices);
	void Draw(GLuint Primitve, Model& Model, const Shader* ShaderProgram);

	inline float GetDeltaTime() { return m_DeltaTime; }

	inline const auto& Postprocess() const { return m_Postprocess; }

private:
	Renderer();
	~Renderer() = default;

	std::unique_ptr<PostProcess> m_Postprocess;

	float			m_DeltaTime;
	float			m_CurrentFrame;
	float			m_PreviousFrame;
};