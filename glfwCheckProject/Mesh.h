#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

enum TextureType
{

};

struct Texture
{
	GLuint id;
	TextureType type;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> texture);
	//void Draw(Shader shader);

private:
	GLuint VAO, VBO, EBO;
	void SetupMesh();
};