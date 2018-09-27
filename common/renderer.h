#pragma once

#include <iostream>

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/camera.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/tangentspace.hpp>
#include <common/config.h>
#include <common/text2D.hpp>

//singleton renderer
class Renderer {
public:
	// getInstance gets private instance of the renderer
	static Renderer* getInstance();

	//initializes the renderer
	int init();

	//updates the renderer
	void update();

	//gets glfw window pointer
	GLFWwindow* getWindow() { return _window; }

	//destroys current window
	void destroyWindow();

private:
	//the instance of renderer
	static Renderer* instance;

	GLFWwindow* _window;

	//default constructor
	Renderer();
	//deconstructor
	virtual ~Renderer();

	//resolution taken from config.h
	int resX = RESOLUTION_X;
	int resY = RESOLUTION_Y;

	//deltatime and fps counters
	float deltaTime;
	int FPS = 0;
	int HFPS = 0;
	double lastTime;
	double lastTimeFPS;

	//default vertexArrayID
	GLuint VertexArrayID;
	//programID
	GLuint programID;

	GLuint MatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;
	GLuint ModelView3x3MatrixID;

	GLuint LightID;

	GLuint DiffuseTexture;
	GLuint NormalTexture;
	GLuint SpecularTexture;

	GLuint DiffuseTextureID;
	GLuint NormalTextureID;
	GLuint SpecularTextureID;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	//buffers
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint tangentbuffer;
	GLuint bitangentbuffer;
	GLuint elementbuffer;

	//indexed vertices, uv's, normals, tangents, and bitangents
	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	std::vector<glm::vec3> indexed_tangents;
	std::vector<glm::vec3> indexed_bitangents;
};
