#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/CommonValues.h"

#include "include/Window.h"
#include "include/Mesh.h"
#include "include/Shader.h"
#include "include/Camera.h"
#include "include/Texture.h"
#include "include/DirectionalLight.h"
#include "include/PointLight.h"
#include "include/SpotLight.h"
#include "include/Material.h"

#include "include/Model.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Shader directionalShadowShader;

Model bishop, king, queen, rook, knight, pawn;
Material shinyMaterial, dullMaterial;	

Camera camera;

Texture brickTexture;
Texture marbleTexture;
Texture plainTexture;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

unsigned int pointLightCount = 0, spotLightCount = 0;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;

// Vertex Shader
// Make small s
static const char* vShader = "shaders/shader.vert";

// Fragment Shader
static const char* fShader = "shaders/shader.frag";

void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, 
						unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);
		
		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects() 
{
	unsigned int indices[] = {		
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
	    // x     y      z	  u	    v	  nx	ny    nz
		-1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, -1.0f,  1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -0.6f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 0.0f,  1.0f,  0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f, 0.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f, -10.0f, 1.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f,  10.0f, 0.0f, 1.0f,	0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f,  10.0f, 1.0f, 1.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int backWallIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat backWallVertices[] = {
		-10.0f,  0.0f, -10.0f, 0.0f, 0.0f,	0.0f, 0.0f, -1.0f,
		 10.0f,  0.0f, -10.0f, 1.0f, 0.0f,	0.0f, 0.0f, -1.0f,
		-10.0f, 10.0f, -10.0f, 0.0f, 1.0f,	0.0f, 0.0f, -1.0f,
		 10.0f, 10.0f, -10.0f, 1.0f, 1.0f,	0.0f, 0.0f, -1.0f
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(backWallVertices, backWallIndices, 32, 6);
	meshList.push_back(obj4);
}

void TransformAndRenderModel(Model* m, Material* mat, GLfloat transX, GLfloat transY, GLfloat transZ, GLfloat scale, GLfloat rotX, GLfloat rotY, GLfloat rotZ)
{
	// First translate, rotate, then scale so that it's executed as scale, rotate, translate
	glm::mat4 model = glm::mat4();
	model = glm::translate(model, glm::vec3(transX, transY, transZ));
	model = glm::rotate(model, rotX * toRadians, glm::vec3(1, 0, 0));
	model = glm::rotate(model, rotY * toRadians, glm::vec3(0, 1, 0));
	model = glm::rotate(model, rotZ * toRadians, glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale, scale, scale));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	mat->UseMaterial(uniformSpecularIntensity, uniformShininess);
	m->RenderModel();
}

void TransformAndRenderMesh(Mesh* m, Material* mat, GLfloat transX, GLfloat transY, GLfloat transZ, GLfloat scale, GLfloat rotX, GLfloat rotY, GLfloat rotZ)
{
	glm::mat4 model = glm::mat4();
	model = glm::translate(model, glm::vec3(transX, transY, transZ));
	model = glm::rotate(model, rotX * toRadians, glm::vec3(1, 0, 0));
	model = glm::rotate(model, rotY * toRadians, glm::vec3(0, 1, 0));
	model = glm::rotate(model, rotZ * toRadians, glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(scale, scale, scale));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	marbleTexture.UseTexture();
	mat->UseMaterial(uniformSpecularIntensity, uniformShininess);
	m->RenderMesh();
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
	directionalShadowShader.CreateFromFiles("shaders/directionalShadowMap.vert", "shaders/directionalShadowMap.frag");
}

void RenderScene()
{
	glm::mat4 model;	

	TransformAndRenderMesh(meshList[2], &dullMaterial, 0.0f, -2.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	TransformAndRenderMesh(meshList[3], &dullMaterial, 0.0f, -2.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	TransformAndRenderMesh(meshList[3], &dullMaterial, 0.0f, -2.0f, 0.0f, 1.0f, 0.0f, -90.0f, 0.0f);
	TransformAndRenderMesh(meshList[3], &dullMaterial, 0.0f, -2.0f, 0.0f, 1.0f, 0.0f, 90.0f, 0.0f);

	TransformAndRenderModel(&rook, &shinyMaterial, 3.0f, -2.0f, -1.0f, .3f, -90.0f, 0.0f, 180.0f);
	TransformAndRenderModel(&knight, &shinyMaterial, 2.5f, -2.0f, -1.0f, .3f, -90.0f, 0.0f, 180.0f);
	TransformAndRenderModel(&bishop, &shinyMaterial, 2.0f, -2.0f, -1.0f, .3f, -90.0f, 0.0f, 180.0f);
	TransformAndRenderModel(&queen, &shinyMaterial, -2.0f, -2.0f, -1.0f, .3f, -90.0f, 0.0f, 180.0f);
	TransformAndRenderModel(&king, &shinyMaterial, 2.5f, -2.0f, -1.0f, .3f, -90.0f, 0.0f, 180.0f);
	TransformAndRenderModel(&bishop, &shinyMaterial, -6.0f, -2.0f, -1.0f, .3f, -90.0f, 0.0f, 180.0f);
	TransformAndRenderModel(&knight, &shinyMaterial, -10.0f, -2.0f, -1.0f, .3f, -90.0f, 0.0f, 180.0f);
	TransformAndRenderModel(&rook, &shinyMaterial, -14.0f, -2.0f, -1.0f, .3f, -90.0f, 0.0f, 180.0f);

	TransformAndRenderModel(&pawn, &shinyMaterial, 3.0f, -2.0f, 0.0f, .3f, -90.0f, 0.0f, 180.0f);
	TransformAndRenderModel(&pawn, &shinyMaterial, 1.0f, -2.0f, 0.0f, .3f, -90.0f, 0.0f, 180.0f);
	TransformAndRenderModel(&pawn, &shinyMaterial, -1.5f, -2.0f, 0.0f, .3f, -90.0f, 0.0f, 180.0f);
	TransformAndRenderModel(&pawn, &shinyMaterial, -4.0f, -2.0f, 0.0f, .3f, -90.0f, 0.0f, 180.0f);	
	TransformAndRenderModel(&pawn, &shinyMaterial, -6.7f, -2.0f, 0.0f, .3f, -90.0f, 0.0f, 180.0f);
	TransformAndRenderModel(&pawn, &shinyMaterial, -9.0f, 0.0f, 3.0f, .3f, -90.0f, 0.0f, 180.0f);
	TransformAndRenderModel(&pawn, &shinyMaterial, -11.5f, -2.0f, 0.0f, .3f, -90.0f, 0.0f, 180.0f);
	TransformAndRenderModel(&pawn, &shinyMaterial, -14.0f, -2.0f, 0.0f, .3f, -90.0f, 0.0f, 180.0f);
}


void DirectionalShadowMapPass(DirectionalLight* light)
{
	directionalShadowShader.UseShader();

	glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

	light->GetShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.GetModelLocation();
	glm::mat4 temp = light->CalculateLightTransform();
	directionalShadowShader.SetDirectionalLightTransform(&temp);

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	shaderList[0].UseShader();

	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformModel = shaderList[0].GetModelLocation();
	uniformEyePosition = shaderList[0].GetEyePositionLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformShininess = shaderList[0].GetShininessLocation();

	glViewport(0, 0, mainWindow.getWidth(), mainWindow.getHeight());

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

	shaderList[0].SetDirectionalLight(&mainLight);
	shaderList[0].SetPointLights(pointLights, pointLightCount);
	shaderList[0].SetSpotLights(spotLights, spotLightCount);
	glm::mat4 temp = mainLight.CalculateLightTransform();
	shaderList[0].SetDirectionalLightTransform(&temp);

	mainLight.GetShadowMap()->Read(GL_TEXTURE1);
	shaderList[0].SetTexture(0);
	shaderList[0].SetDirectionalShadowMap(1);

	glm::vec3 lowerLight = camera.getCameraPosition();
	lowerLight.y -= 0.3f;
	//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

	RenderScene();
}

int main() 
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	brickTexture = Texture("textures/brick.png");
	brickTexture.LoadTextureA();
	marbleTexture  = Texture("textures/marble.jpg");
	marbleTexture.LoadTexture();
	plainTexture = Texture("textures/plain.png");
	plainTexture.LoadTextureA();

	shinyMaterial = Material(4.0f, 256);
	dullMaterial = Material(0.3f, 4);

	bishop = Model();
	bishop.LoadModel("models/bishop.obj");

	king = Model();
	king.LoadModel("models/king.obj");

	queen = Model();
	queen.LoadModel("models/queen.obj");

	rook = Model();
	rook.LoadModel("models/rook.obj");

	knight = Model();
	knight.LoadModel("models/knight.obj");

	pawn = Model();
	pawn.LoadModel("models/pawn.obj");
	
	

	mainLight = DirectionalLight(2048, 2048,
								1.0f, 1.0f, 1.0f, 
								0.1f, 0.3f,
								0.0f, -15.0f, -10.0f);

	pointLights[0] = PointLight(1.0f, 1.0f, .8f,
								0.0f, 0.1f,
								0.0f, 0.0f, 0.0f,
								0.3f, 0.2f, 0.1f);
	pointLightCount++;
	pointLights[1] = PointLight(1.0f, 1.0f, 0.8f,
								0.0f, 0.1f,
								-4.0f, 2.0f, 0.0f,
								0.3f, 0.1f, 0.1f);
	pointLightCount++;

	spotLights[0] = SpotLight(1.0f, 1.0f, .8f,
							  -.0f, 1.0f,
							  -1.0f, 5.0f, -7.0f,
							  0.0f, -1.0f, 0.0f,
							  1.0f, 0.0f, 0.0f,
							  40.0f);
	spotLightCount++;
	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
							  0.0f, 0.5f,
							  0.0f, -1.5f, 0.0f,
							  -10.0f, 2.0f, -7.0f,
							  1.0f, 0.0f, 0.0f,
							  30.0f);
	spotLightCount++;
	spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f,
							  0.0f, 0.5f,
							  0.0f, 1.5f, 0.0f,
							  10.0f, -1.0f, -8.0f,
							  1.0f, 0.0f, 0.0f,
							  30.0f);
	spotLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;

		// Get + Handle User Input
		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		DirectionalShadowMapPass(&mainLight);
		RenderPass(camera.calculateViewMatrix(), projection);

		mainWindow.swapBuffers();
	}

	return 0;
}
