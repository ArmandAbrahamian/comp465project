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

-----------------------------------------------------------------
To Do:
* Create gravity for Unum, Duo, Ship
* Update the status bar title to display the current U/S
* Fix missle model direction be direction of ship.

Ship Movement:
* Warping capabilities
* Update camera when pitch up/down, roll left/right

Missle Sites:
* Register model for missle silo
* Register Model for missle (optional)

Missle Movement:
* Smart Missles
// C - LAT * E

Time Quantum:
 

Pass or Resign:
* Can display a texture on the screen, text overlay, or output to the console for game over.
* When the ship runs out of missles game over.
* When the missle silo missles collide with the ship game over.

Documentation:
* Need to modify completely.

*/

# define __Windows__
# include "../includes465/include465.hpp"
# include <string>
# include "SpaceBody.h"

const int X = 0, Y = 1, Z = 2, START = 0, STOP = 1,
RUBERINDEX = 0, UNUMINDEX = 1, DUOINDEX = 2, PRIMUSINDEX = 3, SECUNDUSINDEX = 4, SHIPINDEX = 5, MISSILEINDEX = 6, FIRSTMISSLESILOINDEX = 7, SECONDMISSLESILOINDEX = 8,
FRONTCAMERAINDEX = 0, TOPCAMERAINDEX = 1, SHIPCAMERAINDEX = 2, UNUMCAMERAINDEX = 3, DUOCAMERAINDEX = 4;

/* Models: */
const int nModels = 9;  // number of models in this scene
const int nVertices[nModels] = { 264 * 3, 312 * 3, 264 * 3, 264 * 3, 264 * 3, 996 * 3, 282 * 3, 720 * 3, 720 * 3 }; // vertex count
float modelBR[nModels];       // model's bounding radius
float modelSize[nModels] = { 2000.0f, 200.0f, 400.0f, 100.0f, 150.0f, 100.0f, 75.0f, 100.0f, 100.0f };   // size of model
//								Ruber		Unum			Duo					primus			secundus				SpaceShip				missle
char * modelFile[nModels] = { "Sun.tri", "RingPlanet.tri", "FacePlanet.tri", "WaterPlanet.tri", "BlownUpPlanet.tri", "spaceShip-bs100.tri", "Missile.tri", "MissileSite.tri", "MissileSite.tri"  };
glm::vec3 scale[nModels];       // set in init()
glm::mat4 translationMatrix[nModels];
//										ruber				unum					duo						primus					secundus			ship						missle
glm::vec3 translatePosition[nModels] = { glm::vec3(0,0,0), glm::vec3(4000, -50, 0), glm::vec3(9000, 0, 0), glm::vec3(8100, 0, 0),glm::vec3(7250,0,0), glm::vec3(5000, 1000, 5000), glm::vec3(4900,1000,4850) };
//SpaceBody * spaceBody[nModels];

/* Shader handles, matrices, etc */
GLuint MVP;  // Model View Projection matrix's handle
GLuint vPosition[nModels], vColor[nModels], vNormal[nModels];   // vPosition, vColor, vNormal handles for models
																// model, view, projection matrices and values to create modelMatrix.
glm::mat4 modelMatrix[nModels];          // set in display()
glm::mat4 viewMatrix;
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
const glm::vec3 upVector(0.0f, 1.0f, 0.0f);
const glm::vec3 topVector(1.0f, 0.0f, 0.0f);
glm::vec3 camPosition;
glm::vec3 shipPosition;
glm::vec3 shipCamEyePosition(0, 200, 500);
glm::vec3 planetCamEyePosition(0, 0.0f, -8000);
glm::vec3 topCamEyePosition(0, 20000.0f, 0);
glm::vec3 frontCamEyePosition(0.0f, 10000.0f, 20000.0f);

/* Planet rotational variables: */
GLfloat radians = 0.004f;
GLfloat radians2 = 0.002f;
float eyeDistanceMultiplier = 10.0f;
float eyeDistance;
float unumGravityVector = 1.11f;
float duoGravityVector = 5.63f;
glm::mat4 identityMatrix(1.0f); // initialized identity matrix.
glm::mat4 moonRotationMatrix;
glm::mat4 transformMatrix[nModels];
glm::mat4 rotationMatrix;
glm::vec3 rotationalAxis(0.0f, 1.0f, 0.0f);

/* World variables */
const float gravity = 90000000.0f;
bool gravityState = false;

