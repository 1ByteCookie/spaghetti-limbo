#include "Mesh.hpp"

Mesh::Mesh(const std::vector<Vertex>& VertexData, const std::vector<uint32_t>& IndexData)
	:	m_VAO(0)
	,	m_VBO(0)
	,	m_IB0(0)
	,	m_Indices(IndexData.size())
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, VertexData.size() * sizeof(Vertex), VertexData.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_IB0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IB0);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexData.size() * sizeof(uint32_t), IndexData.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::Position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::Normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::UV));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

Mesh::Mesh(Mesh&& Object) noexcept
{
	m_VAO		= Object.m_VAO;
	m_VBO		= Object.m_VBO;
	m_IB0		= Object.m_IB0;
	m_Indices	= Object.m_Indices;

	Object.m_VAO		= 0;
	Object.m_VBO		= 0;
	Object.m_IB0		= 0;
	Object.m_Indices	= 0;

}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IB0);
}

void Mesh::Bind() const
{
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IB0);
}

void Mesh::Unbind() const
{
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
