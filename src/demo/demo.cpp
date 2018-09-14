// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <common/shader.h>
#include <common/texture.h>
#include <common/camera.h>

GLFWwindow* window;

int window_width = 1280;
int window_height = 720;

std::string texture_file	= "assets/uvtemplate.DDS";
std::string fragment_shader	= "shaders/sprite.frag";
std::string vertex_shader	= "shaders/sprite.vert";

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( window_width, window_height, "Demo", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	//glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	//glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// Create and compile our GLSL program from the shaders
	// see: shader.h/cpp
	GLuint programID = loadShaders(vertex_shader.c_str(), fragment_shader.c_str());

	// Get a handle for our buffers
	GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
	GLuint vertexUVID = glGetAttribLocation(programID, "vertexUV");

	// Get a handle for our "MVP" uniform
	GLuint matrixID = glGetUniformLocation(programID, "MVP");
	// Get a handle for our "myTextureSampler" uniform
	GLuint textureID  = glGetUniformLocation(programID, "myTextureSampler");

	// Load the texture (see: texture.h/cpp)
	GLuint texture = loadDDS(texture_file.c_str());

	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A sprite has 1 face (quad) with 2 triangles each, so this makes 1*2=2 triangles, and 2*3 vertices
	int sprite_width = 512;
	int sprite_height = 512;
	GLfloat g_vertex_buffer_data[] = {
		 0.5f * sprite_width, -0.5f * sprite_height, 0.0f,
		-0.5f * sprite_width, -0.5f * sprite_height, 0.0f,
		-0.5f * sprite_width,  0.5f * sprite_height, 0.0f,
		-0.5f * sprite_width,  0.5f * sprite_height, 0.0f,
		 0.5f * sprite_width,  0.5f * sprite_height, 0.0f,
		 0.5f * sprite_width, -0.5f * sprite_height, 0.0f
	};

	// Two UV coordinates for each vertex.
	GLfloat g_uv_buffer_data[] = {
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

	glm::mat4 ProjectionMatrix = glm::ortho(0.0f, (float)window_width, (float)window_height, 0.0f, 0.1f, 100.0f);

	float rotation_z = 0.0f;

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Compute the ViewMatrix from keyboard and mouse input (see: camera.h/cpp)
		computeMatricesFromInputs(window);

		glm::vec3 cursor = getCursor();
		//printf("(%f,%f)\n",cursor.x, cursor.y);

		glm::mat4 ViewMatrix = getViewMatrix(); // get from Camera (Camera position and direction)
		glm::mat4 ModelMatrix = glm::mat4(1.0f);

		// Use our shader
		glUseProgram(programID);

		// Build the Model matrix
		glm::mat4 translationMatrix	= glm::translate(glm::mat4(1.0f), glm::vec3(500.0f, 300.0f, 0.0f));
		glm::mat4 rotationMatrix	= glm::eulerAngleYXZ(0.0f, 0.0f, rotation_z);
		glm::mat4 scalingMatrix		= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

		ModelMatrix = translationMatrix * rotationMatrix * scalingMatrix;

		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		rotation_z += 0.03f;

		// Send our transformation to the currently bound shader,
		// in the "MVP" uniform
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(textureID, 0);

		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(vertexPosition_modelspaceID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			vertexPosition_modelspaceID,  // The attribute we want to configure
			3,                            // size : x+y+z => 3
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(vertexUVID);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			vertexUVID,                   // The attribute we want to configure
			2,                            // size : U+V => 2
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);

		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, 2*3); // 2*3 indices starting at 0 -> 2 triangles

		glDisableVertexAttribArray(vertexPosition_modelspaceID);
		glDisableVertexAttribArray(vertexUVID);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &textureID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
