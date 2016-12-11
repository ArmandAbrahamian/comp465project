/*
File: Object3D.hpp

Description: Represent the scale, translation, and rotation of a 3D object.

*/

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

class Object3D 
{

protected:

	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 translationMatrix;
	glm::mat4 orientationMatrix;
	glm::mat4 modelMatrix;
	glm::mat4 identity;
	glm::vec3 scale;
	glm::vec3 rotationAxis;
	float rotationAmount;
	float modelSize;
	float modelBoundingRadius;
	bool orbit = false;

public:

	// Constructor
	Object3D(float modelSize, float modelBoundingRadius)
	{
		// Scale the model to the desired size
		scale = glm::vec3(modelSize / modelBoundingRadius);
		scaleMatrix = glm::scale(glm::mat4(), glm::vec3(scale));

		identity = glm::mat4();		// initialize the identity matrix

		// No initial rotation, orientation, or translation
		rotationMatrix = identity;
		orientationMatrix = identity;
		translationMatrix = identity;

		modelMatrix = identity;

		rotationAxis = glm::vec3(0, 1, 0);
	}

	// Returns the model matrix for the 3D object.
	glm::mat4 getModelMatrix()
	{
		return orientationMatrix * scaleMatrix;
	}
	
	// Returns the rotational matrix for the 3D object.
	glm::mat4 getRotationMatrix()
	{
		return rotationMatrix;
	}

	// Returns the orientation matrix for the 3D object.
	glm::mat4 getOrientationMatrix() 
	{
		return orientationMatrix;
	}

	glm::mat4 getTranslationMatrix()
	{
		return translationMatrix;
	}

	float getRotationAmount()
	{
		return rotationAmount;
	}

	// Translates the translation matrix by a passed in amount.
	void setTranslationMatrix(glm::vec3 passedTranslation)
	{
		translationMatrix = glm::translate(translationMatrix, passedTranslation);
	}

	// Sets the translation matrix to an outside matrix.
	void setTranslationMatrix(glm::mat4 passedTranslationMatrix)
	{
		translationMatrix = passedTranslationMatrix;
	}
	
	// Sets the rotation matrix to an outside matrix.
	void setRotationMatrix(glm::mat4 passedRotationMatrix)
	{
		rotationMatrix = passedRotationMatrix;
	}

	// Sets the rotational rate for the 3D object.
	void setRotationAmount(float passedRadianAmount)
	{
		rotationAmount = passedRadianAmount;
	}

	// Sets the orientational matrix.
	void setOrientationMatrix(glm::mat4 newOrientation) 
	{
		orientationMatrix = newOrientation;
	}

	// Sets the object flag to a planet orbit.
	void setOrbit()
	{
		orbit = true;
	}

	/* Changes the position of the object 
	to a new position without affecting it's orientation.
	*/
	void setPosition(glm::vec3 newPosition)
	{
		orientationMatrix[3][0] = newPosition.x;
		orientationMatrix[3][1] = newPosition.y;
		orientationMatrix[3][2] = newPosition.z;
	}

	// Updates the rotation and orientation matrix.
	void update()
	{
		rotationMatrix = glm::rotate(rotationMatrix, rotationAmount, rotationAxis);

		// Set the orientation matrix based on what type of object it is:
		if (orbit == true)
		{
			orientationMatrix = rotationMatrix * translationMatrix;
		}
		else
		{
			orientationMatrix = translationMatrix * rotationMatrix;
		}
	}
};
