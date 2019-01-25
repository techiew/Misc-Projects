#version 330 core

//in vec2 texCoord;
in vec4 colorOut;
in vec3 normalOut;
in vec3 fragPosOut;

out vec4 FragColor;

//uniform sampler2D diffuse;

void main()
{
	//Ambient light
	float ambientStrength = 1.0f;
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 ambient = ambientStrength * lightColor;
	
	//Diffuse light
	vec3 norm = normalize(normalOut);
	vec3 lightPos = vec3(0.2f, 0.5f, 0.9f);
	vec3 lightDirection = normalize(lightPos - fragPosOut);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = diff * lightColor;
	
	//Specular light
	float specularStrength = 1.0f;
	vec3 viewPos = vec3(0.0f, 0.0f, 0.0f);
	vec3 viewDirection = normalize(viewPos - lightPos);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * colorOut.xyz;
	
	FragColor = vec4(result, 1.0f);
	
	//FragColor = texture2D(diffuse, texCoord);
}