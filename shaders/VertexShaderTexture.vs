#version 330 core
in vec3 aPositionVertex;
in vec2 aTextCoords;
in vec3 aPositionNormal;
//in vec3 originPos;

out vec2 TexCoord;
out vec3 PositionNormal;	
out vec3 FragPos;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform float time;
uniform vec3 viewPos;
uniform vec3 originPos;
uniform vec3 starPos;
uniform float planetSpeed;
uniform float timeOffset;

uniform vec3 startPos;

uniform vec2 offsets[10];

void main()
{
	float distancePlanets = distance(originPos, startPos);
	float camX = sin((time*planetSpeed) + timeOffset) * distancePlanets;
	float camZ = cos((time*planetSpeed) + timeOffset) * distancePlanets;
			
	vec4 finalPos = projectionMatrix * viewMatrix * vec4(aPositionVertex.x + camX + originPos.x, aPositionVertex.y + originPos.y,aPositionVertex.z+camZ + originPos.z, 1.0f);
	
	//vec2 offset = offsets[gl_InstanceID];
	//vec3 aOffset = aPositionVertex.xyz + offset;
	gl_Position = finalPos;
	TexCoord = vec2(aTextCoords);
	PositionNormal = mat3(transpose(inverse(modelMatrix))) * aPositionNormal;
	FragPos = vec3(vec4(aPositionVertex.x + camX + originPos.x, aPositionVertex.y + originPos.y,aPositionVertex.z+camZ+ originPos.z, 1.0f));
}