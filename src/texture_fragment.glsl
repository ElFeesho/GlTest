#version 330 core

uniform vec3 lightPos;
uniform sampler2D boundTexture;
uniform vec3 camPos;


out vec4 frag_colour;
in vec2 oposition;
in vec3 fragPos;
in vec3 onormal;

void main()
{		
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 textColor = texture(boundTexture, oposition).xyz;
	float ambientStrength = 0.3f;
    vec3 ambient = ambientStrength * vec3(1.0f, 1.0f, 1.0f);
  	
    // Diffuse 
    vec3 norm = normalize(onormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightPos - camPos), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(camPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * textColor;
    frag_colour = vec4(result, 1.0f);
}  