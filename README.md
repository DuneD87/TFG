
# Tutorial OpenGL
Instalar glad, glfw, assimp i rapidxml: \
sudo apt install -y xorg-dev libgl1-mesa-dev libglu-dev libassimp-dev \
git clone https://github.com/Dav1dde/glad.git \
cd glad \
cmake ./ \
sudo make install \
sudo mkdir /usr/local/include/glad \
sudo cp glad.h /usr/local/include/glad \
sudo cp khrplatform.h /usr/local/include/glad \
sudo apt-get install libglfw3-dev \
sudo apt-get install libassimp-dev \
sudo apt install librapidxml-dev -y \
git clone https://github.com/DanielChappuis/reactphysics3d.git \
cd reactphysics3d \
cmake ./ \
sudo make install \
