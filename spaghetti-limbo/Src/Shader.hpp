#pragma once

#include <glad/glad.h>
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>

/*
	the implementation of this header file is a masterpiece
	example of spaghetti code that even the finest cook from italy
	could only dream of achieving
*/

class Shader
{
public:
	// creates a vertex shader and fragment shader
	static Shader* CreateVF(const std::string& VS, const std::string& FS);
	

	~Shader();
	const GLuint GetID() { return m_ID; }
	void Bind() const;
	void Unbind() const;


	void Uniform1i(const std::string& Name, int v0);

	void Uniform1f(const std::string& Name, float v0);

	void Uniform3fv(const  std::string& Name, glm::vec3& Vector);
	
	void UniformMatrix4fv(const std::string& Name, glm::mat4& Matrix);

private:
	Shader() :m_ID(0) { }

	GLuint CompileFromFile(GLuint ShaderType, const std::string& Path);

	std::string ReadFile(const std::string& Path);

	int GetUniformLocation(const std::string& Name);


	GLuint									m_ID;
	std::unordered_map<std::string, int>	m_Uniforms;
};