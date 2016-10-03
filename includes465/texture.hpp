
/*

file: texture.hpp

The freeTexture(...) method deletes texture resource

The loadRawTexture(...) method expects 3 bytes per pixel (24bpp).
The program loads the texture top-left (1,0) to bottom-right(1,0).
Your image may appear upside down depending on how you saved the *.raw
file.  You might need to change the texture coordinates.

The raw file can be converted from other formats by IrfranView.  To
convert to *.raw file format you will also need to install the
plugins.  http://www.irfanview.net/

Mike Barnes
11/5/2013
*/


void freeTexture( GLuint texture ) {  glDeleteTextures( 1, &texture ); }

GLuint loadRawTexture( GLuint texture, const char * filename, int width, int height) {
  unsigned char * data;
  FILE * file;
  int readResult;

  // open texture data
  file = fopen( filename, "rb" );
  if ( file == NULL ) {
     printf("File %s can't be opened\n", filename);
     return 0;}
  // allocate buffer -- 3 bytes / texel -- rgb
  data = (unsigned char *) malloc( width * height * 3 );
  // read texture data
  readResult = fread( data, width * height * 3, 1, file );
  if (readResult != 1) {
     printf("File %s was not read correctly\n", filename);
     return 0; }
  fclose( file );
  
  glGenTextures( 1, &texture ); // generate the texture with the loaded data
  glBindTexture( GL_TEXTURE_2D, texture ); //bind the texture  
  glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); //set texture environment parameters
  // set texture parameters
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
  // generate the texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, 
	  GL_RGB, GL_UNSIGNED_BYTE, data);
  free( data ); //free the texture
  return texture; //return whether it was successfull
}
