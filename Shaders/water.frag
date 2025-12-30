#version 330 core
in vec2 TexCoord;
in vec3 FragPos;
in float Height;
in float IsWater;

out vec4 FragColor;

uniform vec3 circlePos;
uniform bool isCircle;
uniform float time;

void main()
{
    if (isCircle) {
        FragColor = vec4(1.0, 0.2, 0.2, 1.0); // Couleur du personnage
    } else if (IsWater > 0.5) {
        // Eau
        float waveSpeed = 0.5;
        float waveHeight = 0.05;
        float waveFreq = 10.0;

        // Effet de vagues animées
        float wave = sin(FragPos.x * waveFreq + time * waveSpeed) * 
                     cos(FragPos.z * waveFreq + time * waveSpeed) * waveHeight;

        vec4 waterColor = vec4(0.0, 0.3 + wave, 0.7 + wave, 0.8);

        // Effet de profondeur
        float depth = 0.05 - FragPos.y;
        waterColor.b += depth * 0.2;

        FragColor = waterColor;
    } else {
        // Île
        float distance = length(vec2(FragPos.x, FragPos.z));

        // Transition des couleurs basée sur la hauteur
        vec4 beachColor = vec4(0.76, 0.7, 0.5, 1.0);      // Sable
        vec4 grassColor = vec4(0.2, 0.6, 0.3, 1.0);       // Herbe

        // Mélange des couleurs en fonction de la hauteur
        float heightFactor = smoothstep(0.0, 0.07, Height);
        vec4 terrainColor = mix(beachColor, grassColor, heightFactor);

        // Ajouter des variations légères
        float noisePattern = sin(FragPos.x * 30.0) * sin(FragPos.z * 30.0) * 0.05;
        terrainColor += vec4(noisePattern, noisePattern, noisePattern, 0.0);

        // Transition vers l'eau aux bords
        if (distance > 0.65) {
            float blendFactor = smoothstep(0.65, 0.7, distance);
            vec4 waterEdgeColor = vec4(0.2, 0.4, 0.6, 1.0);
            terrainColor = mix(terrainColor, waterEdgeColor, blendFactor);
        }

        FragColor = terrainColor;
    }
}