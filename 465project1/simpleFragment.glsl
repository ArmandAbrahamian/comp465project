/* 
SimpleFragment.glsl

Fragment shader with color input and output.

Mike Barnes
8/16/2014
*/

# version 330 core

uniform sampler2D Texture;
uniform bool IsTexture;

in vec3 Position;
in vec3 Normal;
in vec4 color;
in vec2 TextCoord;

out vec4 FragColor;

void main() {
  FragColor = color;

  if(IsTexture) {
	FragColor = texture(Texture, TextCoord);
  }
  
}