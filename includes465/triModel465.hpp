/*
triModel465.hpp

Two utility functions:  loadTriModel(...) and loadModelBuffer(...)

Using loadModelBuffer(...) in your OpenGL core application will call
loadTriModel(...) to read the model's file data.

LoadTriModel(...) reads a *.tri model file exported from AC3D and creates
glm::vec4 arrays for vertex and color and a glm::vec3 array normal for 
surface data.  A vec4 color value and a vec3 normal value is computed 
for every vertex (not surface).

The 3 glm::vec? parameters are empty on call and with values
on return.  

Function's 4 assumptions:
1.  the *.tri model is centered at (0,0,0).
2.  The *.tri model has only triangles, no co-linear edges/triangle
2.  the *.tri model's vertices have been optimized ( Object | OptimizeVertices ... )
3.  the *.tri model's surfaces have been optimized ( Object | OptimizeSurfaces ... )

Use loadModelBuffer(...) to set *.tri model data into vao's vbo buffer.

Functions prints various error messages, with error returns -1.0f
Functions returns the bounding radius of the model with valid model file.

Note use of std::abs(...) instead of abs(...)

This is for gnu's g++ compiler.  It does not provide abs overloaded
for floats.  abs(float) implicitly converts to abs(int) w/o warning!
Models with a bounding 1 > radius > 0 will truncate to a bounding
radius of 0.0 and so not display.

Mike Barnes
10/11/2013
*/

float loadTriModel(char * fileName, int nVertices, glm::vec4 vertex[], glm::vec4 color[], glm::vec3 normal[]) {
  const int X = 0, Y = 1, Z = 2;
  FILE * fileIn;
  glm::vec3 point[3];   // 3 vertices of a triangle
  glm::vec4 surfaceColor; 
  bool done;
  float coord[3][3];  // triangle's 3 vertice's x, y, z values
  float maxAxes[3] = {-1000.0f, -1000.0f, -1000.0f}; // maximum lenght of x, y, and z from center
  unsigned int triangleColor;		// triangle's 3 coordinates have same hex color code
  unsigned int red, green, blue;  // triangleColor's component colors
  int count = 0, vertexCount = 0, normalCount = 0, colorCount = 0;

  // printf("loadTriModel sizes: nVertices %d, vertex %d, color %d, normal %d\n", nVertices, sizeof(vertex), sizeof(color), sizeof(normal));
  fileIn = fopen(fileName, "r");
  if (fileIn != NULL) {
    while (!feof(fileIn)) {
        done = false;
        for(int i = 0; i < 3; i++)
          for(int j = 0; j < 3; j++) {
            coord[i][j] = -99999.9f;
            if ( fscanf(fileIn, "%f", &coord[i][j]) == EOF) { // at EOF
              done = true;
              break; }
          }
        if(!done) {  
          // create vertices and normals
          // std::abs(....) is used instead of abs(...) because g++ does not provide float abs(float)
          for(int i = 0; i < 3; i++) {  // get triangle's points
            point[i] = glm::vec3(coord[i][X], coord[i][Y], coord[i][Z]); 
            vertex[vertexCount] = glm::vec4(point[i].x, point[i].y, point[i].z, 1.0f);
            // update maxAxes for model's bounding sphere
            if (maxAxes[X] < std::abs(coord[i][X])) maxAxes[X] = std::abs(coord[i][X]);
            if (maxAxes[Y] < std::abs(coord[i][Y])) maxAxes[Y] = std::abs(coord[i][Y]);
            if (maxAxes[Z] < std::abs(coord[i][X])) maxAxes[Z] = std::abs(coord[i][Z]);
            vertexCount++;  }
        // compute normals  for counter-clockwise vertex winding
        normal[normalCount++] = glm::normalize(glm::cross(point[1] - point[0], point[2] - point[0]));
        normal[normalCount++] = glm::normalize(glm::cross(point[2] - point[1], point[0] - point[1]));
        normal[normalCount++] = glm::normalize(glm::cross(point[0] - point[2], point[1] - point[2]));
        fscanf(fileIn, "%x", &triangleColor);
        red = triangleColor;
        green = triangleColor;
        blue = triangleColor;
        red = red >> 16;
        green = green << 16;
        green = green >> 24;   
        blue = blue << 24;
        blue = blue >> 24;
        // make surfaceColor float r,g,b values 0..1
        surfaceColor = glm::vec4(red / 255.0f, green / 255.0f,  blue / 255.0f, 1.0f);
        for(int i = 0; i < 3; i++) color[colorCount++] = surfaceColor;
        count++;
        }  // ! done
        // create vertexs, normal, color 
      } // while ! eof
    } // if
  else { // file is null
      printf("loadTriModel error:  can't open %s\n", fileName);
      return -1.0f;  // just to satisfy the compiler
      }
  if (count != nVertices/3) {
    printf("loadTriModel error:  count of surfaces != numberVertex:  count %4d != nVertices/3 %4d\n",
      count, nVertices/3);
    return -1.0f; }
  if (vertexCount != nVertices) {
    printf("loadTriModel error:  count of vertices mismatch:  vertexCount %4d != nVertices %4d\n",
      vertexCount, nVertices);
    return -1.0f; }
  // return bounding radius
  if (maxAxes[X] >= maxAxes[Y] && maxAxes[X] >= maxAxes[Z]) return maxAxes[X];
  if (maxAxes[Y] >= maxAxes[X] && maxAxes[Y] >= maxAxes[Z]) return maxAxes[Y];
  if (maxAxes[Z] >= maxAxes[X] && maxAxes[Z] >= maxAxes[Y]) return maxAxes[Z];
  printf("loadTriModel error:  method shouldn't reach this point.");
  return -1.0f;
  }

