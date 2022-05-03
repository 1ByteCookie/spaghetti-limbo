#include "PostProcess.hpp"

PostProcess::PostProcess(const std::string& vPath, const std::string& fPath)
	:m_Shader( Shader::CreateVF(vPath, fPath) )
{
	float VertexBuffer[] =
	{
		-1.0f,   1.0f,		0.0f, 1.0f,
		-1.0f,  -1.0f,		0.0f, 0.0f,
		 1.0f,  -1.0f,		1.0f, 0.0f,
		 1.0f,   1.0f,		1.0f, 1.0f
	};

	uint32_t IndexBuffer[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBuffer), VertexBuffer, GL_STATIC_DRAW);

	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndexBuffer), IndexBuffer, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	Unbind();
}

PostProcess::~PostProcess()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);
}

void PostProcess::Bind() const
{
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	m_Shader->Bind();
}

void PostProcess::Unbind() const
{
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
