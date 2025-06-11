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
out vec4 ViewPosition; 

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    gl_Position = projection * view * worldPos;
    
    ViewPosition = view * worldPos;
    
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
in vec3 FragPos;
in vec4 ViewPosition; // Receive the view-space position

uniform sampler2D texture1; // Texture for low height
uniform sampler2D texture2; // Texture for mid-low height
uniform sampler2D texture3; // Texture for mid-high height
uniform sampler2D texture4; // Texture for high height
uniform vec3 gReversedLightDir;

// Fog parameters
uniform vec3 fogColor = vec3(0.75, 0.85, 0.9); // Light blue-gray color
uniform float fogStart = 300.0;
uniform float fogEnd = 1500.0;
uniform int fogEquation = 0; // 0 = linear, 1 = exponential, 2 = exponential squared

void main()
{
    float height = clamp(HeightRatio, 0.0, 1.0);
    
    vec4 color4 = texture(texture1, v_TexCoord); // Grass
    vec4 color3 = texture(texture2, v_TexCoord); // Dirt
    vec4 color2 = texture(texture3, v_TexCoord); // Rock
    vec4 color1 = texture(texture4, v_TexCoord); // Snow
    float blend1 = smoothstep(0.0, 0.3, height);
    float blend2 = smoothstep(0.3, 0.5, height);
    float blend3 = smoothstep(0.5, 1.0, height);
    
    vec4 blendedColor = mix(color4, color2, blend1);       // Grass to Dirt
    blendedColor = mix(blendedColor, color3, blend2);      // Dirt to Rock
    blendedColor = mix(blendedColor, color4, blend3);      // Rock to Snow
    
    // Lighting calculation
    vec3 normal = normalize(Normal);
    float diffuse = max(dot(normal, gReversedLightDir), 0.3);
    
    // Apply lighting to the texture color
    vec4 finalColor = blendedColor * diffuse;
    
    // Calculate fog factor
    float dist = length(ViewPosition.xyz);
    float fogFactor = 1.0;
    
    if (fogEquation == 0) {
        fogFactor = clamp((fogEnd - dist) / (fogEnd - fogStart), 0.0, 1.0);
    } else if (fogEquation == 1) {
        const float fogDensity = 0.003;
        fogFactor = clamp(exp(-fogDensity * dist), 0.0, 1.0);
    } else if (fogEquation == 2) {
        const float fogDensity = 0.0005;
        fogFactor = clamp(exp(-fogDensity * fogDensity * dist * dist), 0.0, 1.0);
    }
    
    FragColor = mix(vec4(fogColor, 1.0), finalColor, fogFactor);
}