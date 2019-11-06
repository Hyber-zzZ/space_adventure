#version 430
layout (location = 0) in vec4 position_clr;
layout (location = 1) in mat4 model;
out float clr;
uniform mat4 view;
uniform mat4 projection;
void main(){
    gl_Position=projection*view*model*vec4(position_clr.xyz,1.0f);
    clr=position_clr.w;
}
