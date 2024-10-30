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
#include <string>

//variables
GLFWwindow* window;
const int width = 1080, height = 1080;


// Character variables
float characterXpos = -0.094444;
float characterYpos = -0.983333;
const float CHARACTER_SPEED = 0.005f;
const float CHARACTER_SIZE = 0.05f;

// Obstacles
const int MAX_OBSTACLES = 1000;
int OBSTACLES_COUNT = 0;
const float OBSTACLE_SIZE = 1.0f;
glm::vec3 obstacles[MAX_OBSTACLES];


//Handling cursor position
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
}

void button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
		FILE* file;
		file = fopen("obstacles.txt", "a");
		if (file == NULL) {
			std::cout << "Error opening file" << std::endl;
		}
		else {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			// Normalize the position to the screen
			xpos = xpos / width * 2 - 1;
			ypos = -ypos / height * 2 + 1;
			fprintf(file, "%f,%f\n", xpos, ypos);
			fclose(file);
			std::cout << "Obstacle position saved. (" << xpos << ", " << ypos << ")" << std::endl;

			// Add the obstacle to the obstacles array
			obstacles[OBSTACLES_COUNT] = glm::vec3(xpos, ypos, 0);
			OBSTACLES_COUNT++;

		}
	}
}

void window_callback(GLFWwindow* window, int new_width, int new_height)
{
	glViewport(0, 0, new_width, new_height);
}

// Helper function to check collision between character and obstacles
bool checkCollision(float newX, float newY) {
	for (int i = 0; i < OBSTACLES_COUNT; i++) {
		// Calculate bounding box for obstacle
		float obstacleLeft = obstacles[i].x - CHARACTER_SIZE / 2;
		float obstacleRight = obstacles[i].x + CHARACTER_SIZE / 2;
		float obstacleBottom = obstacles[i].y - CHARACTER_SIZE / 2;
		float obstacleTop = obstacles[i].y + CHARACTER_SIZE / 2;

		// Calculate bounding box for character
		float characterLeft = newX - CHARACTER_SIZE / 2;
		float characterRight = newX + CHARACTER_SIZE / 2;
		float characterBottom = newY - CHARACTER_SIZE / 2;
		float characterTop = newY + CHARACTER_SIZE / 2;

		// Check for collision using AABB (Axis-Aligned Bounding Box)
		if (characterRight > obstacleLeft && characterLeft < obstacleRight &&
			characterTop > obstacleBottom && characterBottom < obstacleTop) {
			return true;  // Collision detected
		}
	}
	return false;  // No collision
}

void moveCharacter() {
	float originalX = characterXpos;
	float originalY = characterYpos;

	// Check if movement keys are pressed and update position
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) characterYpos += CHARACTER_SPEED;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) characterYpos -= CHARACTER_SPEED;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) characterXpos -= CHARACTER_SPEED;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) characterXpos += CHARACTER_SPEED;

	// Ensure character stays within screen bounds
	characterXpos = std::max(-1.0f, std::min(characterXpos, 1.0f));
	characterYpos = std::max(-1.0f, std::min(characterYpos, 1.0f));

	// Check for collisions; if collision, revert to original position
	if (checkCollision(characterXpos, characterYpos)) {
		characterXpos = originalX;
		characterYpos = originalY;
		std::cout << "Collision detected! Movement blocked." << std::endl;
	}
	else {
		std::cout << "Character position: (" << characterXpos << ", " << characterYpos << ")" << std::endl;
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
	window = glfwCreateWindow(width, height, "2D Project", NULL, NULL);
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
		0.0f, 0.0f,    // Bottom-left corner
		CHARACTER_SIZE, 0.0f,   // Bottom-right corner
		0.0f, CHARACTER_SIZE,   // Top-left corner
		CHARACTER_SIZE, CHARACTER_SIZE   // Top-right corner
	};

	GLuint indices[] = {
		0, 1, 2,  // First triangle (bottom-left)
		1, 3, 2   // Second triangle (top-right)
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
		2,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		2 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);



	// Load obstacles from file
	FILE* obstaclesFile;
	obstaclesFile = fopen("obstacles.txt", "r");
	if (obstaclesFile == NULL) {
		std::cout << "Error opening file" << std::endl;
	}
	else {
		int i = 0;
		while (!feof(obstaclesFile) && i < MAX_OBSTACLES) {
			float x, y, z;
			int results = fscanf(obstaclesFile, "%f,%f\n", &x, &y);
			if (results != 2) {
				continue;
			}
			obstacles[i] = glm::vec3(x, y, 0.0f);
			i++;
			OBSTACLES_COUNT++;
		}
		fclose(obstaclesFile);
		std::cout << "Loaded " << OBSTACLES_COUNT << " obstacles" << std::endl;
		std::cout << "Positions: " << std::endl;
		for (int i = 0; i < OBSTACLES_COUNT; i++) {
			std::cout << obstacles[i].x << " " << obstacles[i].y << std::endl;
		}
	}


	// Action callbacks
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, button_callback);

	glfwSetFramebufferSizeCallback(window, window_callback);

	// Bind the VAO and shader program once at the beginning of the loop
	glBindVertexArray(vao);
	glUseProgram(programID);

	// Get uniform locations once
	unsigned int transformLoc = glGetUniformLocation(programID, "transform");
	unsigned int colorLoc = glGetUniformLocation(programID, "color");

	// Check if the window was closed
	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
	{
		// Swap buffers
		glfwSwapBuffers(window);

		// Check for events
		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
			glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
			glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
			glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				moveCharacter();
		}

		// Draw obstacles
		for (int i = 0; i < OBSTACLES_COUNT; i++) {
			glm::mat4 obstacle = glm::mat4(1.0f);  // Initialize as identity matrix
			obstacle = glm::translate(obstacle, glm::vec3(obstacles[i].x, obstacles[i].y, 0.0f));

			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(obstacle));
			glm::vec4 obstacleColor = glm::vec4(0.5f, 0.0f, 0.5f, 1.0f);
			glUniform4fv(colorLoc, 1, glm::value_ptr(obstacleColor));


			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		// Draw character
		glm::mat4 character = glm::mat4(1.0f); // Initialize as identity matrix
		character = glm::translate(character, glm::vec3(characterXpos, characterYpos, 0.0f));

		// Set transform and color uniforms for character
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(character));
		glm::vec4 characterColor = glm::vec4(0.0f, 0.5f, 0.5f, 1.0f);
		glUniform4fv(colorLoc, 1, glm::value_ptr(characterColor));

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


