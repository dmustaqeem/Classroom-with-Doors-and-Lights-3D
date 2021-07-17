#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/freeglut.h>
#include "Shader.h"


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void do_movement();
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// Window dimensions
const GLuint WIDTH = 1000, HEIGHT = 1000;

// Camera

// Camera
float camUpDown = 0.0;
glm::vec3 cameraPos = glm::vec3(-5.0f, -1.9f, 12.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat yaw = -90.0f; // Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
GLfloat pitch = 0.0f;
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];

// Deltatime
GLfloat deltaTime = 0.0f; // Time between current frame and last frame
GLfloat lastFrame = 0.0f;   // Time of last frame

// Light attributes
glm::vec3 lightPos2(10.0f, 0.0f, 5.0f);

// The MAIN function, from here we start the application and run the game loop

GLfloat coneVert[];

int k = 0;
float angle = -0.0;


GLfloat cameraSpeed;


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0, 0, HEIGHT, HEIGHT);

	glEnable(GL_DEPTH_TEST);

	GLuint shaderProgram = initShader("vert.glsl", "frag.glsl");
	GLuint lampshader = initShader("lampvert.glsl", "lampfrag.glsl");



	GLfloat Surface[] =
	{
		-100.0,-2.0, 100.5, 1.0f, 0.0f, 0.0f,
		 100.0,-2.0, 100.5, 1.0f, 0.0f, 0.0f,
		 100.0,-2.0, -100.0,1.0f, 0.0f, 0.0f,

		 100.0,-2.0, -100.0, 1.0f, 0.0f, 0.0f,
		-100.0,-2.0, -100.0, 1.0f, 0.0f, 0.0f,
		-100.0,-2.0,  100.5, 1.0f, 0.0f, 0.0f
	};



	GLfloat rightWall[] = {
	  0.0,		-2.0,	4.8, 1.0f, 0.0f, 0.0f,
	  0.0,		1.0,	4.8, 1.0f, 0.0f, 0.0f,
	  0.0,		1.0,	15.0, 1.0f, 0.0f, 0.0f,

	  0.0,		1.0,	15.0, 1.0f, 0.0f, 0.0f,
	  0.0,		-2.0,	15.0, 1.0f, 0.0f, 0.0f,
	  0.0,		-2.0,	4.8, 1.0f, 0.0f, 0.0f,
	};

	GLfloat leftWall[] = {
	  -10.0,		-2.0,	4.8, 1.0f, 0.0f, 0.0f,
	  -10.0,		0.0,	4.8, 1.0f, 0.0f, 0.0f,
	  -10.0,		0.0,	15.0, 1.0f, 0.0f, 0.0f,

	  -10.0,		0.0,	15.0, 1.0f, 0.0f, 0.0f,
	  -10.0,		-2.0,	15.0, 1.0f, 0.0f, 0.0f,
	  -10.0,		-2.0,	4.8, 1.0f, 0.0f, 0.0f,
	};

	GLfloat windowLeft[] =
	{
	  -10.0,		 0.0,	4.8, 1.0f, 0.0f, 0.0f,
	  -10.0,		 1.0,	4.8, 1.0f, 0.0f, 0.0f,
	  -10.0,		 1.0,	15.0, 1.0f, 0.0f, 0.0f,

	  -10.0,		 1.0,	15.0, 1.0f, 0.0f, 0.0f,
	  -10.0,		 0.0,	15.0, 1.0f, 0.0f, 0.0f,
	  -10.0,		 0.0,	4.8, 1.0f, 0.0f, 0.0f,
	};

	GLfloat partLeft[] =
	{
	  -9.99,		 0.0,	9.5, 1.0f, 0.0f, 0.0f,
	  -9.99,		 1.0,	9.5, 1.0f, 0.0f, 0.0f,
	  -9.99,		 1.0,	9.6, 1.0f, 0.0f, 0.0f,

	  -9.99,		 1.0,	9.6, 1.0f, 0.0f, 0.0f,
	  -9.99,		 0.0,	9.6, 1.0f, 0.0f, 0.0f,
	  -9.99,		 0.0,	9.5, 1.0f, 0.0f, 0.0f,
	};

	GLfloat backWall[] =
	{
		-10.0,		-2.0,	4.8, 1.0f, 0.0f, 0.0f,
		 -10.0,		 0.0,	4.8, 1.0f, 0.0f, 0.0f,
		  0.0,		 0.0,	4.8, 1.0f, 0.0f, 0.0f,

		  0.0,		 0.0,	4.8, 1.0f, 0.0f, 0.0f,
		  0.0,		-2.0,	4.8, 1.0f, 0.0f, 0.0f,
		 -10.0,		-2.0,	4.8, 1.0f, 0.0f, 0.0f,


		 -10.0,		-2.0,	15.0, 1.0f, 0.0f, 0.0f,
		 -10.0,		 1.0,	15.0, 1.0f, 0.0f, 0.0f,
		  -3.0,		 1.0,	15.0, 1.0f, 0.0f, 0.0f,

		  -3.0,		 1.0,	15.0, 1.0f, 0.0f, 0.0f,
		  -3.0,		-2.0,	15.0, 1.0f, 0.0f, 0.0f,
		 -10.0,		-2.0,	15.0, 1.0f, 0.0f, 0.0f,
		 //Left Side of Door 
		 -2.0,		-2.0,	15.0, 1.0f, 0.0f, 0.0f,
		 -2.0,		 1.0,	15.0, 1.0f, 0.0f, 0.0f,
		  0.0,		 1.0,	15.0, 1.0f, 0.0f, 0.0f,

		  0.0,		 1.0,	15.0, 1.0f, 0.0f, 0.0f,
		  0.0,		-2.0,	15.0, 1.0f, 0.0f, 0.0f,
		 -2.0,		-2.0,	15.0, 1.0f, 0.0f, 0.0f,
		 //Above Door
		 -3.0,		-1.0,	15.0, 1.0f, 0.0f, 0.0f,
		 -3.0,		 1.0,	15.0, 1.0f, 0.0f, 0.0f,
		 -2.0,		 1.0,	15.0, 1.0f, 0.0f, 0.0f,

		 -2.0,		 1.0,	15.0, 1.0f, 0.0f, 0.0f,
		 -2.0,		-1.0,	15.0, 1.0f, 0.0f, 0.0f,
		 -3.0,		-1.0,	15.0, 1.0f, 0.0f, 0.0f,
	};

	GLfloat door[] = 
	{
		 -3.0,		-2.0,	15.0, 1.0f, 0.0f, 0.0f,
		 -3.0,		-1.0,	15.0, 1.0f, 0.0f, 0.0f,
		 -2.0,		-1.0,	15.0, 1.0f, 0.0f, 0.0f,

		 -2.0,		-1.0,	15.0, 1.0f, 0.0f, 0.0f,
		 -2.0,		-2.0,	15.0, 1.0f, 0.0f, 0.0f,
		 -3.0,		-2.0,	15.0, 1.0f, 0.0f, 0.0f,
	};

	GLfloat lowerWall[] = 
	{
	  -0.10,		-2.0,	4.8, 1.0f, 0.0f, 0.0f,
	  -0.10,		-1.7,	4.8, 1.0f, 0.0f, 0.0f,
	  -0.10,		-1.7,	15.0, 1.0f, 0.0f, 0.0f,

	  -0.10,		-1.7,	15.0, 1.0f, 0.0f, 0.0f,
	  -0.10,		-2.0,	15.0, 1.0f, 0.0f, 0.0f,
	  -0.10,		-2.0,	4.8, 1.0f, 0.0f, 0.0f,
	};

	GLfloat windowRight[] =
	{
	  -0.10,		-1.5,	14.4, 1.0f, 0.0f, 0.0f,
	  -0.10,		-0.5,	14.4, 1.0f, 0.0f, 0.0f,
	  -0.10,		-0.5,	13.5, 1.0f, 0.0f, 0.0f,

	  -0.10,		-0.5,	13.5, 1.0f, 0.0f, 0.0f,
	  -0.10,		-1.5,	13.5, 1.0f, 0.0f, 0.0f,
	  -0.10,		-1.5,	14.4, 1.0f, 0.0f, 0.0f,
	  //Left Side 
	  -0.10,		-1.5,	12.6, 1.0f, 0.0f, 0.0f,
	  -0.10,		-0.5,	12.6, 1.0f, 0.0f, 0.0f,
	  -0.10,		-0.5,	13.4, 1.0f, 0.0f, 0.0f,

	  -0.10,		-0.5,	13.4, 1.0f, 0.0f, 0.0f,
	  -0.10,		-1.5,	13.4, 1.0f, 0.0f, 0.0f,
	  -0.10,		-1.5,	12.6, 1.0f, 0.0f, 0.0f,
	};

	GLfloat windowLines[] =
	{
	  -0.11,		-1.5,	14.4, 1.0f, 0.0f, 0.0f,
	  -0.11,		-0.5,	14.4, 1.0f, 0.0f, 0.0f,
	  -0.11,		-0.5,	14.3, 1.0f, 0.0f, 0.0f,
	  -0.11,		-1.5,	14.3, 1.0f, 0.0f, 0.0f,

	  -0.11,		-1.5,	14.2, 1.0f, 0.0f, 0.0f,
	  -0.11,		-0.5,	14.2, 1.0f, 0.0f, 0.0f,
	  -0.11,		-0.5,	14.1, 1.0f, 0.0f, 0.0f,
	  -0.11,		-1.5,	14.1, 1.0f, 0.0f, 0.0f,

	  -0.11,		-1.5,	14.0, 1.0f, 0.0f, 0.0f,
	  -0.11,		-0.5,	14.0, 1.0f, 0.0f, 0.0f,
	  -0.11,		-0.5,	13.9, 1.0f, 0.0f, 0.0f,
	  -0.11,		-1.5,	13.9, 1.0f, 0.0f, 0.0f,

	  -0.11,		-1.5,	13.8, 1.0f, 0.0f, 0.0f,
	  -0.11,		-0.5,	13.8, 1.0f, 0.0f, 0.0f,
	  -0.11,		-0.5,	13.7, 1.0f, 0.0f, 0.0f,
	  -0.11,		-1.5,	13.7, 1.0f, 0.0f, 0.0f,

	  //Le1t Side 
	  -0.11,		-1.5,	13.3, 1.0f, 0.0f, 0.0f,
	  -0.11,		-0.5,	13.3, 1.0f, 0.0f, 0.0f,
	  -0.11,		-0.5,	13.4, 1.0f, 0.0f, 0.0f,
	  -0.11,		-1.5,	13.4, 1.0f, 0.0f, 0.0f,

	  -0.11,		-1.5,	13.1, 1.0f, 0.0f, 0.0f,
	  -0.11,		-0.5,	13.1, 1.0f, 0.0f, 0.0f,
	  -0.11,		-0.5,	13.2, 1.0f, 0.0f, 0.0f,
	  -0.11,		-1.5,	13.2, 1.0f, 0.0f, 0.0f,

	  -0.11,		-1.5,	12.9, 1.0f, 0.0f, 0.0f,
	  -0.11,		-0.5,	12.9, 1.0f, 0.0f, 0.0f,
	  -0.11,		-0.5,	13.0, 1.0f, 0.0f, 0.0f,
	  -0.11,		-1.5,	13.0, 1.0f, 0.0f, 0.0f,

	  -0.11,		-1.5,	12.7, 1.0f, 0.0f, 0.0f,
	  -0.11,		-0.5,	12.7, 1.0f, 0.0f, 0.0f,
	  -0.11,		-0.5,	12.8, 1.0f, 0.0f, 0.0f,
	  -0.11,		-1.5,	12.8, 1.0f, 0.0f, 0.0f,
	};

	GLfloat partitionWR[] =
	{
	  -0.10,		-1.5,	13.4, 1.0f, 0.0f, 0.0f,
	  -0.10,		-0.5,	13.4, 1.0f, 0.0f, 0.0f,
	  -0.10,		-0.5,	13.5, 1.0f, 0.0f, 0.0f,

	  -0.10,		-0.5,	13.5, 1.0f, 0.0f, 0.0f,
	  -0.10,		-1.5,	13.5, 1.0f, 0.0f, 0.0f,
	  -0.10,		-1.5,	13.4, 1.0f, 0.0f, 0.0f,
	};


	GLfloat windowBack[] =
	{
		 -10.0,		0.0,	4.8, 1.0f, 0.0f, 0.0f,
		 -10.0,		1.0,	4.8, 1.0f, 0.0f, 0.0f,
		  0.0,		1.0,	4.8, 1.0f, 0.0f, 0.0f,

		  0.0,		1.0,	4.8, 1.0f, 0.0f, 0.0f,
		  0.0,		0.0,	4.8, 1.0f, 0.0f, 0.0f,
		 -10.0,		0.0,	4.8, 1.0f, 0.0f, 0.0f,

	};

	GLfloat partBack[] =
	{
		 -5.1,		0.0,	4.81, 1.0f, 0.0f, 0.0f,
		 -5.1,		1.0,	4.81, 1.0f, 0.0f, 0.0f,
		 -5.0,		1.0,	4.81, 1.0f, 0.0f, 0.0f,

		 -5.0,		1.0,	4.81, 1.0f, 0.0f, 0.0f,
		 -5.0,		0.0,	4.81, 1.0f, 0.0f, 0.0f,
		 -5.1,		0.0,	4.81, 1.0f, 0.0f, 0.0f,
	};

	GLfloat roof[] =
	{
	  -10.0,		1.0,	4.8, 1.0f, 0.0f, 0.0f,
		0.0,		1.0,	4.8, 1.0f, 0.0f, 0.0f,
		0.0,		1.0,	15.0, 1.0f, 0.0f, 0.0f,

		0.0,		1.0,	15.0, 1.0f, 0.0f, 0.0f,
	   -10.0,		1.0,	15.0, 1.0f, 0.0f, 0.0f,
	  -10.0,		1.0,	4.8, 1.0f, 0.0f, 0.0f,
	};

	
	GLuint VBO[30], VAO[30];
	glGenVertexArrays(30, VAO);
	glGenBuffers(30, VBO);

	//Surface 
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Surface), Surface, GL_STATIC_DRAW);

	//glBindVertexArray(containerVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);


	//Surface 
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rightWall), rightWall, GL_STATIC_DRAW);

	//glBindVertexArray(containerVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);


	glBindVertexArray(VAO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(leftWall), leftWall, GL_STATIC_DRAW);

	//glBindVertexArray(containerVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glBindVertexArray(VAO[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(windowLeft), windowLeft, GL_STATIC_DRAW);

	//glBindVertexArray(containerVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glBindVertexArray(VAO[5]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(backWall), backWall, GL_STATIC_DRAW);

	//glBindVertexArray(containerVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);


	glBindVertexArray(VAO[6]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(windowBack), windowBack, GL_STATIC_DRAW);

	//glBindVertexArray(containerVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glBindVertexArray(VAO[7]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(partLeft), partLeft, GL_STATIC_DRAW);

	//glBindVertexArray(containerVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glBindVertexArray(VAO[8]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(partBack), partBack, GL_STATIC_DRAW);

	//glBindVertexArray(containerVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glBindVertexArray(VAO[9]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[9]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(roof), roof, GL_STATIC_DRAW);

	//glBindVertexArray(containerVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glBindVertexArray(VAO[10]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[10]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(door), door, GL_STATIC_DRAW);

	//glBindVertexArray(containerVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glBindVertexArray(VAO[11]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[11]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lowerWall), lowerWall, GL_STATIC_DRAW);

	//glBindVertexArray(containerVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glBindVertexArray(VAO[12]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[12]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(windowRight), windowRight, GL_STATIC_DRAW);

	//glBindVertexArray(containerVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glBindVertexArray(VAO[13]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[13]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(partitionWR), partitionWR, GL_STATIC_DRAW);

	//glBindVertexArray(containerVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);


	glBindVertexArray(VAO[14]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[14]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(windowLines), windowLines, GL_STATIC_DRAW);

	//glBindVertexArray(containerVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the normal vectors
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	while (!glfwWindowShouldClose(window))
	{

		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		do_movement();


		glClearColor(0.0, 0.8, 1.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);
		GLint objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
		GLint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
		GLint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");

		glUniform3f(objectColorLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightPosLoc, lightPos2.x, lightPos2.y, lightPos2.z);
		glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);


		// Create camera transformations
		glm::mat4 view;
		glm::mat4 projection;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
		GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
		GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));




		glm::mat4 model;

		glBindVertexArray(VAO[1]);
		glm::vec3 blue1D(0.0f, 0.0f, 0.5f);
		model = glm::translate(model, blue1D);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(objectColorLoc, 0.623529f, 0.623529f, 0.372549f);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glBindVertexArray(VAO[2]);
		glUniform3f(objectColorLoc, 1.0, 1.0, 1.0);
		glDrawArrays(GL_TRIANGLES, 0, 500);
		glBindVertexArray(0);


		glBindVertexArray(VAO[3]);
		glUniform3f(objectColorLoc, 1.0, 1.0, 1.0);
		glDrawArrays(GL_TRIANGLES, 0, 500);
		glBindVertexArray(0);

		glBindVertexArray(VAO[4]);
		glUniform3f(objectColorLoc, 0.0, 0.6, 1.0);
		glDrawArrays(GL_TRIANGLES, 0, 500);
		glBindVertexArray(0);

		glBindVertexArray(VAO[5]);
		glUniform3f(objectColorLoc, 1.0, 1.0, 1.0);
		glDrawArrays(GL_TRIANGLES, 0, 500);
		glBindVertexArray(0);

		glBindVertexArray(VAO[6]);
		glUniform3f(objectColorLoc, 0.0, 0.6, 1.0);
		glDrawArrays(GL_TRIANGLES, 0, 500);
		glBindVertexArray(0);

		glBindVertexArray(VAO[7]);
		glUniform3f(objectColorLoc, 1.0, 1.0, 1.0);
		glDrawArrays(GL_TRIANGLES, 0, 500);
		glBindVertexArray(0);

		glBindVertexArray(VAO[8]);
		glUniform3f(objectColorLoc, 1.0, 1.0, 1.0);
		glDrawArrays(GL_TRIANGLES, 0, 500);
		glBindVertexArray(0);

		glBindVertexArray(VAO[9]);
		glUniform3f(objectColorLoc, 1.0, 1.0, 1.0);
		glDrawArrays(GL_TRIANGLES, 0, 500);
		glBindVertexArray(0);

		glBindVertexArray(VAO[10]);
		glUniform3f(objectColorLoc, 0.0, 0.0, 1.0);
		glDrawArrays(GL_TRIANGLES, 0, 500);
		glBindVertexArray(0);

		glBindVertexArray(VAO[11]);
		glUniform3f(objectColorLoc, 0.752941,0.752941, 0.752941);
		glDrawArrays(GL_TRIANGLES, 0, 500);
		glBindVertexArray(0);

		glBindVertexArray(VAO[12]);
		glUniform3f(objectColorLoc, 0.0, 0.6, 1.0);
		glDrawArrays(GL_TRIANGLES, 0, 500);
		glBindVertexArray(0);

		glBindVertexArray(VAO[13]);
		glUniform3f(objectColorLoc, 0,0,0);
		glDrawArrays(GL_TRIANGLES, 0, 500);
		glBindVertexArray(0);

		glBindVertexArray(VAO[14]);
		glUniform3f(objectColorLoc, 0, 0, 0);
		glDrawArrays(GL_QUADS, 0, 500);
		glBindVertexArray(0);

		glPushMatrix();
		glm::vec3 t1(-0.1f, 0.0f, 14.0f);
		model = glm::translate(model, t1);
		model = glm::rotate(model, 1.5f, glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(objectColorLoc, 0.36, 0.25, 0.20);
		glutSolidCylinder(0.1,0.1, 100, 100);
		glPopMatrix();
		glBindVertexArray(0);

		glPushMatrix();
		glm::vec3 t2(-1.1f, 0.0f, -1.6f);
		model = glm::translate(model, t2);
		model = glm::rotate(model, 1.5f, glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(objectColorLoc, 0.36, 0.25, 0.20);
		glutSolidCylinder(0.1, 0.1, 100, 100);
		glPopMatrix();
		glBindVertexArray(0);


		glUseProgram(lampshader);

		modelLoc = glGetUniformLocation(lampshader, "model");
		viewLoc = glGetUniformLocation(lampshader, "view");
		projLoc = glGetUniformLocation(lampshader, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4();

		model = glm::translate(model, lightPos2);
		model = glm::scale(model, glm::vec3(0.2f));


		glPushMatrix();
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glutSolidSphere(1.0, 30, 30);
		glPopMatrix();
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

float speed = 5.0f;
void do_movement()
{
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (GLfloat)xpos;
		lastY = (GLfloat)ypos;
		firstMouse = false;
	}

	GLfloat xoffset = (GLfloat)xpos - lastX;
	GLfloat yoffset = lastY - (GLfloat)ypos; // Reversed since y-coordinates go from bottom to left
	lastX = (GLfloat)xpos;
	lastY = (GLfloat)ypos;

	GLfloat sensitivity = (GLfloat)0.05; // Change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);


}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		speed += 1.0;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		speed -= 1.0;
	}
}