/* Ship variables */
GLfloat shipUpdateRadians = 0.02f;
glm::mat4 shipOrientationMatrix;
glm::mat4 shipTranslationMatrix;
glm::mat4 shipRotationMatrix;
glm::vec3 forward;
glm::vec3 pitch;
glm::vec3 shipUp(0.0f, 1.0f, 0.0f);
glm::vec3 shipRight(1.0f , 0.0f, 0.0f);
glm::vec3 shipLookingAt(0.0f, 0.0f, -1.0f);
float shipGravityVector = 1.76f;
int shipSpeedState = 0;
GLfloat shipSpeed[3] = { 10.0f, 50.0f, 200.0f };

/* General Missle Variables */
	// Missle has two states: leaving ship, and smart mode.
const int missleLifetime = 2000;
const int missleActivationTimer = 200;

/* Ship Missle Variables */
GLfloat shipMissleSpeed = 20;
int shipMissles = 9;
int missleUpdateFrameCount;
bool shipMissleFired = false;
bool isShipMissleSmart = false;
bool shipMissleDetectedEnemy = false;
glm::mat4 shipMissleTranslationMatrix;
glm::mat4 shipMissleRotationMatrix;
glm::mat4 shipMissleOrientationMatrix;
glm::vec3 missleDirection;
glm::vec3 shipMissleAOR;
glm::vec3 shipMissleLAT;
float radian;

/* Missle Site Variables */
int unumMissles = 5;
int secundusMissles = 5;
GLfloat radius = 25; // or 5000?
GLfloat siteMissleSpeed = 5;

/* Missle Site Missle Variables */
bool missleSiteMissleFired = false;
bool isMissleSiteSmart = false;
bool missleSiteDetectedEnemy = false;

/* Timer variables */
int timerDelay = 5, frameCount = 0; // A delay of 5 milliseconds is 200 updates / second
int timeQuantumState = 0;
int timeQuantum[4] = { 5, 40, 100, 500 }; // ace, pilot, trainee, debug TQs
double currentTime, lastTime, timeInterval;
bool idleTimerFlag = false;  // interval or idle timer ?

/* Display state and "state strings" for title display */
int timerIndex = 0;
char titleStr[175];
char fpsStr[15];
char baseStr[42] = "Warbird Simulator: {v, x, s, t, f, g, w} ";
char warbirdMissleCount[14] = "| Warbird 9";
char unumMissleCount[11] = " | Unum 5";
char secundusMissleCount[15] = " | Secundus 5";
char cameraStr[30] = "| View: Front Camera ";
char * timerStr[4] = { " | U/S 200 ", " | U/S 25 ", " | U/S 10", " | U/S 2 " };

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
		frontCamEyePosition,  // eye position (in world space)
		translatePosition[RUBERINDEX],                   // look at position (looks at the origin)
		upVector); // up vect0r (head is up, set to 0,-1,0 to look upside down)

	topCamera = glm::lookAt(
		topCamEyePosition,  // eye position
		translatePosition[RUBERINDEX],                   // look at position
		topVector); // up vect0r

	unumCamera = glm::lookAt(
		planetCamEyePosition,  // eye position
		translatePosition[UNUMINDEX], // Looks at unum assuming [1] is unum
		upVector); // up vect0r

	duoCamera = glm::lookAt(
		planetCamEyePosition,  // eye position
		translatePosition[DUOINDEX], //Looks at duo, assuming [3] is duo
		upVector); // up vect0r

	shipCamera = glm::lookAt(
		shipCamEyePosition,  // eye position
		translatePosition[SHIPINDEX],           // look at position
		upVector); // up vect0r

	mainCamera = frontCamera;

	// set render state values
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f); // Establishes what color the window will be cleared to.

// Create spaceBody objects
	//for (int i = 0; i < nModels; i++)
	//{
	//	if (i == 0 || i == nModels - 1)
	//	{
	//		spaceBody[i] = new SpaceBody(false, false);
	//	}
	//	else if (i > 0 && i < 3)
	//	{
	//		spaceBody[i] = new SpaceBody(true, false);
	//	}
	//	else if (i == 3)
	//	{
	//		spaceBody[i] = new SpaceBody(true, true);
	//	}
	//}

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
	strcat(titleStr, warbirdMissleCount);
	strcat(titleStr, unumMissleCount);
	strcat(titleStr, secundusMissleCount);
	strcat(titleStr, timerStr[timerIndex]);
	strcat(titleStr, fpsStr);
	strcat(titleStr, cameraStr);
	glutSetWindowTitle(titleStr);
}

