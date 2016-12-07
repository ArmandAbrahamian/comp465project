/*
includes465.hpp

Include file to set up OS compilation for 465 projects.
comment / uncomment for your operating system

Assumes the "including application) has defined one of the following
"target" operating systems.

Contains several cpp defined constants for project builds.
All *465 constants start and end with 2 underscores.

__Linux__      // Ubuntu or other distro
__Mac__        // Mac OSX 
__MinGW__      // Windows, Minimalist Gnu for Windows
__Windows__    // Windows, Visual Studio 201?)

Includes utility functions to load glsl shaders and 
AC3D *.tri models.

Mike Barnes
8/25/2016
*/

// defines and includes

# define __INCLUDES465__
# define GLM_FORCE_PURE  // for latest version of GLM w/ error C2719: ,,,  __declspec(align('16')) won't be aligned
# define BUFFER_OFFSET(x)  ((const GLvoid *) (x))  // OpenGL PG 8th ed. code, in vgl.h 
# define MAX_INFO_LOG_SIZE 2048  // for error messages in loadShaders(...)

# include <stdio.h>  // My examples use printf, I'm not a fan of cin, cout...
# include <stdlib.h>
# include <sys/stat.h>

// OS specific includes
# ifdef __Linux__
# include <GL/glew.h>
# include <GL/freeglut.h>
# endif

# ifdef __Mac__
# include <GL/glew.h>
# include <GLUT/glut.h>
# endif

# ifdef __MinGW__
// put stuff for minGW here, __Linux__ might work
# endif

# ifdef __Windows__
# include <Windows.h>  
# include <GL/glew.h>
# include <GL/freeglut.h>
# endif

// include the glm shader-like math library
# define GLM_FORCE_RADIANS  // use radians not angles
# define GLM_MESSAGES   // compiler messages
# include <glm/glm.hpp>
# include <glm/gtc/constants.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtc/type_ptr.hpp>
# include "../includes465/glmUtils465.hpp"  // print matrices and vectors, ... 
# include "../includes465/shader465.hpp"    // load vertex and fragment shaders
# include "../includes465/triModel465.hpp"  // load AC3D *.tri model 
#include "../includes465/texture.hpp"
// PI to 10 digits
const float PI = 3.14159265358f;  
 
