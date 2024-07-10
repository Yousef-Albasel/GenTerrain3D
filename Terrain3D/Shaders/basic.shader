#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
    
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float minHeight;
uniform float maxHeight;

out float HeightRatio;

void main()
{
    float DeltaHeight = maxHeight - minHeight;
    HeightRatio = (aPos.y - minHeight) / DeltaHeight;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment
#version 330 core
in float HeightRatio;
out vec4 FragColor;

void main()
{
    float c = HeightRatio * 0.8 + 0.2;
    FragColor = vec4(c, c, c, 1.f);
}
