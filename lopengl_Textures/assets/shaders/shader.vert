//Version 330 de GLSL correponde a 3.3 en OpenGL
#version 330 core
//Indicar los atributos de vertice entrantes:
// por el momento solo tenemos las 3 coordenadas del espacio
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

//Indica que tiene como output un Vec3 llamado ourColor que probablemente use el Fragment Shader
out vec3 ourColor;
out vec2 TexCoord;

uniform vec2 translation;

void main()
{
    //El último valor que tome gl_Position será el que
    //salga como output del vertex shader
    gl_Position = vec4(aPos.x+translation.x, aPos.y+translation.y, aPos.z, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
}