#version 330 core

//Material struct
struct Material {
  sampler2D diffuse;  //La textura será el diffuse y ambient lighting
                      //sampler 2D es un opaque type así que solo se puede usar como uniform
  sampler2D specular;
  float shininess;
};

//Light structu (to reduce or increase the lighting types)
struct Light {
  vec3 position;
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  // Spot light cutOff value 
  float cutOff;
  float outerCutOff;

  // Attenuation coefficients
  float constant;
  float linear;
  float quadratic;
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


void main()
{   
  //Ambient lighting
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));


  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(light.position - FragPos);


  //Diffuse lighting
  float diff = max(dot(norm, lightDir), 0.0);   //Evita negativos con el max
  vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoord)));

  //Specular lighting
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);   //Es necesario que primer param sea rayo incidente
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoord)));

  // Attenuance value
  float distance = length(light.position - FragPos);
  float attenuance = 1.0 / (light.constant + light.linear*distance + light.quadratic*(distance*distance));
  ambient *= attenuance;
  diffuse *= attenuance;
  specular *= attenuance;

  // Spot light theta angle cosine calculation and intensity calculation
  float theta = dot(-lightDir, normalize(light.direction));
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff)/epsilon, 0.0, 1.0);

  diffuse *= intensity;
  specular *= intensity;
  ambient *= intensity;
  
  // Final result
  vec3 resultColor = ambient + diffuse + specular;

  FragColor = vec4(resultColor, 1.0f);
}
