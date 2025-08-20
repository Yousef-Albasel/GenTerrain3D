#version 400 core

layout(quads, equal_spacing, ccw) in;

in vec2 tcs_texCoord[];
out vec3 tes_worldPos;
out vec2 tes_texCoord;
out vec3 tes_normal;
out float HeightRatio;
out vec4 ViewPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform sampler2D heightmapTexture;
uniform float heightScale;
uniform float minHeight;
uniform float maxHeight;

void main() {
    // Bilinear interpolation using gl_TessCoord
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    
    // Interpolate position
    vec3 p0 = mix(gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, u);
    vec3 p1 = mix(gl_in[3].gl_Position.xyz, gl_in[2].gl_Position.xyz, u);
    vec3 position = mix(p0, p1, v);
    
    // Interpolate texture coordinates
    vec2 t0 = mix(tcs_texCoord[0], tcs_texCoord[1], u);
    vec2 t1 = mix(tcs_texCoord[3], tcs_texCoord[2], u);
    vec2 texCoord = mix(t0, t1, v);
    
    // Sample height from heightmap and apply proper scaling
    float heightSample = texture(heightmapTexture, texCoord).r;
    
    // Scale the height properly
    float actualHeight = minHeight + heightSample * (maxHeight - minHeight);
    position.y = actualHeight;
    
    // Calculate world position
    vec4 worldPos = model * vec4(position, 1.0);
    tes_worldPos = worldPos.xyz;
    
    // Calculate view position for fog
    ViewPosition = view * worldPos;
    
    // Calculate height ratio for texture blending (should be 0-1)
    HeightRatio = heightSample; // This is already normalized 0-1
    
    // Calculate normal using heightmap gradients
    vec2 texelSize = 1.0 / textureSize(heightmapTexture, 0);
    float heightL = texture(heightmapTexture, texCoord - vec2(texelSize.x, 0)).r;
    float heightR = texture(heightmapTexture, texCoord + vec2(texelSize.x, 0)).r;
    float heightD = texture(heightmapTexture, texCoord - vec2(0, texelSize.y)).r;
    float heightU = texture(heightmapTexture, texCoord + vec2(0, texelSize.y)).r;
    
    // Calculate normal from height differences with proper scaling
    float heightRange = maxHeight - minHeight;
    vec3 normal = normalize(vec3(
        (heightL - heightR) * heightRange * 0.5, // Scale factor for normal calculation
        2.0,
        (heightD - heightU) * heightRange * 0.5
    ));
    
    // Transform normal to world space
    tes_normal = normalize(mat3(transpose(inverse(model))) * normal);
    tes_texCoord = texCoord;
    
    gl_Position = projection * view * worldPos;
}