void fireMissle()
{
	if(shipMissleFired == false)
	{
		if (shipMissles > 0)
		{
			shipMissleFired = true;
			shipMissleTranslationMatrix = shipTranslationMatrix;
			missleDirection = getIn(shipRotationMatrix) * shipMissleSpeed;
			shipMissles--;
			strcpy(warbirdMissleCount, "| Warbird ");
			strcat(warbirdMissleCount, std::to_string(shipMissles).c_str());
		}
			// The following is for initially rotating the missle model the correct way:
			//shipMissleRotationMatrix = glm::lookAt(getPosition(shipMissleTranslationMatrix), glm::vec3(shipPosition.x, camPosition.y, shipPosition.z), upVector);

		//	// NPC vector is the NPC's "looking at" vector, its direction of movement.
		//	shipMissleLAT = getIn(shipRotationMatrix);

		//	// Target Vector = target's position - NPC's position, the desired new "looking at" for the NPC.
		//	glm::vec3 targetVector = getPosition(shipOrientationMatrix) - getPosition(shipMissleTranslationMatrix);

		//	shipMissleAOR = glm::cross(targetVector,  shipMissleLAT); // axis of rotation
		//	float shipMissleAORDirection = shipMissleAOR.x + shipMissleAOR.y + shipMissleAOR.z;
		//	float aCos = glm::dot(targetVector, shipMissleLAT); // rotation amount

		//	// rotations <- pi radians (0 to 180 degrees) crossProduct in + direction
		//	// rotations > pi radians crossProduct in – (negative) direction
		//	if (shipMissleAORDirection >= 0) // adjust rotational value
		//		radian = aCos;
		//	else
		//		radian = 2 * PI - aCos; // - aCos negative rotation

		//	//shipMissleRotationMatrix = glm::rotate(shipMissleRotationMatrix, radian, shipMissleAOR);

		//	shipMissleRotationMatrix = glm::rotate(shipMissleRotationMatrix, glm::dot(getIn(shipMissleTranslationMatrix), getIn(shipRotationMatrix)), glm::cross(getIn(shipRotationMatrix),getIn(shipRotationMatrix)));

		else
			; // Do Nothing
	}
}

void handleSmartMissle()
{
	// NPC vector is the NPC's "looking at" vector, its direction of movement.
	//shipMissleLAT = getIn(shipRotationMatrix);

	//// Target Vector = target's position - NPC's position, the desired new "looking at" for the NPC.
	//glm::vec3 targetVector = getPosition(shipOrientationMatrix) - getPosition(shipMissleTranslationMatrix);

	//shipMissleAOR = glm::cross(targetVector,  shipMissleLAT); // axis of rotation
	//float shipMissleAORDirection = shipMissleAOR.x + shipMissleAOR.y + shipMissleAOR.z;
	//float aCos = glm::dot(targetVector, shipMissleLAT); // rotation amount

	//// rotations <- pi radians (0 to 180 degrees) crossProduct in + direction
	//// rotations > pi radians crossProduct in – (negative) direction
	//if (shipMissleAORDirection >= 0) // adjust rotational value
	//	radian = aCos;
	//else
	//	radian = 2 * PI - aCos; // - aCos negative rotation

	//shipMissleRotationMatrix = glm::rotate(shipMissleRotationMatrix, radian, shipMissleAOR);
	;
}

void gravitySwitch()
{
	gravityState = !gravityState;
	if (gravityState == true)
	{
		;
	}
	else
		;
}

/*
	Display() callback is required by freeglut.
	It is invoked whenever OpenGL determines a window has to be redrawn.
*/
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Actually clears the window to color specified in glClearColor().

/* 
	Final step in preparing the data for processing by OpenGL is to specify which vertex
	attributes will be issued to the graphics pipeline. 
*/

