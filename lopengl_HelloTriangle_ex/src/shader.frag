#version 330 core
//Indicar que variable se usará como output del Fragment Shader
out vec4 FragColor;

void main()
{   //Para esta primera prueba siempre el output es un color naranja
    //Color:          R      G     B   Alpha
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}