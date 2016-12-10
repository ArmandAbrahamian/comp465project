/* 
SimpleFragment.glsl

Fragment shader with color input and output.

Based on the shader examples in the 
Introduction to OpenGL Version 4.3 red book textbook.

12/10/2016
*/

# version 330 core

// Texture data:
uniform sampler2D Texture;
uniform bool IsTexture;

// Input information from the vertex shader:
in vec3 Position;
in vec3 Normal;
in vec4 color;
in vec2 TextCoord;

// Light Source Parameters:
uniform vec4 ambient; // sets lighting level, same across many vertices.
vec4 diffuse;
vec4 specular;
vec4 position;
vec4 halfVector;
vec3 spotDirection;
float spotExponent;
float spotCutoff; // (range: [0.0,90.0], 180.0)
float spotCosCutoff; // (range: [1.0,0.0],-1.0)
float constantAttenuation;
float linearAttenuation;
float quadraticAttenuation;

// Output information:
out vec4 FragColor;

void main() {
	
vec4: scatteredLight = Ambient; // this is the only light;

  FragColor = min(color * scatteredLight,vec4(1.0));

  if(IsTexture) {
	FragColor = texture(Texture, TextCoord);
  }
  
}