# GLSL Starter
A starter OpenGL shader application written in C++ for Linux. It includes directional, point and spot lights, simulates ambient, diffuse and specular lighting as well as a shadow map with percentage closer filtering (PCF). It supports 3D model loading via Assimp and texture loading via STB Image.

<p align="left">
<img src="https://github.com/acvictor/GLSL-Starter/blob/master/images/ShadowMap2.png" alt="NMPC" width="420" height="230" border="20" /></a> 
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
git clone https://github.com/assimp/assimp.git
cd assimp
cmake CMakeLists.txt
make -j4
sudo make install
export LD_LIBRARY_PATH=/usr/local/lib
```

## Execution
To compile and execute run
```
g++ -std=c++11 main.cpp src/Mesh.cpp src/Shader.cpp src/Window.cpp src/Camera.cpp src/Texture.cpp src/Light.cpp src/Material.cpp src/DirectionalLight.cpp src/PointLight.cpp src/SpotLight.cpp src/Model.cpp src/ShadowMap.cpp -w -lGL -lglfw -lGLEW -lassimp -o exec

./exec
```