// Associate shader variables with vertex arrays:
	for (int index = 0; index < nModels; index++)
	{
		glBindVertexArray(VAO[index]); // set model for its instance. Have to rebind everytime its changed.
		translationMatrix[index] = glm::translate(identityMatrix, translatePosition[index]);
		switch (index)
		{
			case RUBERINDEX: // If it's Ruber don't apply an orbital rotation.
				modelMatrix[index] = translationMatrix[index] *
					glm::scale(identityMatrix, glm::vec3(scale[index]));
				break;

			case UNUMINDEX: // If it's planet Unum (planet closest to Ruber with no moons):
				transformMatrix[index] = rotationMatrix * translationMatrix[index];
				modelMatrix[index] = transformMatrix[index] *
					glm::scale(identityMatrix, glm::vec3(scale[index]));

				if (currentCamera == DUOCAMERAINDEX) // Update Duo's Camera:
				{
					duoCamera = glm::lookAt(getPosition(glm::translate(transformMatrix[DUOINDEX], planetCamEyePosition)), getPosition(transformMatrix[DUOINDEX]), upVector);
					mainCamera = duoCamera;
				}
				break;

			case DUOINDEX: // If it's planet Duo (planest farthest from Ruber with moons Secundus and Primus):
				transformMatrix[index] = moonRotationMatrix * translationMatrix[index];
				modelMatrix[index] = transformMatrix[index] *
					glm::scale(identityMatrix, glm::vec3(scale[index])); // oribital rotation = rotationMatrix * translationMatrix
				// For Debugging:
				//showMat4("rotation", rotationMatrix);
				//showMat4("transform", transformMatrix[index]);
				if (currentCamera == UNUMCAMERAINDEX) // Update Unum's Camera:
				{
					unumCamera = glm::lookAt(getPosition(glm::translate(transformMatrix[UNUMINDEX], planetCamEyePosition)), getPosition(transformMatrix[UNUMINDEX]), upVector);
					mainCamera = unumCamera;
				}
				break;

			case PRIMUSINDEX: // If its Primus, one of the moons, orbit around planet Duo.
				transformMatrix[PRIMUSINDEX] = transformMatrix[DUOINDEX] * rotationMatrix * glm::translate(identityMatrix, (translatePosition[PRIMUSINDEX] - translatePosition[DUOINDEX]));
				modelMatrix[PRIMUSINDEX] = transformMatrix[PRIMUSINDEX] *
					glm::scale(identityMatrix, glm::vec3(scale[PRIMUSINDEX]));
				// For Debugging:
				//showMat4("rotation", moonRotationMatrix);
				//showMat4("transform", transformMatrix[index]);
				break;

			case SECUNDUSINDEX: // If its Secundus, one of the moons, orbit around planet Duo.
				transformMatrix[SECUNDUSINDEX] = transformMatrix[DUOINDEX] * moonRotationMatrix * glm::translate(identityMatrix, (translatePosition[SECUNDUSINDEX] - translatePosition[DUOINDEX]));
				modelMatrix[SECUNDUSINDEX] = transformMatrix[SECUNDUSINDEX] *
					glm::scale(identityMatrix, glm::vec3(scale[SECUNDUSINDEX]));
				break;

			case SHIPINDEX:
				modelMatrix[index] = shipTranslationMatrix * translationMatrix[index] * shipRotationMatrix *
					glm::scale(identityMatrix, glm::vec3(scale[index]));

				if (currentCamera == SHIPCAMERAINDEX) //If we're on ship camera
				{
					camPosition = getPosition(glm::translate(modelMatrix[SHIPINDEX], shipCamEyePosition));
					shipPosition = getPosition(modelMatrix[SHIPINDEX]);
					shipCamera = glm::lookAt(camPosition, glm::vec3(shipPosition.x, camPosition.y, shipPosition.z), upVector);
					mainCamera = shipCamera;
				}
				break;

			case MISSILEINDEX:
				if (shipMissleFired == true)
				{
					// Missle is alive:
					if (missleUpdateFrameCount <= missleLifetime)
					{
						// Missle is active
						if (missleUpdateFrameCount >= missleActivationTimer)
						{
							handleSmartMissle();
						}
						modelMatrix[MISSILEINDEX] = shipMissleTranslationMatrix * translationMatrix[MISSILEINDEX] *
							shipMissleRotationMatrix * glm::scale(identityMatrix, glm::vec3(scale[MISSILEINDEX]));
					}
					
					// Missle is dead:
					else
					{
						shipMissleFired = false;
						missleUpdateFrameCount = 0;
						shipMissleTranslationMatrix = identityMatrix;
						modelMatrix[MISSILEINDEX] = shipMissleTranslationMatrix *
							glm::scale(identityMatrix, glm::vec3(scale[MISSILEINDEX]));
					}
				}
			break;

			case FIRSTMISSLESILOINDEX:
				transformMatrix[index] = glm::translate(transformMatrix[UNUMINDEX], glm::vec3(0,140,0));
				modelMatrix[index] = transformMatrix[index] *
					glm::scale(identityMatrix, glm::vec3(scale[index]));
				break;

			case SECONDMISSLESILOINDEX:
				transformMatrix[index] = glm::translate(transformMatrix[DUOINDEX], glm::vec3(0, 410, 0));
				modelMatrix[index] = transformMatrix[index] *
					glm::scale(identityMatrix, glm::vec3(scale[index]));
				break;

			default:
				break;
		}

		viewMatrix = mainCamera;
		ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix[index];
		glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
		glDrawArrays(GL_TRIANGLES, 0, nVertices[index]);  // Initializes vertex shader, for contiguous groups of vertices.
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
	glutTimerFunc(timeQuantum[timeQuantumState], update, 1); // glutTimerFunc(time, fn, arg). This sets fn() to be called after time millisecond with arg as an argument to fn().
	rotationMatrix = glm::rotate(rotationMatrix, radians, rotationalAxis);
	moonRotationMatrix = glm::rotate(moonRotationMatrix, radians2, rotationalAxis);

	if (shipMissleFired == true)
	{
		shipMissleTranslationMatrix = glm::translate(shipMissleTranslationMatrix, missleDirection);
		missleUpdateFrameCount++;
	}
	//for (int i = 0; i < nModels; i++)
	//{
	//	spaceBody[i]->update(radians, rotationalAxis);
	//}
	glutPostRedisplay();
}

