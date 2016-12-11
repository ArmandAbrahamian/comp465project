/* 
SimpleVertex.glsl

Vertex shader with position, color, normal and ModelViewProject
input and color output.

Mike Barnes
8/17/2013
*/

# version 330 core

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;  // not used
in vec2 vTextCoord;

uniform bool IsTexture;

uniform mat4 ModelViewProjection;  // = projection * view * model
uniform mat4 ModelViewMatrix; //model*view
uniform mat3 NormalMatrix; //mat3(model*view)

//output of vertex shader program
out vec3 Position;
out vec2 TextCoord;
out vec4 Color; // fragments final color
out vec3 Normal;

void main() {

  // transform the normal, without perspective, and normalize it
  Normal = normalize(NormalMatrix * vNormal);

  //2d coord position
  gl_Position = ModelViewProjection * vPosition;

  //calculate position of the object in eye space
  Position = (ModelViewMatrix * vPosition).xyz;

  if(IsTexture) {
	TextCoord = vTextCoord;
	Color = vec4(0.0, 0, 0, 0);
  }
  else {
	TextCoord = vec2(0, 0);
	Color = vColor;
  }
  
  }