#version 330 core
in vec2 TexCoord;
in vec3 PositionNormal;
in vec3 FragPos;

out vec4 color;

uniform sampler2D ourTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	vec3 lightColor = vec3(1.0f,1.0f,1.0f);
	float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor * vec3(texture(ourTexture, TexCoord));
	
	vec3 norm = normalize(PositionNormal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor * vec3(texture(ourTexture, TexCoord));
	
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = (specularStrength * spec * lightColor) * vec3(texture(ourTexture, TexCoord));	
	
	vec3 result = ambient + diffuse + specular;
	
	color = vec4(result, 1.0f);
}