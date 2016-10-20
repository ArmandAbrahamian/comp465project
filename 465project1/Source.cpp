/*
Second Phase of Warbird Simulator.

Description: The Warbird's camera, movement, and warp capabilities, gravity, missle sites, 
and intelligens-semita missles are added to the simulation in this phase.

File: Source1.cpp
465 utility include files:  shader465.hpp, triModel465.hpp

Shaders:  simpleVertex.glsl and simpleFragment.glsl
provide flat shading with a fixed light position

Team Members:
Armand Abrahamian
Ben Villalobos
Bryant Barron

COMP 465 - Fall 2016
11/20/16
*/

# define __Windows__
# include "../includes465/include465.hpp"
# include "SpaceBody.h"

/* Constants: */
const int X = 0, Y = 1, Z = 2, START = 0, STOP = 1;
const int nModels = 7;  // number of models in this scene
const int nVertices[nModels] = { 264 * 3, 312 * 3, 264 * 3, 264 * 3, 264 * 3, 996 * 3, 14 * 3}; // vertex count
const glm::vec3 upVector(0.0f, 1.0f, 0.0f);
const glm::vec3 topVector(1.0f, 0.0f, 0.0f);

SpaceBody * spaceBody[nModels];

/* Models: */
float modelBR[nModels];       // model's bounding radius
float modelSize[nModels] = { 2000.0f, 200.0f, 400.0f, 100.0f, 150.0f, 100.0f, 25.0f};   // size of model
//								Ruber		Unum			Duo					primus			secundus				SpaceShip				missle
char * modelFile[nModels] = { "Sun.tri", "RingPlanet.tri", "FacePlanet.tri", "WaterPlanet.tri", "BlownUpPlanet.tri", "spaceShip-bs100.tri", "obelisk-10-20-10.tri"};
glm::vec3 scale[nModels];       // set in init()
glm::mat4 translationMatrix[nModels];
//										ruber				unum					duo						primus					secundus			ship						missle
glm::vec3 translatePosition[nModels] = { glm::vec3(0,0,0), glm::vec3(4000, -50, 0), glm::vec3(9000, 0, 0), glm::vec3(8100, 0, 0),glm::vec3(7250,0,0), glm::vec3(5000, 1000, 5000), glm::vec3(4900,1000,4850) };

/* Display state and "state strings" for title display */
char titleStr[160];
char fpsStr[15], timerStr[20] = "| U/S 200 ";
char baseStr[90] = "Warbird Simulator: {v(next), x(previous) camera} | Warbird ? | Unum ? | Secundus ? ";
char cameraStr[30] = "| View: Front Camera";

/* Shader handles, matrices, etc */
GLuint MVP;  // Model View Projection matrix's handle
GLuint vPosition[nModels], vColor[nModels], vNormal[nModels];   // vPosition, vColor, vNormal handles for models
																// model, view, projection matrices and values to create modelMatrix.
glm::mat4 modelMatrix[nModels];          // set in display()
glm::mat4 * viewMatrix;
glm::mat4 projectionMatrix;     // set in reshape()
glm::mat4 ModelViewProjectionMatrix; // set in display();

char * vertexShaderFile = "simpleVertex.glsl";
char * fragmentShaderFile = "simpleFragment.glsl";

GLuint shaderProgram;
GLuint VAO[nModels];      // Vertex Array Objects
GLuint buffer[nModels];   // Vertex Buffer Objects

/* Camera: */
char * cameraNames[5] = { "Front Camera", "Top Camera", "Ship Camera", "Unum Camera", "Duo Camera" };
glm::mat4 mainCamera;           // set in init()
glm::mat4 frontCamera;
glm::mat4 topCamera;
glm::mat4 shipCamera;
glm::mat4 unumCamera;
glm::mat4 duoCamera;

int currentCamera = 0;
int maxCameras = 5;

glm::vec3 eye, at, up; // vectors and values for lookAt.
glm::vec3 shipCamEyePosition(5000, 1300, 6000);
glm::vec3 planetCamEyePosition(0, 0.0f, -8000);
glm::vec3 topCamEyePosition(0, 20000.0f, 0);
glm::vec3 frontCamEyePosition(0.0f, 10000.0f, 20000.0f);

/* Rotational variables */
GLfloat radians = 0.004f;
GLfloat radians2 = 0.002f;

float eyeDistanceMultiplier = 10.0f;
float eyeDistance;

