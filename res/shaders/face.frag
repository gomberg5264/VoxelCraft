#version 450 core

in vec2 uv; 
in flat float texIndex; 

out vec4 color;

uniform sampler2DArray sampl;

void main()
{
    // color = vec4(1);
    color = texture(sampl,vec3(uv,texIndex));
    // color = vec4(vec3(float(texIndex) / 2.f),1);
    //color = vec4(tex / 5.f,1);
}