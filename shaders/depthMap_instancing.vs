#version 330 core

layout (location = 0) in vec3 aPositionVertex;
layout (location = 1) in vec2 aTextCoords;
layout (location = 2) in vec3 aPositionNormal;
layout (location = 3) in mat4 instanceMatrix;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform float time;
uniform mat4 lightSpaceMatrix;
uniform vec3 viewPos;

out vec2 TexCoords;
out vec3 FragPoss;

//generate a random value from four points
vec4 rand(vec2 A,vec2 B,vec2 C,vec2 D){ 

	vec2 s=vec2(12.9898,78.233); 
	vec4 tmp=vec4(dot(A,s),dot(B,s),dot(C,s),dot(D,s)); 

	return fract(sin(tmp) * 43758.5453)* 2.0 - 1.0; 
} 

//this is similar to a perlin noise function
float noise(vec2 coord,float d){ 

	vec2 C[4]; 

	float d1 = 1.0/d;

	C[0]=floor(coord*d)*d1; 

	C[1]=C[0]+vec2(d1,0.0); 

	C[2]=C[0]+vec2(d1,d1); 

	C[3]=C[0]+vec2(0.0,d1);


	vec2 p=fract(coord*d); 

	vec2 q=1.0-p; 

	vec4 w=vec4(q.x*q.y,p.x*q.y,p.x*p.y,q.x*p.y); 

	return dot(vec4(rand(C[0],C[1],C[2],C[3])),w); 
} 

void main()
{
	vec4 otherPosition = instanceMatrix * vec4(1.0f);
	vec3 pos = vec3(normalize(otherPosition));
	pos = -time * pos;
	
	vec4 finalPos = projectionMatrix * viewMatrix * modelMatrix * instanceMatrix * vec4(aPositionVertex + pos, 1.0f);
    
	float noiseAmntX = noise( vec2(-(time*0.1) + finalPos.x * 0.05f, 100.0f), 2.5 );
	float noiseAmntY = noise( vec2((time*0.1) + finalPos.y * 0.05f, finalPos.x * 0.01f), 2.5 );
	float  noiseAmntZ = noise( vec2((time*0.1) + finalPos.z * 0.05f, finalPos.z * 0.01f), 2.5 );
	
	mat4 transformationMatrix; 
	transformationMatrix[0] = vec4(1.0f,0.0f,0.0f,(noiseAmntX * 0.2));
	transformationMatrix[1] = vec4(0.0f,1.0f,0.0f,(noiseAmntY * 0.2));
	transformationMatrix[2] = vec4(0.0f,0.0f,1.0f,(noiseAmntZ * 0.1));
	transformationMatrix[3] = vec4(0.0f,0.0f,0.0f,1.0f);

	
	/*mat4 transformationMatrix; 
	transformationMatrix[0] = vec4(1.0f,0.0f,0.0f,0.0f);
	transformationMatrix[1] = vec4(0.0f,1.0f,0.0f,0.0f);
	transformationMatrix[2] = vec4(0.0f,0.0f,1.0f,0.0f);
	transformationMatrix[3] = vec4(0.0f,0.0f,0.0f,1.0f);*/
	
	finalPos = vec4(transformationMatrix * finalPos);
	
	/*transformationMatrix[3] = noiseAmntX * 0.2;
	transformationMatrix[7] = noiseAmntY * 0.2;
	transformationMatrix[11] = noiseAmntZ * 0.1;*/
	
	vs_out.FragPos = vec3(finalPos);
    vs_out.Normal = transpose(inverse(mat3(modelMatrix * instanceMatrix * transformationMatrix))) * aPositionNormal;
    vs_out.TexCoords = aTextCoords;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
	
	gl_Position = vec4(finalPos);
	
	//vec3 d2 = (vec3(finalPos) - vec3(viewPos));
	//FragPoss = ((viewPos + d2 / dot(d2, normalize(vec3(0.0f,0.0f, 1.0f))))*d2);
	FragPoss = vec3(finalPos);
}  