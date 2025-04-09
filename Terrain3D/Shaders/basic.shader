#shader vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 InNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float minHeight;
uniform float maxHeight;

out float HeightRatio;  
out vec2 v_TexCoord;
out vec3 Normal;
out vec3 FragPos; 
void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    gl_Position = projection * view * worldPos;
    
    Normal = mat3(transpose(inverse(model))) * InNormal;
    
    float DeltaHeight = maxHeight - minHeight;
    HeightRatio = (aPos.y - minHeight) / DeltaHeight;
    v_TexCoord = texCoord;
}

#shader fragment
#version 330 core

out vec4 FragColor;
in float HeightRatio;
in vec2 v_TexCoord;
in vec3 Normal;

uniform sampler2D texture1; // Texture for low height (e.g., grass)
uniform sampler2D texture2; // Texture for mid-low height (e.g., dirt)
uniform sampler2D texture3; // Texture for mid-high height (e.g., rock)
uniform sampler2D texture4; // Texture for high height (e.g., snow)

uniform vec3 gReversedLightDir;
void main()
{
    // Clamp HeightRatio to avoid artifacts
    float height = clamp(HeightRatio, 0.0, 1.0);

 // Texture blending based on height
    vec4 color4 = texture(texture1, v_TexCoord); // Grass
    vec4 color3 = texture(texture2, v_TexCoord); // Dirt
    vec4 color2 = texture(texture3, v_TexCoord); // Rock
    vec4 color1 = texture(texture4, v_TexCoord); // Snow
    // Smooth blending weights
    float blend1 = smoothstep(0.0, 0.3, height);
    float blend2 = smoothstep(0.3, 0.6, height);
    float blend3 = smoothstep(0.6, 0.9, height);
    vec4 blendedColor = mix(color1, color2, blend1);           // Grass to Dirt
    blendedColor = mix(blendedColor, color3, blend2);         // Dirt to Rock
    blendedColor = mix(blendedColor, color4, blend3);         // Rock to Snow


    // Lighting calculation
    vec3 normal = normalize(Normal);
    float diffuse = max(dot(normal, gReversedLightDir), 0.3); // Using 0.2 as ambient light
    
    // Apply lighting to the texture color
    FragColor = blendedColor * diffuse;
}
