#pragma once

#include "shader.hpp"
#include "vertex.hpp"
#include "texture.hpp"
#include "material.hpp"
#include <vector>
#include <string>
#include <GL/glew.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>

class Mesh
{
	std::string name = "";

	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	GLuint ebo = 0;
	GLuint vbo = 0;
	GLuint vao = 0;
	
	unsigned int material_index;

public:
	std::vector<Vertex> vertices;

	Mesh(aiMesh* mesh);
	Mesh(const std::vector<Vertex>& vertices,
		const std::vector<GLuint>& indices);

	unsigned int numIndices()
	{
		return indices.size();
	}

	unsigned int getMaterialIndex()
	{
		return material_index;
	}

	void uploadToGPU();
	void removeFromGPU();

	void bind();
	void unbind();
};

class Model
{
	struct Node
	{
		// meshes taken from the vector<Mesh> in Model
		std::vector<Mesh*> meshes;

		std::vector<Node*> children;

		std::string name;

		// relative to parent
		glm::mat4 transform;
	} root;

	std::vector<Mesh> meshes;

	std::vector<Material> materials;

	

	// --- DEBUG ---
	void recursiveDraw(Node* node, ShaderProgram & shader, glm::mat4 transform = glm::mat4());
	// -------------
public:
	Model(const std::string& filepath);



	// --- DEBUG ---
	void debug_draw(ShaderProgram & shader);
	// -------------
};