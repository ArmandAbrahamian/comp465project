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

/* 
Global Variables:
*/
int Index = 0; // global variable indexing into VBO arrays.

// Build each cube face from individual triangles.
// Storage required: (6 faces)(2 triangles/face)(3 vertices/triangle) = 6 * 2 * 3 = 36.
const int NumVertices = 36;

typedef glm::vec4 point4;
typedef glm::vec4 color4;

point4 vPositions[NumVertices];
color4 vColors[NumVertices];

GLuint VAO;
GLuint buffer;

char * vertexShaderFile = "simpleVertex.glsl";
char * fragmentShaderFile = "simpleFragment.glsl";
GLuint shaderProgram;

point4 positions[8] =
{
	point4(-0.5, -0.5, 0.5, 1.0),
	point4(-0.5, 0.5, 0.5, 1.0),
	point4(0.5, 0.5, 0.5, 1.0),
	point4(0.5, -0.5, 0.5, 1.0),
	point4(-0.5, -0.5, -0.5, 1.0),
	point4(-0.5, 0.5, -0.5, 1.0),
	point4(0.5, 0.5, 0.5, 1.0),
	point4(0.5, -0.5, 0.5, 1.0)
};

color4 colors[8] =
{
	color4(0.0, 0.0, 0.0, 1.0), // black
	color4(1.0, 0.0, 0.0, 1.0), // red
	color4(1.0, 1.0, 0.0, 1.0), // yellow
	color4(0.0, 1.0, 0.0, 1.0), // green
	color4(0.0, 0.0, 1.0, 1.0), // blue
	color4(1.0, 0.0, 1.0, 1.0), // magenta
	color4(1.0, 1.0, 1.0, 1.0), // white
	color4(0.0, 1.0, 1.0, 1.0) // cyan
};

void quad(int a, int b, int c, int d)
{
	vColors[Index] = colors[a]; vPositions[Index] = positions[a]; Index++;
	vColors[Index] = colors[b]; vPositions[Index] = positions[b]; Index++;
	vColors[Index] = colors[c]; vPositions[Index] = positions[c]; Index++;
	vColors[Index] = colors[a]; vPositions[Index] = positions[a]; Index++;
	vColors[Index] = colors[c]; vPositions[Index] = positions[c]; Index++;
	vColors[Index] = colors[d]; vPositions[Index] = positions[d]; Index++;
}

void colorcube()
{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}

/* 
Display callback is required by freeglut. 
It is invoked whenever OpenGL determines a window has to be redrawn.
*/
void display( void ) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Final step in preparing the data for processing by OpenGL is to specify which vertex 
	attributes will be issued to the graphics pipeline. */

	// Associate shader variables with vertex arrays:
	GLuint vPosition =
		glGetAttribLocation(shaderProgram, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	GLuint vColor =
		glGetAttribLocation(shaderProgram, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vPositions)) );

	// For contiguous groups of vertices. 
	glDrawArrays(GL_TRIANGLES, 0, NumVertices); // Initializes vertex shader.
	glDisableVertexAttribArray(0);
	glutSwapBuffers();
}

void init()
{
	// load the shader programs
	shaderProgram = loadShaders(vertexShaderFile, fragmentShaderFile);
	glUseProgram(shaderProgram);

	// generate VAOs and VBOs
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create and initialize a buffer object (VBO) with data.
	glGenBuffers(1, &buffer); // Generate a buffer's name.
	glBindBuffer(GL_ARRAY_BUFFER, buffer); // Make the buffer the "current" buffer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vPositions) + sizeof(vColors), NULL, GL_STATIC_DRAW); // Initialize the buffer.
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vPositions), vPositions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vPositions), sizeof(vColors), vColors);

	// set render state values
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 033: case'q': case'Q':
		exit(EXIT_SUCCESS);
		break;
	}
	//if (key == 'q' || key == 'Q')
	//{
	//	exit(EXIT_SUCCESS);
	//}
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
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(100, 100);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	// set OpenGL and GLSL versions to 3.3 for Comp 465/L, comment to see highest versions.
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Warbird Simulator; Use q to quit.");

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
	glutKeyboardFunc(keyboard);

	glutMainLoop();  // This call passes control to GLUT.

	printf("done\n");
	return 0;
}