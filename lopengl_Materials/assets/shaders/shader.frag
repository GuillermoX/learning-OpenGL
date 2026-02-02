#version 330 core

//Material struct
struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

//Light structu (to reduce or increase the lighting types)
struct Light {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

//Indicar que variable se usará como output del Fragment Shader
out vec4 FragColor;

//Indica que recibe como input (probablemente del Vertex Shader) un Vec3 llamado ourColor
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

uniform sampler2D texture1;   //Tipo de dato textura 2D que nos indicará la CPU
uniform sampler2D texture2;
uniform float tex_interpol;


void main()
{   
  //Ambient lighting
  vec3 ambient = light.ambient * material.ambient;

  //Diffuse lighting
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(light.position - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);   //Evita negativos con el max
  vec3 diffuse = light.diffuse * (diff * material.diffuse);

  //Specular lighting
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);   //Es necesario que primer param sea rayo incidente
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * (spec * material.specular);

  vec3 resultColor = ambient + diffuse + specular;
  FragColor = vec4(resultColor, 1.0f);
}