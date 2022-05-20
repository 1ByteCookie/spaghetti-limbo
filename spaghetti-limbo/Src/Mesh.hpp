#pragma once

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3	Position;
	glm::vec3	Normal;
	glm::vec2	UV;
};

class Mesh
{
public:
	
	Mesh(const std::vector<Vertex>& VertexData, const std::vector<uint32_t>& IndexData);
	Mesh(Mesh&& Object) noexcept;
	~Mesh();

	void Bind() const;
	void Unbind() const;

	inline uint32_t GetIndices() const { return m_Indices; }

private:
	
	GLuint		m_VAO;
	GLuint		m_VBO;
	GLuint		m_IB0;

	uint32_t	m_Indices;
};