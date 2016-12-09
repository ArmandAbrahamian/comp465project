/* 
SimpleFragment.glsl

Fragment shader with color input and output.

Mike Barnes
8/16/2014
*/

# version 330 core

in vec4 color;
out vec4 FragColor;

uniform sampler2D Texture;
uniform bool IsTexture;

in vec3 Position;

in vec2 TextCoord;

void main() {
  FragColor = color;

  if(IsTexture) {
	FragColor = texture(Texture, TextCoord);
  }
  
}