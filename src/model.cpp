#include "model.hpp"

#include <stack>
#include <iostream>
#include "model.hpp"
#include <iostream>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/anim.h>
#include <glm/gtx/transform.hpp>
#include "logger.hpp"
#include "shader.hpp"

Mesh::Mesh(aiMesh * mesh)
{
	name = mesh->mName.C_Str();

	material_index = mesh->mMaterialIndex;

	// Vertices
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		//aiVector3D vec = mesh->mVertices[i];
		//std::cout << vec.x << " " << vec.y << " " << vec.z << "\n";
		aiVector3D normal;
		if (mesh->HasNormals())
		{
			normal = mesh->mNormals[i];
		}
		aiVector2D tex_coords;
		if (mesh->HasTextureCoords(0))
		{
			tex_coords.x = mesh->mTextureCoords[0][i].x;
			tex_coords.y = mesh->mTextureCoords[0][i].y;
		}
		vertices.emplace_back(mesh->mVertices[i],
			normal,
			tex_coords);
	}

	// Indices
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < 3; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	for (int i = 0; i < mesh->mNumBones; i++)
	{
		//mesh->mBones[i]->
	}
}

Mesh::Mesh(const std::vector<Vertex>& _vertices, const std::vector<GLuint>& _indices)
{
	vertices = _vertices;
	indices = _indices;
}

void Mesh::uploadToGPU()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);


	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &vertices[0], GL_STATIC_DRAW);


	unsigned int stride = 2 * sizeof(glm::vec3) + sizeof(glm::vec2);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(glm::vec3)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(2 * sizeof(glm::vec3)));


	glBindVertexArray(0);
}

void Mesh::removeFromGPU()
{
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void Mesh::bind()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void Mesh::unbind()
{
	glBindVertexArray(0);
}

Model::Model(const std::string & file)
{
	Log::debugln("Loading model '" + file + "'");

	const aiScene* scene = aiImportFile(file.c_str(),
		//aiProcess_GenNormals |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_OptimizeMeshes | 
		aiProcess_OptimizeGraph |
		aiProcess_SortByPType
	);
	if (!scene)
	{
		Log::errorln("Could not open model '" + file + "' - " + std::string(aiGetErrorString()));
		return;
	}
	if (!scene->HasMeshes())
	{
		Log::errorln("No meshes in: '" + file + "'");
		return;
	}


	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		meshes.emplace_back(scene->mMeshes[i]);
		meshes.back().uploadToGPU();
	}

	
	/*
	if (scene->HasAnimations())
	{
		for (int i = 0; i < scene->mNumAnimations; i++)
		{
			auto& anim = scene->mAnimations[i];
			//anim->mName.C_Str();

			//anim->m
		}
	}
	*/

	if (scene->HasMaterials())
	{
		std::string dir;
		size_t last = file.find_last_of('/');
		if (last >= file.size())
			dir = "";
		else
			dir = file.substr(0, last+1);

		auto mats = scene->mMaterials;
		for (int i = 0; i < scene->mNumMaterials; i++)
		{
			materials.emplace_back(mats[i], dir);
		}	
	}

	std::stack<aiNode*> aistack;
	std::stack<Node*> stack;
	aiNode* aicurrent = scene->mRootNode;
	Node* current = &root;
	bool done = false;
	while (!done)
	{
		// Process node
		// ---------------------------
		
		for (int i = 0; i < aicurrent->mNumMeshes; i++)
		{
			size_t index = aicurrent->mMeshes[i];
			current->meshes.push_back(&meshes[i]);
		}
		current->name = std::string(aicurrent->mName.C_Str());

		for (int i = 0; i < aicurrent->mNumChildren; i++)  
		{
			current->children.push_back(new Node());

			// ---------------------------


			aistack.push(aicurrent->mChildren[i]);
			stack.push(current->children[i]);
		}


		if (aistack.empty())
		{
			done = true;
		}
		else
		{
			aicurrent = aistack.top();
			aistack.pop();

			current = stack.top();
			stack.pop();
		}
	}

	aiReleaseImport(scene);
}

void Model::recursiveDraw(Node* node, ShaderProgram & shader, glm::mat4 transform)
{
	glm::mat4 current_transform = transform*node->transform;

	shader.uniform("model", current_transform);

	for (int i = 0; i < node->meshes.size(); i++)
	{
		node->meshes[i]->bind();
		glDrawElements(GL_TRIANGLES, node->meshes[i]->numIndices(), GL_UNSIGNED_INT, (void*)0);

	}
	for (int i = 0; i < node->children.size(); i++)
	{
		recursiveDraw(node->children[i], shader, current_transform);
	}
}

float angle = 0;

void Model::debug_draw(ShaderProgram & shader)
{
	angle += 0.01;

	glm::mat4 rotation = glm::rotate(angle, glm::vec3(0, 1, 0));

	recursiveDraw(&root, shader, rotation);
}