glm::mat4 identityMatrix(1.0f); // initialized identity matrix.
glm::mat4 moonRotationMatrix;
glm::mat4 transformMatrix[nModels];
glm::mat4 rotationMatrix;
glm::vec3 rotationalAxis(0.0f, 1.0f, 0.0f);

int timerDelay = 5, frameCount = 0; // A delay of 5 milliseconds is 200 updates / second
double currentTime, lastTime, timeInterval;
bool idleTimerFlag = false;  // interval or idle timer ?

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
	for (int i = 0; i < nModels; i++)
	{
		modelBR[i] = loadModelBuffer(modelFile[i], nVertices[i], VAO[i], buffer[i], shaderProgram,
			vPosition[i], vColor[i], vNormal[i], "vPosition", "vColor", "vNormal");

		// set scale for models given bounding radius  
		scale[i] = glm::vec3(modelSize[i] / modelBR[i]);

		if (modelBR[i] == -1.0f)
		{
			printf("loadTriModel error:  returned -1.0f \n");
			system("pause");
		}
		else
		{
			printf("loaded %s model with %7.2f bounding radius \n", modelFile[i], modelBR[i]);
		}
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
		glm::vec3(0.0f, 10000.0f, 20000.0f),  // eye position (in world space)
		glm::vec3(0),                   // look at position (looks at the origin)
		glm::vec3(0.0f, 1.0f, 0.0f)); // up vect0r (head is up, set to 0,-1,0 to look upside down)

	topCamera = glm::lookAt(
		glm::vec3(0, 20000.0f, 0),  // eye position
		glm::vec3(0),                   // look at position
		glm::vec3(1.0f, 0.0f, 0.0f)); // up vect0r

	unumCamera = glm::lookAt(
		glm::vec3(0, 0.0f, -8000),  // eye position
		glm::vec3(translatePosition[1]), // Looks at unum assuming [1] is unum
		glm::vec3(0.0f, 1.0f, 0.0f)); // up vect0r

	duoCamera = glm::lookAt(
		glm::vec3(0, 0.0f, -8000),  // eye position
		glm::vec3(translatePosition[3]), //Looks at duo, assuming [3] is duo
		glm::vec3(0.0f, 1.0f, 0.0f)); // up vect0r

	shipCamera = glm::lookAt(
		glm::vec3(5000, 1300, 6000),  // eye position
		glm::vec3(translatePosition[5]),           // look at position
		glm::vec3(0.0f, 1.0f, 0.0f)); // up vect0r

	mainCamera = frontCamera;

	// set render state values
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f); // Establishes what color the window will be cleared to.

	// Create spaceBody objects
	for (int i = 0; i < nModels; i++)
	{
		if (i == 0 || i == nModels-1)
		{
			spaceBody[i] = new SpaceBody(false, false);
		}
		else if (i > 0 && i < 3)
		{
			spaceBody[i] = new SpaceBody(true, false);
		}
		else if (i == 3)
		{
			spaceBody[i] = new SpaceBody(true, true);
		}
	}

	lastTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
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

// update and display animation state in window title
void updateTitle() 
{
	strcpy(titleStr, baseStr);
	strcat(titleStr, timerStr);
	strcat(titleStr, fpsStr);
	strcat(titleStr, cameraStr);
	glutSetWindowTitle(titleStr);
}

