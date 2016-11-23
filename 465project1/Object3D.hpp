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
	glm::mat4 modelMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 translationMatrix;
	glm::mat4 orientationMatrix;
	glm::mat4 transformMatrix;
	glm::mat4 identity;
	glm::vec3 scale;
	glm::vec3 rotationalAxis;
	glm::vec3 position;
	float rotationAmount;
	float modelSize;
	float modelBoundingRadius;
	bool orbital, planetOrbital;

public:
	Object3D(bool passedOrbital, bool passedPlanetOrbital)
	{
		this->orbital = passedOrbital;
		this->planetOrbital = passedPlanetOrbital;
		rotationMatrix = identity;  // no initial orientation
		translationMatrix = identity; // no initial position
	}

	glm::mat4 getModelMatrix() 
	{
		return this->modelMatrix;
	}

	void setModelMatrix()
	{
		if (orbital) // orbital rotation
		{
			this->transformMatrix = rotationMatrix * translationMatrix; // update the transform matrix.
			this->modelMatrix = transformMatrix * scaleMatrix;
		}
			
		else if (planetOrbital)
		{
			this->transformMatrix = transformMatrix * scaleMatrix;
		}
		else
		{
			this->modelMatrix = translationMatrix * scaleMatrix; // update the transform matrix.
		}
	}

	bool getOrbital()
	{
		return this->orbital;
	}

	bool getPlanetOrbital()
	{
		return this->planetOrbital;
	}

	glm::mat4 getRotationMatrix()
	{
		return this->rotationMatrix;
	}

	void setRotationMatrix(glm::mat4 passedRotationMatrix)
	{
		this->rotationMatrix = passedRotationMatrix;;
	}

	void setModelBR(float passedModelBR)
	{
		this->modelBoundingRadius = passedModelBR;
	}

	float getModelBR()
	{
		return this->modelBoundingRadius;
	}

	void setScale(glm::vec3 passedScale)
	{
		this->scale = passedScale;
	}

	void setScaleMatrix(glm::mat4 passedScalingMatrix)
	{
		this->scaleMatrix = passedScalingMatrix;
	}

	glm::mat4 getScaleMatrix()
	{
		return this->scaleMatrix;
	}

	glm::vec3 getScale()
	{
		return this->scale;
	}
	
	glm::mat4 getTranslationMatrix()
	{
		return this->translationMatrix;
	}

	void setTranslationMatrix(glm::mat4 passedTranslationMatrix)
	{
		this->translationMatrix = passedTranslationMatrix;
	}

	void setTransformMatrix(glm::mat4 passedTransformMatrix)
	{
		this->translationMatrix = passedTransformMatrix;
	}

	glm::mat4 getTransformMatrix()
	{
		return this->transformMatrix;
	}

	void update(float passedRadians, glm::vec3 passedRotationAxis)
	{
		rotationMatrix = glm::rotate(rotationMatrix, passedRadians, passedRotationAxis);
		//translationMatrix = glm::translate(translationMatrix, translation);
	}
};
