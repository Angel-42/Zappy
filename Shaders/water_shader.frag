#version 330 core
out vec4 FragColor;

in vec4 clipSpace;
in vec3 FragPos;
in vec3 Normal;
in vec2 TextureCoords;
in vec3 toCamera;
in vec3 fromLight;

uniform float time;
uniform vec3 lightColor;

uniform sampler2D reflection_texture;
uniform sampler2D refraction_texture;
uniform sampler2D dudv_texture;
uniform sampler2D normal_map;

const float dist_factor = 0.07; // Augmenté pour plus d'effet de distorsion
const float shineDamper = 20.0;
const float reflectivity = 0.6;

void main()
{
  // Animation des textures
  float moveFactor = time * 0.05;
  
  // Coordonnées de texture animées pour la distorsion
  vec2 distortedTexCoords = texture(dudv_texture, vec2(TextureCoords.x + moveFactor, TextureCoords.y)).rg * 0.1;
  distortedTexCoords = TextureCoords + vec2(distortedTexCoords.x, distortedTexCoords.y + moveFactor);
  
  // Échantillonner la texture dudv pour la distorsion
  vec2 totalDistortion = (texture(dudv_texture, distortedTexCoords).rg * 2.0 - 1.0) * dist_factor;

  // Coordonnées pour la réflexion et la réfraction
  vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5;
  vec2 refractTexCoords = ndc + totalDistortion;
  vec2 reflectTexCoords = vec2(ndc.x, -ndc.y) + totalDistortion;
  
  // Limiter les valeurs
  refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);
  reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
  reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);

  // Couleurs de base
  vec4 reflectColor = texture(reflection_texture, reflectTexCoords);
  vec4 refractColor = texture(refraction_texture, refractTexCoords);
  
  // Normale de la surface de l'eau (utilisant votre texture normal map)
  vec4 normalMapColor = texture(normal_map, distortedTexCoords);
  vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b * 4.0, normalMapColor.g * 2.0 - 1.0);
  normal = normalize(normal);
  
  // Direction de vue et fresnel effect
  vec3 viewVector = normalize(toCamera);
  float refractiveFactor = dot(viewVector, normal);
  refractiveFactor = pow(refractiveFactor, 1.0);
  refractiveFactor = clamp(refractiveFactor, 0.0, 1.0);
  
  // Reflets spéculaires améliorés
  vec3 lightDirection = normalize(-fromLight);
  vec3 reflectedLight = reflect(-lightDirection, normal);
  
  float specular = max(dot(reflectedLight, viewVector), 0.0);
  specular = pow(specular, shineDamper);
  vec3 specularHighlights = lightColor * specular * reflectivity;
  
  // Effet de profondeur avec la texture de caustics (texture de réfraction)
  vec4 causticEffect = texture(refraction_texture, TextureCoords * 3.0 + totalDistortion + vec2(0.0, time * 0.03));
  
  // Mélanger les effets
  vec4 waterColorBase = vec4(0.0, 0.4, 0.55, 1.0); // Couleur de base de l'eau
  
  // Mélange final avec tous les effets
  vec4 finalColor = mix(reflectColor, refractColor, refractiveFactor);
  finalColor = mix(finalColor, waterColorBase, 0.2);
  finalColor += vec4(specularHighlights, 0.0);
  
  // Ajouter l'effet de caustics
  finalColor += causticEffect * 0.2;
  
  // Ajouter un peu de mouvement à la surface
  float surfaceNoise = (sin(TextureCoords.x * 40.0 + time * 2.0) + cos(TextureCoords.y * 40.0 + time)) * 0.05;
  finalColor += surfaceNoise * waterColorBase;
  
  // Transparence contrôlée
  finalColor.a = 0.8;
  
  FragColor = finalColor;
}
