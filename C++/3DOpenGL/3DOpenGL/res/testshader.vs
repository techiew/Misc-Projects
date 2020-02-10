#version 330 core

in vec3 position;
in vec4 color;
//in vec3 normal;

//out vec2 texCoord;
out vec4 colorOut;
out vec3 normalOut;
out vec3 fragPosOut;

uniform mat4 transform;

void main()
{
	vec3 normal = vec3(0.0f, 0.0f, 1.0f);
	gl_Position = transform * vec4(position, 1.0);
	//texCoord = inputTexCoord;
	colorOut = color;
	fragPosOut = gl_Position.xyz;
	normalOut = mat3(transpose(inverse(transform))) * normal;
}