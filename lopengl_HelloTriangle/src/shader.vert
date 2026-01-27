// -- Copia del vertex shader utilizado en main.cpp en forma
//    de string constante

//Version 330 de GLSL correponde a 3.3 en OpenGL
#version 330 core   //Indicamos que estamos en core-profile
//Indicar los atributos de vertice entrantes:
// por el momento solo tenemos las 3 coordenadas del espacio
layout (location = 0) in vec3 aPos;

void main()
{
    //El último valor que tome gl_Position será el que
    //salga como output del vertex shader
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}