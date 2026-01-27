#version 330 core
//Indicar que variable se usará como output del Fragment Shader
out vec4 FragColor;

//Indica que recibe como input (probablemente del Vertex Shader) un Vec3 llamado ourColor
in vec3 ourColor;

//Indica el uso de una variable global
uniform float darkness;


void main()
{   //Para esta primera prueba siempre el output es un color naranja
    //Color:          R      G     B   Alpha
    FragColor = vec4(ourColor*darkness, 1.0f);
}