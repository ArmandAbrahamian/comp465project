/*

Source1.cpp

465 utility include files:  shader465.hpp, triModel465.hpp

Shaders:  simpleVertex.glsl and simpleFragment.glsl
provide flat shading with a fixed light position

Armand Abrahamian
9/23/16
*/

# define __Windows__
# include "../includes465/include465.hpp"

/* Constants: */
const int X = 0, Y = 1, Z = 2, START = 0, STOP = 1;
// constants for models:  file names, vertex count, model display size
const int nModels = 3;  // number of models in this scene
const int nVertices[nModels] = { 264 * 3, 264 * 3, 996 * 3 };

/* Global Variables: */
int Index = 0;  // global variable indexing into VBO arrays

// display state and "state strings" for title display
bool perspective = true;

char fpsStr[15], timerStr[20] = " interval timer";
char baseStr[50] = "Warbird Simulator Phase 1, Use q to quit.";
char titleStr[100];

char * modelFile[nModels] = { "FacePlanet.tri", "WaterPlanet.tri", "spaceShip-bs100.tri" };
char * cameraNames[5] = { "Front Camera", "Top Camera", "Ship Camera", "Unum Camera", "Duo Camera" };
char * vertexShaderFile = "simpleVertex.glsl";
char * fragmentShaderFile = "simpleFragment.glsl";
float modelBR[nModels];       // model's bounding radius
float scaleValue[nModels];    // model's scaling "size" value
GLuint shaderProgram;
GLuint VAO[nModels];      // Vertex Array Objects
GLuint buffer[nModels];   // Vertex Buffer Objects

// Shader handles, matrices, etc
GLuint MVP;  // Model View Projection matrix's handle
GLuint vPosition[nModels], vColor[nModels], vNormal[nModels];   // vPosition, vColor, vNormal handles for models
																// model, view, projection matrices and values to create modelMatrix.
float modelSize[nModels] = { 25.0f, 30.0f, 50.0f };   // size of model
glm::vec3 scale[nModels];       // set in init()
glm::vec3 translate[nModels] = { glm::vec3(0,0,0), glm::vec3(50, -50, 0), glm::vec3(-150, -50, -50) };
glm::mat4 modelMatrix;          // set in display()
glm::mat4 mainCamera;           // set in init()
glm::mat4 frontCamera;
glm::mat4 topCamera;
glm::mat4 shipCamera;
glm::mat4 unumCamera;
glm::mat4 duoCamera;
glm::mat4 projectionMatrix;     // set in reshape()
glm::mat4 ModelViewProjectionMatrix; // set in display();

int currentCamera = 0;
int maxCameras = 5;

glm::vec3 eye, at, up; // vectors and values for lookAt.

// rotational variables
GLfloat rotateRadian = 0.0f;
float eyeDistanceMultiplier = 10.0f;
float eyeDistance;
glm::mat4 identity(1.0f); // initialized identity matrix.
glm::mat4 rotation;
int timerDelay = 40, frameCount = 0;
double currentTime, lastTime, timeInterval;
bool idleTimerFlag = false;  // interval or idle timer ?

/*
Display callback is required by freeglut.
It is invoked whenever OpenGL determines a window has to be redrawn.
*/
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Actually clears the window to color specified in glClearColor().

														/* Final step in preparing the data for processing by OpenGL is to specify which vertex
														attributes will be issued to the graphics pipeline. */

	rotation = glm::rotate(identity, rotateRadian, glm::vec3(0, 1, 0)); // yaw rotation

																		// Associate shader variables with vertex arrays:
	for (int m = 0; m < nModels; m++) {
		modelMatrix = glm::translate(glm::mat4(), translate[m]) *
			glm::scale(glm::mat4(), glm::vec3(scale[m]));

		ModelViewProjectionMatrix = projectionMatrix * mainCamera * modelMatrix * rotation;
		glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
		glBindVertexArray(VAO[m]);
		glDrawArrays(GL_TRIANGLES, 0, nVertices[m]);  // Initializes vertex shader, for contiguous groups of vertices.
	}
	glutSwapBuffers();

	frameCount++;
	// see if a second has passed to set estimated fps information
	currentTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
	timeInterval = currentTime - lastTime;
	if (timeInterval >= 1000)
	{
		sprintf(fpsStr, " fps %4d", (int)(frameCount / (timeInterval / 1000.0f)));
		lastTime = currentTime;
		frameCount = 0;
	}
}

// Indicates what action should be taken when the window is resized.
void reshape(int width, int height)
{
	float aspectRatio = (GLfloat)width / (GLfloat)height;
	float FOVY = glm::radians(60.0f);

	glViewport(0, 0, width, height);
	projectionMatrix = glm::perspective(FOVY, aspectRatio, 1.0f, 100000.0f);
	printf("reshape: FOVY = %5.2f, width = %4d height = %4d aspect = %5.2f \n",
		FOVY, width, height, aspectRatio);
}

