#version 330 core
in vec3 aPositionVertex;
in vec2 aTextCoords;

out vec2 TexCoord;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPositionVertex, 1.0f);
	//gl_Position = projectionMatrix  * vec4(aPositionVertex, 1.0f);
	// We swap the y-axis by substracing our coordinates from 1. This is done because most images have the top y-axis inversed with OpenGL's top y-axis.
	// TexCoord = texCoord;
	TexCoord = vec2(aTextCoords.x, 1.0 - aTextCoords.y);
}