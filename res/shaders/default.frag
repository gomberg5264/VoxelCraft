#version 330 core

in vec2 uv; 
out vec4 color;

uniform sampler2D tex;

void main()
{
    color = texture(tex,uv);
    // color = vec4(vec3(debug / 5),1);
}