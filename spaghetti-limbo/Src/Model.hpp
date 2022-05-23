#pragma once

#include "Mesh.hpp"
#include <assimp/scene.h>

#include <string>


class Model
{
public:

	Model(const std::string& Path);
	~Model() = default;

	const auto& GetMeshes() const { return m_Meshes; }

	inline glm::mat4& Transform() { return m_Transform; }


private:

	std::vector<Mesh>	m_Meshes;
	glm::mat4			m_Transform;

	void Load(const std::string& Path);
	void TraverseNode(const aiNode* Node, const aiScene* Scene);
	Mesh LoadMesh(const aiMesh* aiMeshObject, const aiScene* Scene);
};