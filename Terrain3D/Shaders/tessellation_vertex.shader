#shader vertex
#version 400 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 InNormal;

out vec2 vs_texCoord;

void main()
{
    // For tessellation pipeline, we need to output position
    // Pass through the vertex position (will be interpolated in tessellation stages)
    gl_Position = vec4(aPos, 1.0);
    vs_texCoord = texCoord;
}

#shader fragment
#version 400 core
out vec4 FragColor;

in vec3 tes_worldPos;
in vec2 tes_texCoord;
in vec3 tes_normal;
in float HeightRatio;
in vec4 ViewPosition;

uniform sampler2D texture1; // Texture for low height
uniform sampler2D texture2; // Texture for mid-low height
uniform sampler2D texture3; // Texture for mid-high height
uniform sampler2D texture4; // Texture for high height
uniform vec3 gReversedLightDir;

// Fog parameters
uniform vec3 fogColor = vec3(0.75, 0.85, 0.9);
uniform float fogStart = 300.0;
uniform float fogEnd = 1500.0;
uniform int fogEquation = 0;

void main()
{
    float height = clamp(HeightRatio, 0.0, 1.0);
    
    vec4 color4 = texture(texture1, tes_texCoord * 32.0); // Grass - tiled
    vec4 color3 = texture(texture2, tes_texCoord * 32.0); // Dirt - tiled
    vec4 color2 = texture(texture3, tes_texCoord * 32.0); // Rock - tiled
    vec4 color1 = texture(texture4, tes_texCoord * 32.0); // Snow - tiled
    
    float blend1 = smoothstep(0.0, 0.3, height);
    float blend2 = smoothstep(0.3, 0.5, height);
    float blend3 = smoothstep(0.5, 1.0, height);
    
    vec4 blendedColor = mix(color4, color2, blend1);       // Grass to Dirt
    blendedColor = mix(blendedColor, color3, blend2);      // Dirt to Rock
    blendedColor = mix(blendedColor, color1, blend3);      // Rock to Snow
    
    // Lighting calculation
    vec3 normal = normalize(tes_normal);
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