// To maximize efficiency, operations that only need to be called once are called in init().
void init()
{
	// load the shader programs
	shaderProgram = loadShaders(vertexShaderFile, fragmentShaderFile);
	glUseProgram(shaderProgram);

	// generate VAOs and VBOs
	glGenVertexArrays(nModels, VAO);
	glGenBuffers(nModels, buffer);

	// load the buffers from the model files
	for (int i = 0; i < nModels; i++) {
		modelBR[i] = loadModelBuffer(modelFile[i], nVertices[i], VAO[i], buffer[i], shaderProgram,
			vPosition[i], vColor[i], vNormal[i], "vPosition", "vColor", "vNormal");

		// set scale for models given bounding radius  
		scale[i] = glm::vec3(modelSize[i] * 1.0f / modelBR[i]);

		if (modelBR[i] == -1.0f) {
			printf("loadTriModel error:  returned -1.0f \n");
			system("pause");
		}
		else
			printf("loaded %s model with %7.2f bounding radius \n", modelFile[i], modelBR[i]);
	}

	MVP = glGetUniformLocation(shaderProgram, "ModelViewProjection");

	printf("Shader program variable locations:\n");
	printf("  vPosition = %d  vColor = %d  vNormal = %d MVP = %d\n",
		glGetAttribLocation(shaderProgram, "vPosition"),
		glGetAttribLocation(shaderProgram, "vColor"),
		glGetAttribLocation(shaderProgram, "vNormal"), MVP);

	//THE PLANETS ARE TOO SMALL. If you want to see them, decrease the
	//10000 and 20000 to 1000 and 2000 or smaller
	frontCamera = glm::lookAt(
		glm::vec3(0.0f, 10000.0f, 20000.0f),  // eye position
		glm::vec3(0),                   // look at position
		glm::vec3(0.0f, 1.0f, 0.0f)); // up vect0r

	topCamera = glm::lookAt(
		glm::vec3(0, 20000.0f, 0),  // eye position
		glm::vec3(0),                   // look at position
		glm::vec3(1.0f, 0.0f, 0.0f)); // up vect0r

	shipCamera = glm::lookAt(
		glm::vec3(0, 300, 1000),  // eye position
		glm::vec3(0, 300, 0),                   // look at position
		glm::vec3(0.0f, 1.0f, 0.0f)); // up vect0r

	unumCamera = glm::lookAt(
		glm::vec3(0, 0.0f, -800),  // eye position
		glm::vec3(vPosition[0]), // Looks at unum, assuming [0] is unum
		glm::vec3(0.0f, 1.0f, 0.0f)); // up vect0r

	duoCamera = glm::lookAt(
		glm::vec3(0, 0.0f, -800),  // eye position
		glm::vec3(vPosition[1]), //Looks at duo, assuming [1] is duo
		glm::vec3(0.0f, 1.0f, 0.0f)); // up vect0r

	mainCamera = frontCamera;

	// set render state values
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f); // Establishes what color the window will be cleared to.

	lastTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
}

void switchCamera(int camera)
{
	switch (camera)
	{
	case 0:
		mainCamera = frontCamera;
		break;
	case 1:
		mainCamera = topCamera;
		break;
	case 2:
		mainCamera = shipCamera;
		break;
	case 3:
		mainCamera = unumCamera;
		break;
	case 4:
		mainCamera = duoCamera;
		break;
	default:
		return;
	}
	printf("Current Camera: %s\n", cameraNames[camera]);
	display();
}

// for use with Idle and intervalTimer functions 
// to set rotation
void update(void)
{
	rotateRadian += 0.1f;
	if (rotateRadian >  2 * PI) rotateRadian = 0.0f;
	rotation = glm::rotate(identity, rotateRadian, glm::vec3(0, 1, 0));
	glutPostRedisplay();
}

// Estimate FPS, use for fixed interval timer driven animation
void intervalTimer(int i) 
{
	glutTimerFunc(timerDelay, intervalTimer, 1);
	if (!idleTimerFlag)
	{
		update();  // fixed interval timer
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 033: case'q': case'Q':
		exit(EXIT_SUCCESS);
		break;
	case 'v': case 'V':
		currentCamera = (currentCamera + 1) % maxCameras;
		switchCamera(currentCamera);
		break;
	case 'x': case 'X':
		if (currentCamera == 0)
		{
			currentCamera = 5;
		}
		currentCamera = (currentCamera - 1) % maxCameras;
		switchCamera(currentCamera);
		break;
	}

}

/*
The main() has a number of tasks:
* Initialize and open a window
* Initialize the buffers and parameters by calling init()
* Specify the callback functions for events
* Enter an infinite event loop
*/
int main(int argc, char** argv)
{
	glutInit(&argc, argv); // Initializes GLUT.
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH); // Configure  GLUT options.
	glutInitWindowSize(1024, 800);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	// set OpenGL and GLSL versions to 3.3 for Comp 465/L, comment to see highest versions.
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(baseStr);

	/*
	GLEW manages function pointers for OpenGL so we want to
	initialize GLEW before we call any OpenGL functions.
	*/
	glewExperimental = GL_TRUE;  // Set true to use more modern techniques for managing OpenGL functionality.
	GLenum err = glewInit(); // Initialize GLEW
	if (GLEW_OK != err) // Check for any errors. (Must be done after GLUT has been inititalized)
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
	glutDisplayFunc(display); // Continuously called for interacting with the window. 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	glutIdleFunc(NULL);  // start with intervalTimer
	glutTimerFunc(timerDelay, intervalTimer, 1);

	glutMainLoop();  // This call passes control to GLUT.

	printf("done\n");
	return 0;
}