/*
	Display() callback is required by freeglut.
	It is invoked whenever OpenGL determines a window has to be redrawn.
*/
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Actually clears the window to color specified in glClearColor().

														/* Final step in preparing the data for processing by OpenGL is to specify which vertex
														attributes will be issued to the graphics pipeline. */

	// Associate shader variables with vertex arrays:
	for (int m = 0; m < nModels; m++) 
	{
		glBindVertexArray(VAO[m]); // set model for its instance. Have to rebind everytime its changed.
		translationMatrix[m] = glm::translate(identityMatrix, translatePosition[m]);
		// If it's Ruber, SpaceShip, or missle model don't apply an orbital rotation.
		if (m == 0 || m == 5 || m == 6 )
		{
			modelMatrix[m] = translationMatrix[m] *
				glm::scale(identityMatrix, glm::vec3(scale[m])); 
		}
		// If it's Duo:
		else if (m == 2)
		{
			transformMatrix[m] = moonRotationMatrix * translationMatrix[m];
			modelMatrix[m] = transformMatrix[m] *
				glm::scale(identityMatrix, glm::vec3(scale[m]));
			duoCamera = glm::lookAt(getPosition(transformMatrix[m]);
		}
		// If its Primus, one of the moons, orbit around planet Duo.
		else if(m == 3 )
		{
			transformMatrix[m] = transformMatrix[2] * rotationMatrix * glm::translate(identityMatrix, (translatePosition[m] - translatePosition[m - 1]) );
			modelMatrix[m] = transformMatrix[m] *
				glm::scale(identityMatrix, glm::vec3(scale[m]));
			// For Debugging:
			//showMat4("rotation", moonRotationMatrix);
			//showMat4("transform", transformMatrix[m]);
		}
		// If its Secundus, one of the moons, orbit around planet Duo.
		else if (m == 4)
		{
			transformMatrix[m] = transformMatrix[2] * moonRotationMatrix * glm::translate(identityMatrix, (translatePosition[m] - translatePosition[m - 1]));
			modelMatrix[m] = transformMatrix[m] *
				glm::scale(identityMatrix, glm::vec3(scale[m]));

		}
		else
		{
			transformMatrix[m] = rotationMatrix * translationMatrix[m];
			modelMatrix[m] = transformMatrix[m] *
				glm::scale(identityMatrix, glm::vec3(scale[m])); // oribital rotation = rotationMatrix * translationMatrix
			// For Debugging:
			//showMat4("rotation", rotationMatrix);
			//showMat4("transform", transformMatrix[m]);
		}
		viewMatrix = &mainCamera;
		ModelViewProjectionMatrix = projectionMatrix * *viewMatrix * modelMatrix[m];
		glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
		glDrawArrays(GL_TRIANGLES, 0, nVertices[m]);  // Initializes vertex shader, for contiguous groups of vertices.
	}
	glutSwapBuffers();

	frameCount++;
	// see if a second has passed to set estimated fps information
	currentTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
	timeInterval = currentTime - lastTime;
	if (timeInterval >= 1000)
	{
		sprintf(fpsStr, "| F/S %4d ", (int)(frameCount / (timeInterval / 1000.0f)));
		lastTime = currentTime;
		frameCount = 0;
		updateTitle();
	}
}

// Animate scene objects by updating their transformation matrices
// for use with Idle and intervalTimer functions to set rotation
void update(int i)
{
	glutTimerFunc(timerDelay, update, 1); // glutTimerFunc(time, fn, arg). This sets fn() to be called after time millisecond with arg as an argument to fn().
	rotationMatrix = glm::rotate(rotationMatrix, radians, rotationalAxis);
	moonRotationMatrix = glm::rotate(moonRotationMatrix, radians2, rotationalAxis);
	//for (int i = 0; i < nModels; i++)
	//{
	//	spaceBody[i]->update(radians, rotationalAxis);
	//}
	glutPostRedisplay();
}

// Estimate FPS, use for fixed interval timer driven animation
void intervalTimer(int i)
{
	glutTimerFunc(timerDelay, intervalTimer, 1);
	if (!idleTimerFlag)
	{
		update(1);  // fixed interval timer
	}
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
	sprintf(cameraStr, "| View: %s", cameraNames[camera]);
	printf("Current Camera: %s\n", cameraNames[camera]);
	display();
}

void handleCameraLogic(int camera)
{
	;
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
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

void handleSpecialKeypress(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_UP:
			if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
			{
				;
			}
			else
			{
				;
			}
			break;
		case GLUT_KEY_DOWN:
			if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
			{
				;
			}
			else
			{
				;
			}
			break;
		case GLUT_KEY_LEFT:
			if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
			{
				;
			}
			else
			{
				;
			}
			break;
		case GLUT_KEY_RIGHT:
			if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
			{
				;
			}
			else
			{
				;
			}
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
	glutCreateWindow(titleStr);

	/*
	GLEW manages function pointers for OpenGL so we want to
	initialize GLEW before we call any OpenGL functions.
	*/
	glewExperimental = GL_TRUE;  // Set true to use more modern techniques for managing OpenGL functionality.
	GLenum err = glewInit(); // Initialize GLEW
	if (GLEW_OK != err) // Check for any errors. (Must be done after GLUT has been inititalized)
	{
		printf("GLEW Error: %s \n", glewGetErrorString(err));
	}
	else 
	{
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
	glutSpecialFunc(handleSpecialKeypress);

	glutIdleFunc(NULL); // start with intervalTimer
	glutTimerFunc(timerDelay, update, 1); // glutTimerFunc(time, fn, arg). This sets fn() to be called after time millisecond with arg as an argument to fn().

	glutMainLoop();  // This call passes control to enter GLUT event processing cycle.

	printf("done\n");
	return 0;
}
