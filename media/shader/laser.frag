#version 430
out vec4 FragColor;
in  float clr;

void main()
{
    vec3 objClr=vec3(1.0,0.8,3*(1-clr)*(1-clr));
    FragColor = vec4(0.541,0.99,0.584,1.0f);
}
