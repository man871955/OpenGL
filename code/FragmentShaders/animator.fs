#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform vec4 lightColor;
uniform sampler2D tex;

void main()
{    
    FragColor = lightColor*texture(tex, TexCoords);
}