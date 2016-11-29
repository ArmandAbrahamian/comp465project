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

# define __Object3D__

class Object3D {

private:

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

	Object3D(float modelSize, float modelBoundingRadius)
	{
		// Scale the model to the desired size
		scale = glm::vec3(modelSize / modelBoundingRadius);
		scaleMatrix = glm::scale(glm::mat4(), glm::vec3(scale));

		identity = glm::mat4();		// initialize the identity matrix

		rotationMatrix = identity;	// no initial rotation

		orientationMatrix = identity;	// no initial orientation

		translationMatrix = identity;	// no initial translation

		modelMatrix = identity; // Don't set the model matrix just yet.

		rotationAxis = glm::vec3(0, 1, 0);
	}

	glm::mat4 getModelMatrix()
	{
		return orientationMatrix * scaleMatrix;
	}

	glm::mat4 getRotationMatrix()
	{
		return this->rotationMatrix;
	}

	glm::mat4 getOrientationMatrix() {
		return orientationMatrix;
	}

	void setTranslationMatrix(glm::vec3 passedTranslation)
	{
		translationMatrix = glm::translate(identity, passedTranslation);
	}

	void setRotationAmount(float passedRadianAmount)
	{
		rotationAmount = passedRadianAmount;;
	}

	void setOrientationMatrix(glm::mat4 newOrientation) {
		orientationMatrix = newOrientation;
	}

	void setOrbit()
	{
		orbit = true;
	}

	void update()
	{
		rotationMatrix = glm::rotate(rotationMatrix, rotationAmount, rotationAxis);
		//translationMatrix = glm::translate(translationMatrix, translation);

		// Set the orientation matrix based on what type of object it is:
		if (orbit == true)
		{
			orientationMatrix = rotationMatrix * translationMatrix;
		}
		else
		{
			orientationMatrix = translationMatrix;
		}
	}
};
