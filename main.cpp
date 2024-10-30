// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#include "dependente\glew\glew.h"

// Include GLFW
#include "dependente\glfw\glfw3.h"

// Include GLM
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

#include "shader.hpp"

//variables
GLFWwindow* window;
const int width = 1024, height = 1024;
int randomSquarePosition = 0;
float characterXpos = 0;
float characterYpos = 0;

//Handling cursor position
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	std::cout << "The mouse cursor is: " << xpos << " " << ypos << std::endl;
}

void button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
		randomSquarePosition = rand() % 10;
		std::cout << "Random square position: " << randomSquarePosition << std::endl;
	}
}

void window_callback(GLFWwindow* window, int new_width, int new_height)
{
	glViewport(0, 0, new_width, new_height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		characterYpos += 0.1f;
		if (characterYpos > 1.0f) {
			characterYpos = 1.0f;
		}
		std::cout << "Character position: " << characterXpos << " " << characterYpos << std::endl;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		characterYpos -= 0.1f;
		if (characterYpos < -1.0f) {
			characterYpos = -1.0f;
		}
		std::cout << "Character position: " << characterXpos << " " << characterYpos << std::endl;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		characterXpos -= 0.1f;
		if (characterXpos < -1.0f) {
			characterXpos = -1.0f;
		}
		std::cout << "Character position: " << characterXpos << " " << characterYpos << std::endl;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		characterXpos += 0.1f;
		if (characterXpos > 1.0f) {
			characterXpos = 1.0f;
		}
		std::cout << "Character position: " << characterXpos << " " << characterYpos << std::endl;
	}
	
}

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "Lab 4", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	//specify the size of the rendering window
	glViewport(0, 0, width, height);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	GLfloat vertices[] = {
		0.05f,  0.05f, 0.0f,
		0.05f, -0.05f, 0.0f,
		-0.05f, -0.05f, 0.0f,
		-0.05f,  0.05f, 0.0f 
	};

	GLuint indices[] = {
		0, 3, 1,
		1, 3, 2,
	};


	GLuint vbo, vao, ibo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	//bind VAO
	glBindVertexArray(vao);

	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//bind IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);

	//create matrices for transforms
	glm::mat4 trans(1.0f);

	//maybe we can play with different positions
	glm::vec3 positions[] = {
		glm::vec3(0.0f,  0.0f,  0),
		glm::vec3(0.2f,  0.5f, 0),
		glm::vec3(-0.15f, -0.22f, 0),
		glm::vec3(-0.38f, -0.2f, 0),
		glm::vec3(0.24f, -0.4f, 0),
		glm::vec3(-0.17f,  0.3f, 0),
		glm::vec3(0.23f, -0.2f, 0),
		glm::vec3(0.15f,  0.2f, 0),
		glm::vec3(0.15f,  0.7f, 0),
		glm::vec3(-0.13f,  0.1f, 0)
	};

	// Set a callback for handling mouse cursor position
	// Decomment for a callback example
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, button_callback);
	glfwSetKeyCallback(window, key_callback);


	glfwSetFramebufferSizeCallback(window, window_callback);


	// Check if the window was closed
	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
	{
		// Swap buffers
		glfwSwapBuffers(window);

		// Check for events
		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			trans = glm::rotate(trans, 0.1f, glm::vec3(0.0, 0.0, 1.0));
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
			trans = glm::rotate(trans, -0.1f, glm::vec3(0.0, 0.0, 1.0));

		}

		trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(characterXpos, characterYpos, 0.0f));

		//bind VAO
		glBindVertexArray(vao);

		// send variables to shader
		unsigned int transformLoc = glGetUniformLocation(programID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		unsigned int transformLoc2 = glGetUniformLocation(programID, "color");
		glm::vec4 color = glm::vec4(0.5f, 0, 0.5f, 1.0);
		glUniform4fv(transformLoc2, 1, glm::value_ptr(color));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	// Cleanup
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}