// Estimate FPS, use for fixed interval timer driven animation
void intervalTimer(int i)
{
	glutTimerFunc(timeQuantum[timeQuantumState], intervalTimer, 1);
	if (!idleTimerFlag)
	{
		update(1);  // fixed interval timer
	}
}

void switchCamera(int camera)
{
	switch (camera)
	{
	case FRONTCAMERAINDEX:
		mainCamera = frontCamera;
		break;
	case TOPCAMERAINDEX:
		mainCamera = topCamera;
		break;
	case SHIPCAMERAINDEX:
		mainCamera = shipCamera;
		break;
	case UNUMCAMERAINDEX:
		mainCamera = unumCamera;
		break;
	case DUOCAMERAINDEX:
		mainCamera = duoCamera;
		break;
	default:
		return;
	}
	sprintf(cameraStr, "| View: %s", cameraNames[camera]);
	printf("Current Camera: %s\n", cameraNames[camera]);
	display();
}

void updateCamera(int camera)
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
			currentCamera = 5;
		currentCamera = (currentCamera - 1) % maxCameras;
		switchCamera(currentCamera);
		break;
	case 's': case'S':
		if (shipSpeedState >= 2)
			shipSpeedState = 0;
		else
			shipSpeedState++;
		break;
	case 't': case'T':
		if (timeQuantumState >= 3)
		{
			timeQuantumState = 0;
			timerIndex = 0;
		}
		else
		{
			timeQuantumState++;
			timerIndex++;
		}
		break;
	case 'f': case'F':
		fireMissle();
		break;
	case 'g': case'G':
		gravitySwitch();
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
			shipRotationMatrix = glm::rotate(shipRotationMatrix, shipUpdateRadians, shipRight);
		}
		else // ship forward = positive step on "at" vector
		{
			forward = getIn(shipOrientationMatrix) * shipSpeed[shipSpeedState];
			shipTranslationMatrix = glm::translate(shipTranslationMatrix, forward);
			shipOrientationMatrix = shipTranslationMatrix * shipRotationMatrix;
		}
		break;
	case GLUT_KEY_DOWN:
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
		{
			shipRotationMatrix = glm::rotate(shipRotationMatrix, shipUpdateRadians, -shipRight);
		}
		else // ship backward = negative step on "at" vector
		{
			forward = getIn(shipOrientationMatrix) * shipSpeed[shipSpeedState];
			shipTranslationMatrix = glm::translate(shipTranslationMatrix, -forward);
			shipOrientationMatrix = shipTranslationMatrix * shipRotationMatrix;
		}
		break;
	case GLUT_KEY_LEFT:
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
		{
			shipRotationMatrix = glm::rotate(shipRotationMatrix, shipUpdateRadians, shipLookingAt);
		}
		else // ship yaws "right" = rotate 0.02 radians on "up" vector
		{
			shipRotationMatrix = glm::rotate(shipRotationMatrix, shipUpdateRadians, rotationalAxis);
		}
		break;
	case GLUT_KEY_RIGHT: 
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
		{
			shipRotationMatrix = glm::rotate(shipRotationMatrix, shipUpdateRadians, -shipLookingAt);
		}
		else // ship yaws "left" = rotate -0.02 radians on "up" vector
		{
			shipRotationMatrix = glm::rotate(shipRotationMatrix, shipUpdateRadians, -rotationalAxis);
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
	glutTimerFunc(timeQuantum[timeQuantumState], update, 1); // glutTimerFunc(time, fn, arg). This sets fn() to be called after time millisecond with arg as an argument to fn().

	glutMainLoop();  // This call passes control to enter GLUT event processing cycle.

	printf("done\n");
	return 0;
}
