#version 400 core

layout(vertices = 4) out; // 4 control points per quad patch

in vec2 vs_texCoord[];
out vec2 tcs_texCoord[];

uniform float tessellationLevel;
uniform vec3 cameraPos;
uniform float maxDistance;

void main() {
    // Pass through control point data
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    tcs_texCoord[gl_InvocationID] = vs_texCoord[gl_InvocationID];
    
    // Only first invocation sets tessellation levels
    if (gl_InvocationID == 0) {
        // Calculate distance-based tessellation level
        vec3 patchCenter = (gl_in[0].gl_Position.xyz + gl_in[1].gl_Position.xyz + 
                           gl_in[2].gl_Position.xyz + gl_in[3].gl_Position.xyz) * 0.25;
        
        float distance = length(cameraPos - patchCenter);
        float tessLevel = max(1.0, tessellationLevel * (1.0 - distance / maxDistance));
        
        // Set tessellation levels for quad
        gl_TessLevelOuter[0] = tessLevel; // Left edge
        gl_TessLevelOuter[1] = tessLevel; // Bottom edge  
        gl_TessLevelOuter[2] = tessLevel; // Right edge
        gl_TessLevelOuter[3] = tessLevel; // Top edge
        gl_TessLevelInner[0] = tessLevel; // Horizontal inner
        gl_TessLevelInner[1] = tessLevel; // Vertical inner
    }
}