#include "Model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

Model::Model(const std::string& Path)
	:	m_Transform( glm::mat4(1.0f) )
{
	Load(Path);
}


void Model::Load(const std::string& Path)
{
	Assimp::Importer Importer;
	const aiScene* Scene = Importer.ReadFile(Path, aiProcess_Triangulate | aiProcess_FlipUVs);

	TraverseNode(Scene->mRootNode, Scene);
}


void Model::TraverseNode(const aiNode* Node, const aiScene* Scene)
{
	for (uint32_t i = 0; i < Node->mNumMeshes; i++)
	{
		m_Meshes.push_back( LoadMesh( Scene->mMeshes[ Node->mMeshes[i] ], Scene ) );
	}

	for (uint32_t i = 0; i < Node->mNumChildren; i++)
	{
		TraverseNode(Node->mChildren[i], Scene);
	}
}


Mesh Model::LoadMesh(const aiMesh* aiMeshObject, const aiScene* Scene)
{
	std::vector<Vertex>		Vertices;
	std::vector<uint32_t>	Indices;

	Vertex		ExtractedVertex;
	glm::vec3	ExtractedVector;
	glm::vec2	ExtractedUV;

	for (uint32_t i = 0; i < aiMeshObject->mNumVertices; i++)
	{
		// vertex positions
		ExtractedVector.x = aiMeshObject->mVertices[i].x;
		ExtractedVector.y = aiMeshObject->mVertices[i].y;
		ExtractedVector.z = aiMeshObject->mVertices[i].z;
		ExtractedVertex.Position = ExtractedVector;

		// normals
		ExtractedVector.x = aiMeshObject->mNormals[i].x;
		ExtractedVector.y = aiMeshObject->mNormals[i].y;
		ExtractedVector.z = aiMeshObject->mNormals[i].z;
		ExtractedVertex.Normal = ExtractedVector;

		// uv
		if (aiMeshObject->mTextureCoords[0])
		{
			ExtractedUV.x = aiMeshObject->mTextureCoords[0][i].x;
			ExtractedUV.y = aiMeshObject->mTextureCoords[0][i].y;
			ExtractedVertex.UV = ExtractedUV;
		}
		else
		{
			ExtractedVertex.UV = glm::vec2(0.0f);
		}


		Vertices.push_back(ExtractedVertex);
	}

	// indices
	for (uint32_t i = 0; i < aiMeshObject->mNumFaces; i++)
	{
		aiFace Face = aiMeshObject->mFaces[i];
		for (uint32_t j = 0; j < Face.mNumIndices; j++)
		{
			Indices.push_back(Face.mIndices[j]);
		}
	}

	return Mesh(Vertices, Indices);
}
