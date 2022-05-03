#pragma once

#include <glad/glad.h>
#include <memory>
#include "Shader.hpp"

// deffer the creation after glad is initialized!!

class PostProcess
{
public:
	PostProcess(const std::string& vPath, const std::string& fPath);
	~PostProcess();

	void Bind() const;
	void Unbind() const;

	const auto& GetShader() const { return m_Shader; }
	const auto& GetVAO() const { return m_VAO; }
	const auto& GetIBO() const { return m_IBO; }

private:
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_IBO;

	std::unique_ptr<Shader> m_Shader;
};