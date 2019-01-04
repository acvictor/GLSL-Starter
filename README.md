# GLSL Starter
This is a starter OpenGL shader application written in C++. It includes directional, point and spot lights, simulates ambient, diffuse and specular lighting as well as a shadow map. It supports 3D model loading via Assimp and texture loading via STB Image.

<p align="center">
<img src="https://github.com/acvictor/GLSL-Starter/blob/master/images/NoShadows.png" alt="NMPC" width="420" height="230" border="20" /></a> 
<img src="https://github.com/acvictor/GLSL-Starter/blob/master/images/ShadowMap.png" alt="NMPC" width="420" height="230" border="20" /></a>
</p>


## Requirements
- OpenGL 3.3 and above
- GLEW
- GLFW
- GLM
- STB Image
- Assimp

To install 
```
sudo apt install mesa-utils mesa-common-dev
sudo apt-get install libglew-dev
sudo apt-get install libglfw3-dev
sudo apt-get install libglm-dev
git clone https://github.com/nothings/stb.git
sudo cp stb/stb_image.h /usr/include/
sudo apt-get install libassimp-dev
```

## Execution
To compile and execute run
```
g++ -std=c++11 main.cpp src/Mesh.cpp src/Shader.cpp src/Window.cpp src/Camera.cpp src/Texture.cpp src/Light.cpp src/Material.cpp src/DirectionalLight.cpp src/PointLight.cpp src/SpotLight.cpp src/Model.cpp -w -lGL -lglfw -lGLEW -lassimp -o exec

./exec
```
