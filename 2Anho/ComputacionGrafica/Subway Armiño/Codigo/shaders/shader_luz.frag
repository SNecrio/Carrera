#version 330 core

in vec3 ourNormal;
in vec3 FragPos;
in vec3 ourColor; // Recibimos el color del vertex shader
in vec2 TexCoord;

out vec4 FragColor;

uniform bool nieblaActiva;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 luzDir;

uniform vec3 lightPos2;
uniform vec3 lightColor2;
uniform vec3 luzDir2;
uniform float anguloLuz2;

uniform vec3 lightPos3;
uniform vec3 lightColor3;
uniform vec3 luzDir3;
uniform float anguloLuz3;

uniform float fogStart;
uniform float fogEnd;
uniform vec3 fogColor;

uniform float anguloLuz;

uniform sampler2D ourTexture;

vec3 calcSpotlight(vec3 lightPos, vec3 lightColor, vec3 luzDir, float angulo, vec3 normal, vec3 fragPos, vec3 viewPos, vec3 baseColor) {
    vec3 ambient = 0.1 * lightColor;
    vec3 ld = luzDir;
    vec3 fd = normalize(fragPos - lightPos);

    if(acos(dot(ld, fd)) < radians(angulo)) {
        vec3 lightDir = normalize(lightPos - fragPos);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        float specularStrength = 1.0;
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
        vec3 specular = specularStrength * spec * lightColor;

        return (ambient + diffuse + specular) * baseColor;
    } else {
        return ambient * baseColor;
    }
}

void main()
{
    vec3 norm = normalize(ourNormal);

    vec3 result1 = calcSpotlight(lightPos, lightColor, luzDir, anguloLuz, norm, FragPos, viewPos, ourColor);
    vec3 result2 = calcSpotlight(lightPos2, lightColor2, luzDir2, anguloLuz2, norm, FragPos, viewPos, ourColor);
    vec3 result3 = calcSpotlight(lightPos3, lightColor3, luzDir3, anguloLuz3, norm, FragPos, viewPos, ourColor);
    
    vec3 finalResult = result1 + result2 + result3;

    vec4 texColor = texture(ourTexture, TexCoord);
    if(texColor.a < 0.1) discard;
    vec4 colorFinal = mix(vec4(finalResult, 1.0), texColor, 0.5);

    //Aplicar niebla si está activa
    if (nieblaActiva) {
        float distance = length(viewPos - FragPos);
        float fogFactor = clamp((fogEnd - distance) / (fogEnd - fogStart), 0.0, 1.0);
        vec3 foggedColor = mix(fogColor, colorFinal.rgb, fogFactor);
        FragColor = vec4(foggedColor, 1.0);
    } else {
        FragColor = colorFinal;
    }

}