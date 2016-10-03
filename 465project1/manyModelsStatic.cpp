/*

manyModelsStatic.cpp

465 utility include files:  shader465.hpp, triModel465.hpp  

Shaders:  simpleVertex.glsl and simpleFragment.glsl
  provide flat shading with a fixed light position

C OpenGL Core 3.3 example that loads "nModels" *.tri model files 
and displays them in at static locations with a static view.
Demonstrates use of triModel465.hpp functions for loading *.tri models.

display() updated to comment out unneeded statements like:
glEnableVertexAttribArray( vPosition[m] );

These lines are not needed even when the models are moving.

Mike Barnes
10/23/2014
*/

# define __Windows__
# include "../includes465/include465.hpp"

const int X = 0, Y = 1, Z = 2, START = 0, STOP = 1;
// constants for models:  file names, vertex count, model display size
const int nModels = 3;  // number of models in this scene
char * modelFile [nModels] = {"axes-r100.tri", "obelisk-10-20-10.tri", "spaceShip-bs100.tri"};
float modelBR[nModels];       // model's bounding radius
float scaleValue[nModels];    // model's scaling "size" value
const int nVertices[nModels] = { 120 * 3, 14 * 3, 996 * 3 };
char * vertexShaderFile   = "simpleVertex.glsl";     
char * fragmentShaderFile = "simpleFragment.glsl";    
GLuint shaderProgram; 
GLuint VAO[nModels];      // Vertex Array Objects
GLuint buffer[nModels];   // Vertex Buffer Objects

// Shader handles, matrices, etc
GLuint MVP ;  // Model View Projection matrix's handle
GLuint vPosition[nModels], vColor[nModels], vNormal[nModels];   // vPosition, vColor, vNormal handles for models
// model, view, projection matrices and values to create modelMatrix.
float modelSize[nModels] = { 100.0f, 25.0f, 50.0f };   // size of model
glm::vec3 scale[nModels];       // set in init()
glm::vec3 translate[nModels] = {glm::vec3(0,0,0), glm::vec3(50, -50, 0), glm::vec3(-150, -50, -50)};
glm::mat4 modelMatrix;          // set in display()
glm::mat4 viewMatrix;           // set in init()
glm::mat4 projectionMatrix;     // set in reshape()
glm::mat4 ModelViewProjectionMatrix; // set in display();

// window title string
char titleStr [50]= "465 manyModelsStatic Example ";

void reshape(int width, int height) {
  float aspectRatio = (float) width / (float) height;
  float FOVY = glm::radians(60.0f);
  glViewport(0, 0, width, height);
  printf("reshape: FOVY = %5.2f, width = %4d height = %4d aspect = %5.2f \n", 
    FOVY, width, height, aspectRatio);
  projectionMatrix = glm::perspective(FOVY, aspectRatio, 1.0f, 100000.0f); 
  }

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // update model matrix
  for(int m = 0; m < nModels; m++) {
    modelMatrix = glm::translate(glm::mat4(), translate[m]) * 
      glm::scale(glm::mat4(), glm::vec3(scale[m]));
    // glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr( modelMatrix)); 
    ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix; 
    glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
	glBindVertexArray(VAO[m]);
	/*  The following 3 lines are not needed !
    glEnableVertexAttribArray( vPosition[m] );
    glEnableVertexAttribArray( vColor[m] );
    glEnableVertexAttribArray( vNormal[m] );
	*/
    glDrawArrays(GL_TRIANGLES, 0, nVertices[m] ); 
    }
  glutSwapBuffers();
  }

// load the shader programs, vertex data from model files, create the solids, set initial view
void init() {
  // load the shader programs
  shaderProgram = loadShaders(vertexShaderFile,fragmentShaderFile);
  glUseProgram(shaderProgram);
  
  // generate VAOs and VBOs
  glGenVertexArrays( nModels, VAO );
  glGenBuffers( nModels, buffer );
  // load the buffers from the model files
  for (int i = 0; i < nModels; i++) {
    modelBR[i] = loadModelBuffer(modelFile[i], nVertices[i], VAO[i], buffer[i], shaderProgram,
      vPosition[i], vColor[i], vNormal[i], "vPosition", "vColor", "vNormal"); 
    // set scale for models given bounding radius  
    scale[i] = glm::vec3( modelSize[i] * 1.0f/modelBR[i]);
    }
  
  MVP = glGetUniformLocation(shaderProgram, "ModelViewProjection");

 /*
  printf("Shader program variable locations:\n");
  printf("  vPosition = %d  vColor = %d  vNormal = %d MVP = %d\n",
    glGetAttribLocation( shaderProgram, "vPosition" ),
    glGetAttribLocation( shaderProgram, "vColor" ),
    glGetAttribLocation( shaderProgram, "vNormal" ), MVP);
 */

  viewMatrix = glm::lookAt(
    glm::vec3(50.0f, 50.0f, 200.0f),  // eye position
    glm::vec3(0),                   // look at position
    glm::vec3(0.0f, 1.0f, 0.0f)); // up vect0r

  // set render state values
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
  }

int main(int argc, char* argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(1024, 800);
  glutInitContextVersion(3, 3);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutCreateWindow("465 manyModelsStatic Example");
  // initialize and verify glew
  glewExperimental = GL_TRUE;  // needed my home system 
  GLenum err = glewInit();  
  if (GLEW_OK != err) 
      printf("GLEW Error: %s \n", glewGetErrorString(err));      
    else {
      printf("Using GLEW %s \n", glewGetString(GLEW_VERSION));
      printf("OpenGL %s, GLSL %s\n", 
        glGetString(GL_VERSION),
        glGetString(GL_SHADING_LANGUAGE_VERSION));
      }
  // initialize scene
  init();
  // set glut callback functions
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMainLoop();
  printf("done\n");
  return 0;
  }
  

