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

uniform mat4 ModelViewProjection;  // = projection * view * model
out vec4 color;

void main() {
  color = vColor;
  gl_Position = ModelViewProjection * vPosition;
  }