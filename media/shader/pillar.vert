#version 430
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 aTexCoord1;
layout (location = 2) in vec3 aNormal;
out vec2 TexCoord1;
out flat int  isMain;
out vec3 FragPos;
out vec3 Normal;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
void main(){
    gl_Position=projection*view*model*vec4(position,1.0f);
    TexCoord1 = aTexCoord1;
	if(gl_InstanceID==0) isMain=1;
	else isMain=0;
    FragPos = vec3(model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
}