// loads data from *.tri model file into a vao's vbo buffer for vertex, color, normal values
// returns bounding radius of model
float loadModelBuffer(char modelFile[25], GLuint nVertices, 
  GLuint vao, GLuint vbo, GLuint shaderProgram, 
  GLuint vPosition, GLuint vColor, GLuint vNormal,
  char * shaderVertex, char * shaderColor, char * shaderNormal) 
  {
  float boundingRadius;
  // set dynamic array sizes
  int vec3Size = nVertices * sizeof(glm::vec3);
  int vec4Size = nVertices * sizeof(glm::vec4);
  // allocate dynamic arrays
  glm::vec4 * vertex = (glm::vec4 *) calloc(vec4Size, sizeof(glm::vec4));
  glm::vec4 * color =  (glm::vec4 *) calloc(vec4Size, sizeof(glm::vec4));
  glm::vec3 * normal = (glm::vec3 *) calloc(vec3Size, sizeof(glm::vec3));
  // get the model file data
  boundingRadius= loadTriModel(modelFile, nVertices, vertex, color, normal);
  if (boundingRadius == -1.0f) {
    printf("loadTriModel error:  returned -1.0f \n");
    return boundingRadius; }
    else
      printf("loaded %s model with %7.2f bounding radius \n", modelFile, boundingRadius);

  // printf("sizeof[%d]:  vec3 %d, vec4 %d, normal %d \n", modelID, vec3Size, vec4Size);
  // fill the model's buffer
  glBindVertexArray(vao);
  glBindBuffer( GL_ARRAY_BUFFER, vbo);
  glBufferData( GL_ARRAY_BUFFER, 2 * vec4Size + vec3Size, NULL, GL_STATIC_DRAW );
  glBufferSubData( GL_ARRAY_BUFFER, 0, vec4Size, vertex );
  glBufferSubData( GL_ARRAY_BUFFER, vec4Size, vec4Size, color );
  glBufferSubData( GL_ARRAY_BUFFER, 2 * vec4Size, vec3Size, normal );
  // set vertex shader variable handles
  vPosition = glGetAttribLocation( shaderProgram, shaderVertex );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  vColor = glGetAttribLocation( shaderProgram, shaderColor );
  glEnableVertexAttribArray( vColor);
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vec4Size) );
  vNormal = glGetAttribLocation( shaderProgram, shaderNormal );
  glEnableVertexAttribArray( vNormal);
  glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(2 * vec4Size) );

  printf("vPosition %d, vColor %d, vNormal %d \n", vPosition, vColor, vNormal);
  // reclaim dynamically allocated memory
  free(vertex);
  free(color);
  free(normal);
  return boundingRadius;
  }
