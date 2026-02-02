#version 330 core
//Indicar que variable se usará como output del Fragment Shader
out vec4 FragColor;

//Indica que recibe como input (probablemente del Vertex Shader) un Vec3 llamado ourColor
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

//Indica el uso de una variable global
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;

uniform sampler2D texture1;   //Tipo de dato textura 2D que nos indicará la CPU
uniform sampler2D texture2;
uniform float tex_interpol;
uniform vec3 lightPos;


void main()
{   
  //Ambient lighting
  float ambientStrenght = 0.1;
  vec3 ambient = ambientStrenght * lightColor;

  //Diffuse lighting
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);

  float diff = max(dot(norm, lightDir), 0.0);   //Evita negativos con el max
  vec3 diffuse = diff * lightColor;

  //Specular lighting
  float specularStrengh = 0.5;
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrengh * spec *lightColor;

  vec3 resultColor = (ambient + diffuse + specular) * objectColor;
  FragColor = vec4(resultColor, 1.0f);
}