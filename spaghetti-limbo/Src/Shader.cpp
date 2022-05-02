#include "Shader.hpp"

#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

Shader::~Shader()
{
	glDeleteProgram(m_ID);
}

void Shader::Bind() const
{
	glUseProgram(m_ID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::Uniform1i(const std::string& Name, int v0)
{
	Bind(); glUniform1i(GetUniformLocation(Name), v0);
}

void Shader::Uniform1f(const std::string& Name, float v0)
{
	Bind(); glUniform1f(GetUniformLocation(Name), v0);
}

void Shader::Uniform3fv(const std::string& Name, glm::vec3& Vector)
{
	Bind(); glUniform3fv(GetUniformLocation(Name), 1, glm::value_ptr(Vector));
}

void Shader::UniformMatrix4fv(const std::string& Name, glm::mat4& Matrix)
{
	Bind(); glUniformMatrix4fv(GetUniformLocation(Name), 1, GL_FALSE, glm::value_ptr(Matrix));
}

Shader* Shader::CreateVF(const std::string& VS, const std::string& FS)
{
	Shader* This = new Shader();

	GLuint ShaderProgram	= glCreateProgram();
	GLuint VAttachment		= This->CompileFromFile(GL_VERTEX_SHADER, VS);
	GLuint FAttachment		= This->CompileFromFile(GL_FRAGMENT_SHADER, FS);

	glAttachShader(ShaderProgram, VAttachment);
	glAttachShader(ShaderProgram, FAttachment);

	glLinkProgram(ShaderProgram);
	glValidateProgram(ShaderProgram);
	This->m_ID = ShaderProgram;


	glDeleteShader(VAttachment);
	glDeleteShader(FAttachment);


	return This;
}

GLuint Shader::CompileFromFile(GLuint ShaderType, const std::string& Path)
{
	GLuint Shader				= glCreateShader(ShaderType);
	std::string ShaderSource	= ReadFile(Path);
	const char* Source			= ShaderSource.c_str();


	glShaderSource(Shader, 1, &Source, nullptr);
	glCompileShader(Shader);

	// Error check
	int Status; char MessageLog[1024];
	glGetShaderiv(Shader, GL_COMPILE_STATUS, &Status);
	if (!Status)
	{
		glGetShaderInfoLog(Shader, 1024, nullptr, MessageLog);
		std::cout << "Error::" << Path << std::endl
			<< ShaderSource << std::endl
			<< MessageLog << std::endl;
	}


	return Shader;
}


// dangerous stuff, refactor later ?....or not lmao
std::string Shader::ReadFile(const std::string& Path)
{
	std::ifstream FileStream(Path);
	std::string Content, ReadLine;
	

	if (FileStream.fail())
	{
		std::cout << Path << ": Error!" << std::endl;
		return Content;
	}


	while (std::getline(FileStream, ReadLine))
		Content += ReadLine + "\n";


	FileStream.close();
	return Content;
}

int Shader::GetUniformLocation(const std::string& Name)
{
	if (m_Uniforms.contains(Name))
		return m_Uniforms.at(Name);

	int Location = glGetUniformLocation(m_ID, Name.c_str());
	m_Uniforms.emplace(Name, Location);

	return Location;
}
