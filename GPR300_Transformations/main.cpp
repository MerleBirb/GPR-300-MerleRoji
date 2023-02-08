#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "EW/Shader.h"
#include "EW/ShapeGen.h"

#include "Transform.h"
#include "TransformFunctions.h"

#include "Camera.h"

#include "time.h"

void resizeFrameBufferCallback(GLFWwindow* window, int width, int height);
void keyboardCallback(GLFWwindow* window, int keycode, int scancode, int action, int mods);

float lastFrameTime;
float deltaTime;

int SCREEN_WIDTH = 1080;
int SCREEN_HEIGHT = 720;

double prevMouseX;
double prevMouseY;
bool firstMouseInput = false;

/* Button to lock / unlock mouse
* 1 = right, 2 = middle
* Mouse will start locked. Unlock it to use UI
* */
const int MOUSE_TOGGLE_BUTTON = 1;
const float MOUSE_SENSITIVITY = 0.1f;

glm::vec3 bgColor = glm::vec3(0);
float exampleSliderFloat = 0.0f;

const int NUM_CUBES = 8;
const int MAX_DIST = 5;
const int MAX_DEG = 360;
const int MAX_SCALE = 3;
Transform cubes[NUM_CUBES];
Transform randTransforms[NUM_CUBES];

Camera camera((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);
float camRadius = 5.0f;
float camSpeed = 1.0f;
float camFOV = 60.0f;

glm::mat4 model = glm::mat4(1);

int main() {
	if (!glfwInit()) {
		printf("glfw failed to init");
		return 1;
	}
	
	srand(time(NULL));

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Transformations", 0, 0);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		printf("glew failed to init");
		return 1;
	}

	glfwSetFramebufferSizeCallback(window, resizeFrameBufferCallback);
	glfwSetKeyCallback(window, keyboardCallback);

	// Setup UI Platform/Renderer backends
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Dark UI theme.
	ImGui::StyleColorsDark();

	Shader shader("shaders/vertexShader.vert", "shaders/fragmentShader.frag");

	MeshData cubeMeshData;
	createCube(1.0f, 1.0f, 1.0f, cubeMeshData);

	Mesh cubeMesh(&cubeMeshData);

	//Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// set random position, rotation, scale for cubes
	for (size_t i = 0; i < NUM_CUBES; i++)
	{
		randTransforms[i].setScale(glm::vec3(rand() % MAX_SCALE + 1));
		randTransforms[i].setRotation(glm::quat(glm::vec3(rand() % MAX_DEG)));
		randTransforms[i].setPosition(glm::vec3(rand() % (2 * MAX_DIST) - MAX_DIST, rand() % (2 * MAX_DIST) - MAX_DIST, rand() % (2 * MAX_DIST) - MAX_DIST));
	}

	glm::vec3 ogCamPos = camera.getPosition();

	while (!glfwWindowShouldClose(window)) {
		glClearColor(bgColor.r,bgColor.g,bgColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		float time = (float)glfwGetTime();
		deltaTime = time - lastFrameTime;
		lastFrameTime = time;

		// set camera parameters
		camera.setPosition(glm::vec3(cos(time * camSpeed), ogCamPos.y, sin(time * camSpeed)) * camRadius);
		camera.setFOV(camFOV);

		// draw camera
		shader.setMat4("_Projection", camera.getProjectionMatrix());
		shader.setMat4("_View", camera.getViewMatrix());

		//Draw
		shader.use();
		for (size_t i = 0; i < NUM_CUBES; i++)
		{
			// set transformation
			cubes[i].setScale(randTransforms[i].getScale());
			cubes[i].setRotation(randTransforms[i].getRotation());
			cubes[i].setPosition(randTransforms[i].getPosition());

			shader.setMat4("_Model", cubes[i].getModelMatrix());
			cubeMesh.draw();
		}

		//Draw UI
		ImGui::Begin("Settings");
		ImGui::SliderFloat("Camera Radius", &camRadius, 0.0f, 20.0f);
		ImGui::SliderFloat("Camera Speed", &camSpeed, -10.0f, 10.0f);
		ImGui::SliderFloat("Camera FOV", &camFOV, 0.0f, 180.0f);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwPollEvents();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void resizeFrameBufferCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
}

void keyboardCallback(GLFWwindow* window, int keycode, int scancode, int action, int mods)
{
	if (keycode == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}