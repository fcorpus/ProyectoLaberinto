#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Cubo.h"
#include "Camera.h"
#include "jugador.h"

using namespace std;

const unsigned width = 1000;
const unsigned height = 600;

unsigned int VBO, VAO, EBO,
			 VBO1, VAO1, EBO1,
			 VBO2, VAO2, EBO2,
			 VBO3, VAO3, EBO3;

unsigned int tamVAO, tamVAO1, tamVAO2, tamVAO3;

vec3 inicio = vec3(3, 13, 0.1);

vec3 cameraPos = vec3(7.0f, 7.0f, 25.0f);
vec3 cameraFront = vec3(7.0f, 7.0f, 3.0f);
vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
float deltaTime = 0.0f;
float lastTime = 0.0f;

Camera camera(vec3(7.0f, 7.0f, 25.0f));
float lastX = width / 2.0f;
float lastY = height / 2.0f;
bool firstMouse = true;

bool gravedadActive = false;

struct AABB
{
	vec3 min;
	vec3 max;
};

void initGLFWVersion();
bool gladLoad();
void updateWindow(GLFWwindow* window, Shader ourShader);

void framebuffer_size_callback(GLFWwindow* window, int w, int h);
void processInput(GLFWwindow* window);
void CameraInput(GLFWwindow* window);
void PlayerInput(GLFWwindow* window);
void Mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void GeneracionBuffer(
	GLuint& VAO,
	GLuint& VBO,
	GLuint& EBO,
	const GLfloat* vertices,
	size_t verticesSize,
	const GLuint* indices,
	size_t indicesSize);
void VertexAttribute(int layout, int data, int total, int start);
void DeleteBuffer(GLuint& VAO, GLuint& VBO, GLuint& EBO);
void Transform(Shader ourShader, vec3 posicion[], int objVAO, int tam);
/*void TransformCubo(Shader ourShader);
void TransformPisoInicio(Shader ourShader);
void TransformPisoFinal(Shader ourShader);*/
void TransformCamera(Shader ourShader);

void updatePhysics(float deltaTime);

bool AABBIntersect(AABB box1, AABB box2);
AABB GenerateBoundingBox(vec3 position, float w, float h, float d);
bool DetecCollision();

int main()
{
	initGLFWVersion();

	//Creacion de ventana
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGl", NULL, NULL);
	if (window == NULL)
	{
		cout << "Falla al cargar ventana" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoad())
	{
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//glfwSetCursorPosCallback(window, Mouse_callback);
	//glfwSetScrollCallback(window, Scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	Shader ourShader("vertexShader.vs", "fragmentShader.fs");

	GeneracionBuffer(VAO, VBO, EBO, vertices, sizeof(vertices), indices, sizeof(indices));
	GeneracionBuffer(VAO1, VBO1, EBO1, verticesPisoInicio, sizeof(verticesPisoInicio), indices, sizeof(indices));
	GeneracionBuffer(VAO2, VBO2, EBO2, verticesPisoFinal, sizeof(verticesPisoFinal), indices, sizeof(indices));
	GeneracionBuffer(VAO3, VBO3, EBO3, jugadorVertices, sizeof(jugadorVertices), indices, sizeof(indicesJugador));

	tamVAO = sizeof(posCube) / sizeof(posCube[0]);
	tamVAO1 = sizeof(posPisoInicio) / sizeof(posPisoInicio[0]);
	tamVAO2 = sizeof(posPisoFinal) / sizeof(posPisoFinal[0]);
	tamVAO3 = sizeof(posJugador) / sizeof(posJugador[0]);

	updateWindow(window, ourShader);

	DeleteBuffer(VAO, VBO, EBO);
	DeleteBuffer(VAO1, VBO1, EBO1);
	DeleteBuffer(VAO2, VBO2, EBO2);
	DeleteBuffer(VAO3, VBO3, EBO3);

	glfwTerminate();
	return 0;
}
void initGLFWVersion()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__
}
bool gladLoad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Falla al cargar glad" << endl;
		return false;
	}
	return true;
}
void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	PlayerInput(window);
	//CameraInput(window);
}

void PlayerInput(GLFWwindow* window)
{

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		posJugador[0].y += 0.01f;
		if (DetecCollision()) 
		{
			posJugador[0].y -= 0.01f; 
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		posJugador[0].y -= 0.01f;
		if (DetecCollision()) 
		{
			posJugador[0].y += 0.01f; 
		}
	}

	
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		posJugador[0].x -= 0.01f;
		if (DetecCollision()) 
		{
			posJugador[0].x += 0.01f; 
		}
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		posJugador[0].x += 0.01f;
		if (DetecCollision()) 
		{
			posJugador[0].x -= 0.01f; 
		}
	}


	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		gravedadActive = true;
	}
	/*if (DetecCollision())
	{
		jugadorVel.z = 0;
		posJugador[0].z = posCube[0].z;
	}*/
}

void CameraInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

void Mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = xposIn;
	float ypos = yposIn;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
void Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void updateWindow(GLFWwindow* window, Shader ourShader)
{
	while (!glfwWindowShouldClose(window))
	{
		float currenTime = glfwGetTime();
		deltaTime = currenTime - lastTime;
		lastTime = currenTime;

		processInput(window);

		if (gravedadActive)
		{
			updatePhysics(deltaTime);
		}	

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.use();
		TransformCamera(ourShader);

		//TransformCubo(ourShader);

		//TransformPisoInicio(ourShader);
		//TransformPisoFinal(ourShader);

		Transform(ourShader, posCube, VAO, tamVAO);
		Transform(ourShader, posPisoInicio, VAO1, tamVAO1);
		Transform(ourShader, posPisoFinal, VAO2, tamVAO2);
		Transform(ourShader, posJugador, VAO3, tamVAO3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void GeneracionBuffer(GLuint& VAO,
	GLuint& VBO,
	GLuint& EBO,
	const GLfloat* vertices,
	size_t verticesSize,
	const GLuint* indices,
	size_t indicesSize)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

	VertexAttribute(0, 3, 6, 0);
	VertexAttribute(1, 3, 6, 3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void VertexAttribute(int layout, int data, int total, int start)
{
	glVertexAttribPointer(layout, data, GL_FLOAT, GL_FALSE, total * sizeof(float), (void*)(start * sizeof(float)));
	glEnableVertexAttribArray(layout);
}
void DeleteBuffer(GLuint& VAO, GLuint& VBO, GLuint& EBO)
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Transform(Shader ourShader, vec3 posicion[], int objVAO, int tam)
{
	//cout << (int)posicion[1];
	glBindVertexArray(objVAO);
	for (int i = 0; i < tam; i++)
	{
		mat4 modelo = mat4(1.0f);
		modelo = translate(modelo, posicion[i]);
		//modelo = rotate(modelo, radians(-45.0f), vec3(0.3f, 0.7f, 0.0f));
		ourShader.setMat4("modelo", modelo);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);
	}
}

void TransformCamera(Shader ourShader)
{
	mat4 projection;
	projection = perspective(radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
	ourShader.setMat4("projection", projection);

	mat4 view = camera.GetViewMatrix();
	ourShader.setMat4("view", view);
}

void updatePhysics(float deltaTime)
{
	jugadorVel.z += gravedad * deltaTime;
	posJugador[0].z += jugadorVel.z * deltaTime;

	if (posJugador[0].z < 0.1f)
	{
		posJugador[0].z = 0.1f;
		jugadorVel.z = 0;
	}
}

bool AABBIntersect(AABB box1, AABB box2)
{
	bool col;
	bool xOverlap = box1.min.x <= box2.max.x && box1.max.x >= box2.min.x;
	bool yOverlap = box1.min.y <= box2.max.y && box1.max.y >= box2.min.y;
	bool zOverlap = box1.min.z <= box2.max.z && box1.max.z >= box2.min.z;

	col = xOverlap && yOverlap && zOverlap;
	return col;
}
AABB GenerateBoundingBox(vec3 position, float w, float h, float d)
{
	AABB box;
	box.min = vec3((position.x - w) / 2.0f, position.y, (position.z - d) / 2.0f);
	box.max = vec3((position.x + w) / 2.0f, position.y + h, (position.z + d) / 2.0f);
	return box;
}
bool DetecCollision()
{
	bool col = false;
	AABB jugadorBox = GenerateBoundingBox(posJugador[0], 0.3f, 0.8f, 0.3f);
	AABB finBox = GenerateBoundingBox(posPisoFinal[0], 0.7f, 0.7f, 0.7f);

	for (int i = 0; i < tamVAO; i++)
	{
		AABB cubeBox = GenerateBoundingBox(posCube[i], 0.6f, 0.8f, 1.0f);
		if (AABBIntersect(jugadorBox, cubeBox))
		{
			col = true;
			break;
		}
	}

	if (AABBIntersect(jugadorBox, finBox))
	{
		posJugador[0] = inicio;
	}
	return col;
}