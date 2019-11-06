#version 430
out vec4 FragColor;
in vec2 TexCoord1;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
void main()
{
    vec3 objClr=mix(texture(ourTexture1, TexCoord1),texture(ourTexture2, TexCoord1),0.3).rgb;
    FragColor = vec4(objClr,1.0);
    //FragColor = texture(ourTexture2, TexCoord1);
}
