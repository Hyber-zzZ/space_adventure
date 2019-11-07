#version 430
out vec4 FragColor;
in vec2 TexCoord1;
in flat float hit;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
void main()
{
    vec3 objClr=mix(texture(ourTexture1, TexCoord1),texture(ourTexture2, TexCoord1),0.3).rgb;
    if(hit==1)objClr=objClr+vec3(0.6,0.0,0.0);
    FragColor = vec4(objClr,1.0);
    //FragColor = texture(ourTexture2, TexCoord1);
}
