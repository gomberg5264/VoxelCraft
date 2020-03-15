#version 450 core

in vec2 uv; 
in int texIndex; 

out vec4 color;

uniform sampler2DArray tex;

void main()
{
    color = texture(tex,vec3(uv,texIndex);
    // color = vec4(vec3(debug / 5),1);
}