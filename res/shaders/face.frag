#version 450 core

in vec3 normal;
in vec2 uv; 
in flat float texIndex; 
in vec3 skyLightDir; 
in vec3 skyLightColor; 

out vec4 color;

uniform sampler2DArray sampl;

void main()
{
    // Blinn shading
    
    color = texture(sampl,vec3(uv,texIndex));
    
    
    //color = vec4(1);
    // color = vec4(vec3(float(texIndex) / 2.f),1);
    //color = vec4(tex / 5.f,1);
}