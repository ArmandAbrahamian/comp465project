/*
Object3D.hpp

Represent the scale, translation, and rotation of a 3D shape.
If you overload the constructor you can create a shape with
arguments for scale, translation, and rotation.

*/

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

class Object3D {
private:

	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 translationMatrix;
	glm::mat4 orientationMatrix;
	glm::mat4 identity;
	glm::vec3 scale;
	glm::vec3 rotationAxis;
	float rotationAmount;
	float modelSize;
	float modelBoundingRadius;
	bool orbital, planetOrbital;

public:

	Object3D(bool passedOrbital, bool passedPlanetOrbital)
	{
		this->orbital = passedOrbital;
		this->planetOrbital = passedPlanetOrbital;
		rotationMatrix = glm::mat4();  // no initial orientation
	}

	glm::mat4 getModelMatrix() 
	{
		if (orbital) // orbital rotation
			return(rotationMatrix * translationMatrix * scaleMatrix);
		else  // center rotation
			return(translationMatrix * rotationMatrix * scaleMatrix);
	}

	bool getOrbital()
	{
		return this->orbital;
	}

	bool getPlanetOrbital()
	{
		return this->planetOrbital;
	}

	glm::mat4 getRotationalMatrix()
	{
		return this->rotationMatrix;
	}

	void update(GLfloat passedRadians, glm::vec3 passedRotationAxis)
	{
		rotationMatrix = glm::rotate(rotationMatrix, passedRadians, passedRotationAxis);
		//translationMatrix = glm::translate(translationMatrix, translation);
	}
};
