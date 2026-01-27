# OpenGL Learning Project

Projects following the tutorials from [learnopengl.com](https://learnopengl.com/)

## 📁 Project Structure

### Common/
Shared code between all projects (shaders, glad, utilities).

### lopengl_HelloWindow
**[Creating a Window](https://learnopengl.com/Getting-started/Hello-Window)**  
Basic GLFW setup and OpenGL window creation.

<!-- ![HelloWindow Screenshot](IMAGE_URL) -->

### lopengl_HelloTriangle
**[Hello Triangle](https://learnopengl.com/Getting-started/Hello-Triangle)**  
First triangle using VAO, VBO and basic shaders.

<!-- ![HelloTriangle Screenshot](IMAGE_URL) -->

### lopengl_HelloTriangle_ex
**[Hello Triangle - Exercises](https://learnopengl.com/Getting-started/Hello-Triangle)**  
Proposed exercises from the Hello Triangle chapter.

<!-- ![HelloTriangle Exercise Screenshot](IMAGE_URL) -->

### lopengl_HelloRectangle
**[Hello Triangle](https://learnopengl.com/Getting-started/Hello-Triangle)**  
Rectangle rendering using EBO (Element Buffer Object).

<!-- ![HelloRectangle Screenshot](IMAGE_URL) -->

### lopengl_Shaders
**[Shaders](https://learnopengl.com/Getting-started/Shaders)**  
Working with shaders, uniforms and attributes.

<!-- ![Shaders Screenshot](IMAGE_URL) -->

### lopengl_Textures
**[Textures](https://learnopengl.com/Getting-started/Textures)**  
Loading and applying textures using stb_image.

<!-- ![Textures Screenshot](IMAGE_URL) -->

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
