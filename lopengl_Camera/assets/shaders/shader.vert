//Version 330 de GLSL correponde a 3.3 en OpenGL
#version 330 core
//Indicar los atributos de vertice entrantes:
// por el momento solo tenemos las 3 coordenadas del espacio
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

//Matriz de transformación
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}