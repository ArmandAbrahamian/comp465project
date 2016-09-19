/* 
SimpleFragment.glsl

Fragment shader with color input and output.

Mike Barnes
8/16/2014
*/

# version 330 core

in vec4 color;
out vec4 fragColor;

void main() {
  fragColor = color;
  }