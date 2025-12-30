#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aTexCoord;

out vec2 TexCoord;
out vec3 FragPos;
out float Height;
out float IsWater;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord.xy;
    FragPos = aPos;
    Height = aPos.y;
    IsWater = aTexCoord.z; // Utiliser la 3ème coordonnée de texture pour identifier l'eau
}