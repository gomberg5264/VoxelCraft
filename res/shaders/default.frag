#version 330 core

in vec2 uv; 
out vec4 color;

uniform sampler2DArray tex;

void main()
{
    color = texture(tex,vec3(uv,0));
    // color = vec4(vec3(debug / 5),1);
}