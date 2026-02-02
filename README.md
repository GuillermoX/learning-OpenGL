# OpenGL Learning Project

Projects following the tutorials from [learnopengl.com](https://learnopengl.com/)

## 📁 Project Structure

### Common/
Shared code between all projects (shaders, glad, utilities).

### lopengl_HelloWindow
**(https://learnopengl.com/Getting-started/Hello-Window)**  
Basic GLFW setup and OpenGL window creation.


### lopengl_HelloTriangle
**(https://learnopengl.com/Getting-started/Hello-Triangle)**  
First triangle using VAO, VBO and basic shaders.

![HelloTriangle Screenshot](https://github.com/GuillermoX/GuillermoX/blob/main/.github/img/learning-OpenGL/HelloTriangle.png)

### lopengl_HelloTriangle_ex
**[Hello Triangle - Exercises](https://learnopengl.com/Getting-started/Hello-Triangle)**  
Proposed exercises from the Hello Triangle chapter.

<!-- ![HelloTriangle Exercise Screenshot](IMAGE_URL) -->

### lopengl_HelloRectangle
**(https://learnopengl.com/Getting-started/Hello-Triangle)**  
Rectangle rendering using EBO (Element Buffer Object).

![HelloRectangle Screenshot](https://github.com/GuillermoX/GuillermoX/blob/main/.github/img/learning-OpenGL/HelloRectangle.png)

### lopengl_Shaders
**(https://learnopengl.com/Getting-started/Shaders)**  
Working with shaders, uniforms and attributes.

![Shaders Screenshot](https://github.com/GuillermoX/GuillermoX/blob/main/.github/img/learning-OpenGL/Shaders.png)

### lopengl_Textures
**(https://learnopengl.com/Getting-started/Textures)**  
Loading and applying textures using stb_image.

![Textures Screenshot](https://github.com/GuillermoX/GuillermoX/blob/main/.github/img/learning-OpenGL/Textures.png)

### lopengl_Transformations
**(https://learnopengl.com/Getting-started/Transformations)**  
Applying transformation matrices to the models.

![Transformations Screenshot](https://github.com/GuillermoX/GuillermoX/blob/main/.github/img/learning-OpenGL/Transformations.png)

### lopengl_CoordinateSystems
**(https://learnopengl.com/Getting-started/Coordinate-Systems)**  
Understanding coordinate systems, model, view and projection matrices to achieve 3D illusion.

![CoordinateSystems Screenshot](https://github.com/GuillermoX/GuillermoX/blob/main/.github/img/learning-OpenGL/CoordinateSystems.png)


### lopengl_Camera
**(https://learnopengl.com/Getting-started/Camera)**  
Creating and understanding a flying camera system.

### lopengl_BasicLighting
**(https://learnopengl.com/Lighting/Basic-Lighting)**  
Understanding and implementing the Phong lighting model (Ambient, Diffuse and Specular lighting) using the fragment shader.

![BasicLighting Screenshot](https://github.com/GuillermoX/GuillermoX/blob/main/.github/img/learning-OpenGL/BasicLighting.png)


## 🛠️ Building

### Requirements
- **GLFW** must be installed on your system
- GLAD is already included in the `Common/` folder

### Build Instructions
Each project has its own `CMakeLists.txt`. To build:

```bash
cd lopengl_<project_name>/build
cmake ..
make
./<executable>
```
