#version 330 core
in vec2 TexCoord;
in vec3 FragPos;
in float IsWater;
in vec3 Normal;
out vec4 FragColor;

uniform bool isCircle;
uniform sampler2D sandTexture;
uniform sampler2D grassTexture;
uniform sampler2D waterTexture;
uniform float time;
uniform sampler2D dudvMap;

// Helper function to estimate terrain normals in the fragment shader
vec3 estimateNormal(vec3 pos) {
    // Only use this for terrain (not water)
    if (IsWater > 0.5) return Normal;
    
    // Terrain height is stored in FragPos.y
    float height = pos.y;
    
    // Use terrain position to create a normal that varies with position
    // This creates a bumpy effect without actual displacement
    float xSlope = sin(pos.x * 15.0 + pos.z * 13.0) * 0.2;
    float zSlope = cos(pos.z * 17.0 + pos.x * 11.0) * 0.2;
    
    return normalize(vec3(-xSlope, 1.0, -zSlope));
}

void main() {
    // Get a terrain-specific normal for lighting
    vec3 terrainNormal = IsWater > 0.5 ? Normal : estimateNormal(FragPos);
    
    // Enhanced directional lighting for better 3D look
    vec3 lightDir = normalize(vec3(0.5, 0.8, 0.2));
    float diff = max(dot(terrainNormal, lightDir), 0.3); // Add ambient component
    
    if (isCircle) {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red for circle
    } 
    else if (IsWater > 0.5) {
        // Calculate distance from shore for sand blending
        float distanceFromCenter = length(vec2(FragPos.x, FragPos.z));
        float shoreFactor = smoothstep(0.7, 0.9, distanceFromCenter);
        
        // Create multiple layers of animated DUDV distortion for dynamic water
        // First distortion layer - large scale movement
        vec2 dudvCoord1 = TexCoord * 1.5;
        dudvCoord1.x += time * 0.04;
        dudvCoord1.y -= time * 0.03;
        vec2 dudvValue1 = (texture(dudvMap, dudvCoord1).rg * 2.0 - 1.0) * 0.02;
        
        // Second distortion layer - medium scale detail
        vec2 dudvCoord2 = TexCoord * 3.0 + dudvValue1 * 0.5;
        dudvCoord2.x -= time * 0.02;
        dudvCoord2.y += time * 0.05;
        vec2 dudvValue2 = (texture(dudvMap, dudvCoord2).rg * 2.0 - 1.0) * 0.015;
        
        // Third distortion layer - small ripple details
        vec2 dudvCoord3 = TexCoord * 5.0;
        dudvCoord3.x += time * 0.06 + dudvValue1.y * 2.0;
        dudvCoord3.y += time * 0.04 - dudvValue1.x * 2.0;
        vec2 dudvValue3 = (texture(dudvMap, dudvCoord3).rg * 2.0 - 1.0) * 0.008;
        
        // Combine all distortion layers for complex water movement
        vec2 finalDistortion = dudvValue1 + dudvValue2 + dudvValue3;
        
        // Create dynamic flowing water effect with moving texture + distortion
        vec2 flowingTexCoord = TexCoord;
        flowingTexCoord.x += time * 0.05 + sin(time * 0.2) * 0.01; // Variable speed
        flowingTexCoord.y -= time * 0.03 + cos(time * 0.3) * 0.01;
        
        // Apply the combined distortion to the moving texture coordinates
        vec2 distortedUV = flowingTexCoord * 3.0 + finalDistortion;
        
        // Sample water texture with the complex distortion
        vec4 waterColor = texture(waterTexture, distortedUV);
        
        // Create dynamic specular highlights that move with the water
        float highlight = pow(max(0.0, dot(reflect(-lightDir, 
                       normalize(Normal + vec3(finalDistortion.x, 0.0, finalDistortion.y) * 0.5)), 
                       vec3(0.0, 1.0, 0.0))), 
                       64.0 + sin(time) * 16.0) * 0.5;
        
        // Sample sand texture near shores with subtle movement
        vec2 sandCoord = TexCoord;
        sandCoord.x += time * 0.01 + finalDistortion.x * 0.2;
        sandCoord.y += finalDistortion.y * 0.2;
        vec4 sandColor = texture(sandTexture, sandCoord * 6.0);
        
        // Create base water color with depth gradient
        vec3 deepColor = vec3(0.0, 0.15, 0.3);
        vec3 shallowColor = vec3(0.0, 0.5, 0.8);
        vec3 baseWaterColor = mix(shallowColor, deepColor, shoreFactor);
        
        // Add some subtle color variation based on time
        baseWaterColor += vec3(0.02 * sin(time * 0.3), 0.02 * cos(time * 0.4), 0.03 * sin(time * 0.2));
        
        // Blend water texture details
        vec3 finalWaterColor = mix(baseWaterColor, waterColor.rgb, 0.3);
        
        // Add highlight that moves with the water
        finalWaterColor += vec3(highlight);
        
        // Blend with sand at the edges
        float sandBlend = 0.0;
        if (distanceFromCenter > 0.7 && distanceFromCenter < 0.78) {
            sandBlend = 1.0 - smoothstep(0.7, 0.78, distanceFromCenter);
            sandBlend *= 0.7; // Reduce sand intensity
            
            // Add some noise to sand edge with subtle animation
            sandBlend *= (0.8 + 0.4 * sin(FragPos.x * 30.0 + FragPos.z * 25.0 + time * 0.2));
            
            finalWaterColor = mix(finalWaterColor, sandColor.rgb, sandBlend);
        }
        
        // Final water opacity - more transparent at edges
        float alpha = mix(0.7, 0.9, shoreFactor);
        
        // Make sand areas more opaque
        alpha = mix(alpha, 0.95, sandBlend);
        
        FragColor = vec4(finalWaterColor, alpha);
    } 
    else {
        // Enhanced terrain rendering with height-based texture blending
        float height = FragPos.y * 5.0; // Scale up for better visibility
        
        // Sample both textures with different scales for more detail
        vec4 sandTex = texture(sandTexture, TexCoord * 3.0);
        vec4 grassTex = texture(grassTexture, TexCoord * 4.0);
        
        // Height-based blending with smooth transition
        float blendFactor = smoothstep(0.0, 0.1, height);
        vec4 terrainColor = mix(sandTex, grassTex, blendFactor);
        
        // Apply directional lighting for better 3D look
        terrainColor.rgb *= diff;
        
        // Add subtle height-based coloring for even more depth
        terrainColor.rgb = mix(terrainColor.rgb, vec3(0.7, 0.7, 0.5), max(0.0, height - 0.2) * 0.5);
        
        FragColor = terrainColor;
    }
}