#include "renderer.h"
#include "vector2.h"

Renderer* Renderer::instance = NULL;

Renderer::Renderer() {
	_window = NULL;

	this->init();
}

int Renderer::init()
{
	// glfwGetTime is called only once, the first time this function is called
	lastTime = glfwGetTime();
	lastTimeFPS = glfwGetTime();

	//Initialise GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); //how many AA you use
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a _window and create its OpenGL context
	_window = glfwCreateWindow(resX, resY, "MXP3D", NULL, NULL);
	if (_window == NULL) {
		fprintf(stderr, "Failed to open GLFW _window. If you have an Intel GPU, they are not 3.3 compatible.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(_window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(_window, resX / 2, resY / 2);

	// Dark red background
	glClearColor(0.4f, 0.0f, 0.0f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	//glDisable(GL_CULL_FACE);

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("shaders/normalMappingShader.vert", "shaders/normalMappingShader.frag");

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");
	ViewMatrixID = glGetUniformLocation(programID, "V");
	ModelMatrixID = glGetUniformLocation(programID, "M");
	ModelView3x3MatrixID = glGetUniformLocation(programID, "MV3x3");

	// Load the texture
	DiffuseTexture = loadDDS("textures/diffuse.DDS");
	NormalTexture = loadBMP_custom("textures/normal.bmp");
	SpecularTexture = loadDDS("textures/specular.DDS");

	// Get a handle for our "myTextureSampler" uniform
	DiffuseTextureID = glGetUniformLocation(programID, "DiffuseTextureSampler");
	NormalTextureID = glGetUniformLocation(programID, "NormalTextureSampler");
	SpecularTextureID = glGetUniformLocation(programID, "SpecularTextureSampler");

	// Read our .obj file
	bool res = loadOBJ("../common/models/cylinder.obj", vertices, uvs, normals);

	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;
	computeTangentBasis(
		vertices, uvs, normals, // input
		tangents, bitangents    // output
	);

	indexVBO_TBN(
		vertices, uvs, normals, tangents, bitangents,
		indices, indexed_vertices, indexed_uvs, indexed_normals, indexed_tangents, indexed_bitangents
	);

	// Load it into a VBO

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &tangentbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_tangents.size() * sizeof(glm::vec3), &indexed_tangents[0], GL_STATIC_DRAW);

	glGenBuffers(1, &bitangentbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_bitangents.size() * sizeof(glm::vec3), &indexed_bitangents[0], GL_STATIC_DRAW);

	// Generate a buffer for the indices as well
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

	//initialize text2D
	initText2D("../common/fonts/font.DDS");

	return 0;
}

Renderer::~Renderer() {
	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &tangentbuffer);
	glDeleteBuffers(1, &bitangentbuffer);
	glDeleteBuffers(1, &elementbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &DiffuseTexture);
	glDeleteTextures(1, &NormalTexture);
	glDeleteTextures(1, &SpecularTexture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL _window and terminate GLFW
	glfwTerminate();
}

void Renderer::destroyWindow()
{
	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &tangentbuffer);
	glDeleteBuffers(1, &bitangentbuffer);
	glDeleteBuffers(1, &elementbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &DiffuseTexture);
	glDeleteTextures(1, &NormalTexture);
	glDeleteTextures(1, &SpecularTexture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL _window and terminate GLFW
	glfwTerminate();
}

void Renderer::update() {
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Measure speed
	double currentTime = glfwGetTime();
	deltaTime = float(currentTime - lastTime);

	if (SHOW_FPS) {
		// Measure FPS and show FPS on screen
		FPS++;
		if (currentTime - lastTimeFPS >= 1.0) { // If last prinf() was more than 1 sec ago
			HFPS = FPS;
			FPS = 0;
			lastTimeFPS += 1.0;
		}
		std::string str = std::to_string(HFPS);
		const char * c = str.c_str();
		printText2D(c, 750, 550, 25);
	}

	// Use our shader
	glUseProgram(programID);

	// Compute the MVP matrix from keyboard and mouse input
	computeMatricesFromInputs(deltaTime);
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	glm::mat4 ModelViewMatrix = ViewMatrix * ModelMatrix;
	glm::mat3 ModelView3x3Matrix = glm::mat3(ModelViewMatrix);
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix3fv(ModelView3x3MatrixID, 1, GL_FALSE, &ModelView3x3Matrix[0][0]);


	glm::vec3 lightPos = glm::vec3(0, 0, 4);
	glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

	// Bind our diffuse texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, DiffuseTexture);
	// Set our "DiffuseTextureSampler" sampler to use Texture Unit 0
	glUniform1i(DiffuseTextureID, 0);

	// Bind our normal texture in Texture Unit 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, NormalTexture);
	// Set our "NormalTextureSampler" sampler to use Texture Unit 1
	glUniform1i(NormalTextureID, 1);

	// Bind our specular texture in Texture Unit 2
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, SpecularTexture);
	// Set our "SpecularTextureSampler" sampler to use Texture Unit 2
	glUniform1i(SpecularTextureID, 2);


	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 4th attribute buffer : tangents
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
	glVertexAttribPointer(
		3,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 5th attribute buffer : bitangents
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
	glVertexAttribPointer(
		4,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		indices.size(),    // count
		GL_UNSIGNED_SHORT, // type
		(void*)0           // element array buffer offset
	);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);


	////////////////////////////////////////////////////////
	// DEBUG ONLY !!!
	// Don't use this in real code !!
	////////////////////////////////////////////////////////



	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((const GLfloat*)&ProjectionMatrix[0]);
	glMatrixMode(GL_MODELVIEW);
	glm::mat4 MV = ViewMatrix * ModelMatrix;
	glLoadMatrixf((const GLfloat*)&MV[0]);


	glUseProgram(0);

	// normals
	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
	for (unsigned int i = 0; i < indices.size(); i++) {
		glm::vec3 p = indexed_vertices[indices[i]];
		glVertex3fv(&p.x);
		glm::vec3 o = glm::normalize(indexed_normals[indices[i]]);
		p += o * 0.1f;
		glVertex3fv(&p.x);
	}
	glEnd();
	// tangents
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	for (unsigned int i = 0; i < indices.size(); i++) {
		glm::vec3 p = indexed_vertices[indices[i]];
		glVertex3fv(&p.x);
		glm::vec3 o = glm::normalize(indexed_tangents[indices[i]]);
		p += o * 0.1f;
		glVertex3fv(&p.x);
	}
	glEnd();
	// bitangents
	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
	for (unsigned int i = 0; i < indices.size(); i++) {
		glm::vec3 p = indexed_vertices[indices[i]];
		glVertex3fv(&p.x);
		glm::vec3 o = glm::normalize(indexed_bitangents[indices[i]]);
		p += o * 0.1f;
		glVertex3fv(&p.x);
	}
	glEnd();
	// light pos
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex3fv(&lightPos.x);
	lightPos += glm::vec3(1, 0, 0)*0.1f;
	glVertex3fv(&lightPos.x);
	lightPos -= glm::vec3(1, 0, 0)*0.1f;
	glVertex3fv(&lightPos.x);
	lightPos += glm::vec3(0, 1, 0)*0.1f;
	glVertex3fv(&lightPos.x);
	glEnd();

	// Swap buffers
	glfwSwapBuffers(_window);
	glfwPollEvents();

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;

}

Renderer * Renderer::getInstance()
{
	if (instance == NULL) {
		instance = new Renderer();
	}
	return instance;
}
