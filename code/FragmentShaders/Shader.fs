#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform vec4 color;
uniform sampler2D texture_diffuse;


void main()
{    
    FragColor = color * texture(texture_diffuse, TexCoords);
    if(FragColor.a <= 0.75)
        discard;
}