#version 330 core

in vec3 ourNormal;
in vec3 FragPos;
in vec3 ourColor; // Recibimos el color del vertex shader
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 luzDir;

uniform float anguloLuz;

uniform sampler2D ourTexture;

void main()
{
    //Luz ambiente
    float ambientI = 0.5;
    vec3 ambient = ambientI * lightColor;

    // Hay que cambiar la posicion de luz por su direccion para que sea direccional

    vec3 ld = luzDir;
    vec3 fd = normalize(vec3((FragPos-lightPos)));

    // Angulo de 15 grados
    if( acos(dot(ld,fd)) < radians(anguloLuz) ){
    
        //difuse
        vec3 norm = normalize(ourNormal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        //specular
        float specularStrength = 1.0;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
        vec3 specular = specularStrength * spec * lightColor;

        //resultado
        vec3 result = (ambient + diffuse + specular) * ourColor;
        vec4 texColor = texture(ourTexture, TexCoord);
        if(texColor.a < 0.1) discard;
        else    FragColor = mix(vec4(result, 1.0), texColor, .5);
    }else{

        vec3 result = (ambient) * ourColor;
        vec4 texColor = texture(ourTexture, TexCoord);
        if(texColor.a < 0.1) discard;
        else    FragColor = mix(vec4(result, 1.0), texColor, .2);
    }

}

/*

--FragColor es lo que deseamos, este se crea con:
    mix(vec4(result, 1.0), texColor, blanco arbitrario);

--texColor es el color de la textura, es:
    texture(mainTextura, vertUV)
    # Si texColor.a, que es el alpha, es menor de un umbral (0.1), se descara con discard;

--result es una mezcla del color y de la luz, se calcula dependiendo de si le da la luz o no:
    
    if(acos(dot(mainLuzDir,normalize(vec3(vertPosFrag-mainPosLuz)))) < radians(mainAnguloLuz))
        (ambient + diffuse + specular) * vertColor;
    else
        (ambient) * vertColor;

--ambient es la luz ambiente, se calcula:
    intensidad de luz arbitraria * mainColorLuz;

--diffuse es la luz cercana a la fuente de luz, la que brilla, se calcula:
    vec3 norm = normalize(vertNormal);
    vec3 lightDir = normalize(mainPosLuz - vertPosFrag);
    float diff = max(dot(norm, lightDir), 0.0); Maximo entre 0 y el producto entre la normal y la posicion
    vec3 diffuse = diff * mainColorLuz;

--specular es cuanto rebota la luz del objeto al que le da, se calcula:
    vec3 viewDir = normalize(mainViewPos - vertPosFrag);
    vec3 reflectDir = reflect(-lightDir, norm); //Ya se hacen en el diffuse
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    vec3 specular = fuerza arbitraria * spec * lightColor;b

*/