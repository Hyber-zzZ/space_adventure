#version 430
layout(location=0)in vec3 position;
layout(location=1)in vec2 aTexCoord1;
layout(location=3)in mat4 model;
layout(location=7)in float _hit;
out vec2 TexCoord1;
out flat float hit;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 modeluni;
void main(){
    gl_Position=projection*view*model*vec4(position,1.0f);
    TexCoord1 = aTexCoord1;
    hit=_hit;
}
