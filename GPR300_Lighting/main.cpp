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
#include "EW/EwMath.h"
#include "EW/Camera.h"
#include "EW/Mesh.h"
#include "EW/Transform.h"
#include "EW/ShapeGen.h"

void processInput(GLFWwindow* window);
void resizeFrameBufferCallback(GLFWwindow* window, int width, int height);
void keyboardCallback(GLFWwindow* window, int keycode, int scancode, int action, int mods);
void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void mousePosCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

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
const float CAMERA_MOVE_SPEED = 5.0f;
const float CAMERA_ZOOM_SPEED = 3.0f;

Camera camera((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT);

glm::vec3 bgColor = glm::vec3(0);
glm::vec3 lightColor = glm::vec3(1.0f);
glm::vec3 lightPosition = glm::vec3(0.0f, 3.0f, 0.0f);

bool wireFrame = false;

struct DirectionalLight
{
	glm::vec3 direction;
	glm::vec3 color;
	float intensity;
};

const int MAX_DIR_LIGHTS = 1;

struct PointLight
{
	glm::vec3 position;
	glm::vec3 color;
	float radius;
	float intensity;
};

const int MAX_PNT_LIGHTS = 2;

struct SpotLight
{
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 color;
	float intensity;
	float minAngle;
	float maxAngle;
};

const int MAX_SPT_LIGHTS = 1;

// NOTES:
/*
* - write uniforms for coefficients, like shininess, etc
* - create data in main
* - create variables in glsl
* - provide ImGUI controls as sliders for coefficients
* - create other lights, point, spotlight, etc
*/

int main() {
	if (!glfwInit()) {
		printf("glfw failed to init");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lighting", 0, 0);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		printf("glew failed to init");
		return 1;
	}

	glfwSetFramebufferSizeCallback(window, resizeFrameBufferCallback);
	glfwSetKeyCallback(window, keyboardCallback);
	glfwSetScrollCallback(window, mouseScrollCallback);
	glfwSetCursorPosCallback(window, mousePosCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	//Hide cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Setup UI Platform/Renderer backends
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Dark UI theme.
	ImGui::StyleColorsDark();

	//Used to draw shapes. This is the shader you will be completing.
	Shader litShader("shaders/defaultLit.vert", "shaders/defaultLit.frag");

	//Used to draw light sphere
	Shader unlitShader("shaders/defaultLit.vert", "shaders/unlit.frag");

	//material settings
	float ambCoefficient = 0.2f;
	float difCoefficient = 0.8f;
	float specCoefficient = 0.8f;
	int shininess = 64.0f;
	glm::vec3 objColor = glm::vec3(0.5f, 0.5f, 0.5f);

	/// DIRECTIONAL LIGHT
	// create lighting
	DirectionalLight dirLights[MAX_DIR_LIGHTS];

	// light settings
	ew::Transform dirLightTransform;
	dirLightTransform.scale = glm::vec3(0.5f);
	dirLightTransform.position = glm::vec3(0.0f, 5.0f, 0.0f);

	for (size_t d = 0; d < MAX_DIR_LIGHTS; d++)
	{
		dirLights[d].direction = dirLightTransform.position;
		dirLights[d].color = glm::vec3(0.5f, 0.5f, 0.5f);
		dirLights[d].intensity = 1.0f;
	}

	/// POINT LIGHT
	// create lighting
	PointLight pntLights[MAX_PNT_LIGHTS];

	// light settings
	ew::Transform pntLightTransforms[MAX_PNT_LIGHTS];

	for (size_t p = 0; p < MAX_PNT_LIGHTS; p++)
	{
		pntLightTransforms[p].scale = glm::vec3(0.5f);
		pntLightTransforms[0].position = glm::vec3(0.5f, 2.0f, 1.5f);
		pntLightTransforms[1].position = glm::vec3(-0.5f, 2.0f, 1.5f);

		pntLights[p].position = pntLightTransforms[p].position;
		pntLights[p].color = glm::vec3(0.5f, 0.5f, 0.5f);
		pntLights[p].radius = pntLightTransforms[p].scale.x;
		pntLights[p].intensity = 1.0f;
	}

	/// SPOTLIGHT
	SpotLight sptLights[MAX_SPT_LIGHTS];
	ew::Transform sptLightTransform;
	sptLightTransform.scale = glm::vec3(0.5f);
	sptLightTransform.position = glm::vec3(0.0f, 3.0f, 0.0f);

	for (size_t s = 0; s < MAX_SPT_LIGHTS; s++)
	{
		sptLights[s].position = sptLightTransform.position;
		sptLights[s].direction = sptLightTransform.position;
		sptLights[s].color = glm::vec3(0.5f, 0.5f, 0.5f);
		sptLights[s].intensity = 1.0f;
		//sptLights[s].minAngle = cos(rad)
	}

	// create mesh data
	ew::MeshData cubeMeshData;
	ew::createCube(1.0f, 1.0f, 1.0f, cubeMeshData);
	ew::MeshData sphereMeshData;
	ew::createSphere(0.5f, 64, sphereMeshData);
	ew::MeshData cylinderMeshData;
	ew::createCylinder(1.0f, 0.5f, 64, cylinderMeshData);
	ew::MeshData planeMeshData;
	ew::createPlane(1.0f, 1.0f, planeMeshData);

	ew::Mesh cubeMesh(&cubeMeshData);
	ew::Mesh sphereMesh(&sphereMeshData);
	ew::Mesh planeMesh(&planeMeshData);
	ew::Mesh cylinderMesh(&cylinderMeshData);

	//Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//Initialize shape transforms
	ew::Transform cubeTransform;
	ew::Transform sphereTransform;
	ew::Transform planeTransform;
	ew::Transform cylinderTransform;

	cubeTransform.position = glm::vec3(-2.0f, 0.0f, 0.0f);
	sphereTransform.position = glm::vec3(0.0f, 0.0f, 0.0f);

	planeTransform.position = glm::vec3(0.0f, -1.0f, 0.0f);
	planeTransform.scale = glm::vec3(10.0f);

	cylinderTransform.position = glm::vec3(2.0f, 0.0f, 0.0f);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(bgColor.r,bgColor.g,bgColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		float time = (float)glfwGetTime();
		deltaTime = time - lastFrameTime;
		lastFrameTime = time;

		//Draw
		litShader.use();
		litShader.setMat4("_Projection", camera.getProjectionMatrix());
		litShader.setMat4("_View", camera.getViewMatrix());

		litShader.setVec3("_CameraPos", camera.getPosition());

		// Set some lighting uniforms
		// Directional Lights
		for (size_t d = 0; d < MAX_DIR_LIGHTS; d++)
		{
			litShader.setVec3("_DirLights[" + std::to_string(d) + "].direction", dirLightTransform.position);
			litShader.setVec3("_DirLights[" + std::to_string(d) + "].color", dirLights[d].color);
			litShader.setFloat("_DirLights[" + std::to_string(d) + "].intensity", dirLights[d].intensity);
		}

		// Point Lights
		for (size_t p = 0; p < MAX_PNT_LIGHTS; p++)
		{
			litShader.setVec3("_PntLights[" + std::to_string(p) + "].position", pntLightTransforms[p].position);
			litShader.setVec3("_PntLights[" + std::to_string(p) + "].color", pntLights[p].color);
			litShader.setFloat("_PntLights[" + std::to_string(p) + "].radius", pntLights[p].radius);
			litShader.setFloat("_PntLights[" + std::to_string(p) + "].intensity", pntLights[p].intensity);
		}

		// Spotlights
		for (size_t i = 0; i < MAX_SPT_LIGHTS; i++)
		{

		}

		//Draw cube
		litShader.setMat4("_Model", cubeTransform.getModelMatrix());
		cubeMesh.draw();

		//Draw sphere
		litShader.setMat4("_Model", sphereTransform.getModelMatrix());
		sphereMesh.draw();

		//Draw cylinder
		litShader.setMat4("_Model", cylinderTransform.getModelMatrix());
		cylinderMesh.draw();

		//Draw plane
		litShader.setMat4("_Model", planeTransform.getModelMatrix());
		planeMesh.draw();

		// coefficients
		litShader.setFloat("_Material.ambientCoefficient", ambCoefficient);
		litShader.setFloat("_Material.diffuseCoefficient", difCoefficient);
		litShader.setFloat("_Material.specularCoefficient", specCoefficient);
		litShader.setFloat("_Material.shininess", shininess);
		litShader.setVec3("_Material.objColor", objColor);

		//Draw light as a small sphere using unlit shader, ironically.
		unlitShader.use();
		unlitShader.setMat4("_Projection", camera.getProjectionMatrix());
		unlitShader.setMat4("_View", camera.getViewMatrix());

		//Directional Light
		unlitShader.setMat4("_Model", dirLightTransform.getModelMatrix());
		unlitShader.setVec3("_Color", lightColor);
		sphereMesh.draw();

		// Point light
		unlitShader.setMat4("_Model", pntLightTransforms[0].getModelMatrix());
		unlitShader.setVec3("_Color", lightColor);
		sphereMesh.draw();
		unlitShader.setMat4("_Model", pntLightTransforms[1].getModelMatrix());
		unlitShader.setVec3("_Color", lightColor);
		sphereMesh.draw();

		//Draw UI
		ImGui::Begin("Settings");

		ImGui::ColorEdit3("Material Color", &objColor.r);
		ImGui::ColorEdit3("Directional Light Color", &dirLights[0].color.r);
		ImGui::DragFloat3("Directional Light Position", &dirLightTransform.position.x, 0.1f);
		ImGui::DragFloat("Directional Light Intensity", &dirLights[0].intensity, 0.1f);
		ImGui::ColorEdit3("Point Light Color 1", &pntLights[0].color.r);
		ImGui::ColorEdit3("Point Light Color 2", &pntLights[1].color.r);
		ImGui::DragFloat3("Point Light Position 1", &pntLightTransforms[0].position.x, 0.1f);
		ImGui::DragFloat3("Point Light Position 2", &pntLightTransforms[1].position.x, 0.1f);
		ImGui::DragFloat("Point Light Intensity 1", &pntLights[0].intensity, 0.1f);
		ImGui::DragFloat("Point Light Intensity 2", &pntLights[1].intensity, 0.1f);
		ImGui::DragFloat("Ambient Coefficient", &ambCoefficient, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Diffuse Coefficient", &difCoefficient, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Specular Coefficient", &specCoefficient, 0.01f, 0.0f, 1.0f);
		ImGui::DragInt("Shininess", &shininess, 2, 2, 512);

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwPollEvents();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
//Author: Eric Winebrenner
void resizeFrameBufferCallback(GLFWwindow* window, int width, int height)
{
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	camera.setAspectRatio((float)SCREEN_WIDTH / SCREEN_HEIGHT);
	glViewport(0, 0, width, height);
}
//Author: Eric Winebrenner
void keyboardCallback(GLFWwindow* window, int keycode, int scancode, int action, int mods)
{
	if (keycode == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	//Reset camera
	if (keycode == GLFW_KEY_R && action == GLFW_PRESS) {
		camera.setPosition(glm::vec3(0, 0, 5));
		camera.setYaw(-90.0f);
		camera.setPitch(0.0f);
		firstMouseInput = false;
	}
	if (keycode == GLFW_KEY_1 && action == GLFW_PRESS) {
		wireFrame = !wireFrame;
		glPolygonMode(GL_FRONT_AND_BACK, wireFrame ? GL_LINE : GL_FILL);
	}
}
//Author: Eric Winebrenner
void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (abs(yoffset) > 0) {
		float fov = camera.getFov() - (float)yoffset * CAMERA_ZOOM_SPEED;
		camera.setFov(fov);
	}
}
//Author: Eric Winebrenner
void mousePosCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED) {
		return;
	}
	if (!firstMouseInput) {
		prevMouseX = xpos;
		prevMouseY = ypos;
		firstMouseInput = true;
	}
	float yaw = camera.getYaw() + (float)(xpos - prevMouseX) * MOUSE_SENSITIVITY;
	camera.setYaw(yaw);
	float pitch = camera.getPitch() - (float)(ypos - prevMouseY) * MOUSE_SENSITIVITY;
	pitch = glm::clamp(pitch, -89.9f, 89.9f);
	camera.setPitch(pitch);
	prevMouseX = xpos;
	prevMouseY = ypos;
}
//Author: Eric Winebrenner
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	//Toggle cursor lock
	if (button == MOUSE_TOGGLE_BUTTON && action == GLFW_PRESS) {
		int inputMode = glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
		glfwSetInputMode(window, GLFW_CURSOR, inputMode);
		glfwGetCursorPos(window, &prevMouseX, &prevMouseY);
	}
}

//Author: Eric Winebrenner
//Returns -1, 0, or 1 depending on keys held
float getAxis(GLFWwindow* window, int positiveKey, int negativeKey) {
	float axis = 0.0f;
	if (glfwGetKey(window, positiveKey)) {
		axis++;
	}
	if (glfwGetKey(window, negativeKey)) {
		axis--;
	}
	return axis;
}

//Author: Eric Winebrenner
//Get input every frame
void processInput(GLFWwindow* window) {

	float moveAmnt = CAMERA_MOVE_SPEED * deltaTime;

	//Get camera vectors
	glm::vec3 forward = camera.getForward();
	glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0,1,0)));
	glm::vec3 up = glm::normalize(glm::cross(forward, right));

	glm::vec3 position = camera.getPosition();
	position += forward * getAxis(window, GLFW_KEY_W, GLFW_KEY_S) * moveAmnt;
	position += right * getAxis(window, GLFW_KEY_D, GLFW_KEY_A) * moveAmnt;
	position += up * getAxis(window, GLFW_KEY_Q, GLFW_KEY_E) * moveAmnt;
	camera.setPosition(position);
}
