//Version 330 de GLSL correponde a 3.3 en OpenGL
#version 330 core
//Indicar los atributos de vertice entrantes:
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

//Matriz de transformación
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    Normal = mat3( transpose(inverse(model))) * aNormal;
    //Enviar al fragment shader la posición del fragmento en el mundo
    FragPos = vec3(model * vec4(aPos, 1.0f));
}