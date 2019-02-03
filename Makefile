all:
	g++ -std=c++11 main.cpp src/Scene.cpp src/Mesh.cpp src/Shader.cpp src/Window.cpp src/Camera.cpp src/Texture.cpp src/Light.cpp src/Material.cpp src/DirectionalLight.cpp src/PointLight.cpp src/SpotLight.cpp src/Model.cpp src/ShadowMap.cpp -w -lGL -lglfw -lGLEW -lassimp -o p
