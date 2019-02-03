#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <iostream>
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

#include "include/Scene.h"

using namespace std;

const float toRadians = 3.14159265f / 180.0f;

int main() 
{
	Scene scene = Scene();
	while (!scene.mainWindow.getShouldClose())
	{
		scene.Render();
	}
	return 0;
}
