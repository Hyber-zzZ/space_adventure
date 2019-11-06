#version 430
out vec4 FragColor;
in vec2 TexCoord1;
in  flat int isMain;
in vec3 Normal;  
in vec3 FragPos;
uniform vec3 lightClr;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D text;
void main()
{
    //vec3 objClr=texture(text,TexCoord1).rgb;
    vec3 objClr=vec3(0.5,0.3,0.5);
	float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightClr;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightClr;

     float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 specular = specularStrength * spec * lightClr;  

	vec3 result = (ambient + diffuse) * objClr;
	FragColor = vec4(result, 1.0f);
}
