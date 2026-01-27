#version 330 core
//Indicar que variable se usará como output del Fragment Shader
out vec4 FragColor;

//Indica que recibe como input (probablemente del Vertex Shader) un Vec3 llamado ourColor
in vec2 TexCoord;

//Indica el uso de una variable global
uniform float darkness;
uniform sampler2D texture1;   //Tipo de dato textura 2D que nos indicará la CPU
uniform sampler2D texture2;
uniform float tex_interpol;


void main()
{   
    /*Usar función "texture" de GLSL para coger el pixel filtrado segun la coordenada
      de textura interpolada*/
    /*Usar mix para interpolar el color de las dos texturas (con 0.2 retorna 80% de color
                                                             textura1 y 20% color textura2)*/
    FragColor = mix(texture(texture1, TexCoord), 
                    texture(texture2, TexCoord), tex_interpol);
                //* darkness;
}