#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aTexCoord;
out vec2 TexCoord;
out vec3 FragPos;
out float IsWater;
out vec3 Normal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main() {
    vec3 pos = aPos;
    
    // Default normal pointing up
    vec3 normal = vec3(0.0, 1.0, 0.0);
    
    // Water surfaces are now flat - no wave animation
    // The IsWater flag is still passed to the fragment shader for different rendering
    
    gl_Position = projection * view * model * vec4(pos, 1.0);
    TexCoord = aTexCoord.xy;
    FragPos = pos;
    IsWater = aTexCoord.z;
    Normal = normal;
}