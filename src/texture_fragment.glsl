#version 330 core

uniform vec3 lightPos;
uniform sampler2D boundTexture;
uniform vec3 camPos;


out vec4 frag_colour;
in vec2 oposition;
in vec3 fragPos;
in vec3 onormal;

const float PI = 3.14159265359;
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return nom / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{		
	vec3 albedo = texture(boundTexture, oposition).xyz;
	float metallic = 1.0f;
	float roughness = 0.1f;
	float ao = 0.8f;

	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 N = normalize(onormal);
    vec3 V = normalize(camPos - fragPos);

    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    vec3 F  = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
	
    vec3 kD = vec3(1.0) - F;
    kD *= 1.0 - metallic;	  
           
    // reflectance equation
    vec3 Lo = vec3(0.0);
    // calculate per-light radiance
    vec3 L = normalize(lightPos - fragPos);
    vec3 H = normalize(V + L);
    float distance    = length(lightPos - fragPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance     = lightColor * attenuation;        
    
    // cook-torrance brdf
    float NDF = DistributionGGX(N, H, roughness);        
    float G   = GeometrySmith(N, V, L, roughness);      
    
    vec3 nominator    = NDF * G * F;
    float denominator = 4 * max(dot(V, N), 0.0) * max(dot(L, N), 0.0) + 0.001; 
    vec3 brdf = nominator / denominator;
        
    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);                
    Lo += (kD * albedo / PI + brdf) * radiance * NdotL; 
   
    vec3 ambient = vec3(0.1) * albedo * ao;
    vec3 color = ambient + Lo;
	
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  
   
    frag_colour = vec4(color, 1.0);
}  