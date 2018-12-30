# Sample-GLSL
This is a starter C++ app for GLSL.

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
g++ main.cpp src/Mesh.cpp src/Shader.cpp src/Window.cpp src/Camera.cpp src/Texture.cpp src/Light.cpp src/Material.cpp src/DirectionalLight.cpp src/PointLight.cpp src/PointLight.cpp-w -lGL -lglfw -lGLEW -o p

./exec
```
