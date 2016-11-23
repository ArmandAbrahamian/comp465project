/*
Missile.hpp
Class that handles the intitialization and updates of the missile.
*/

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

//# ifndef __Object3D__
//# include "Object3D.hpp"
//# endif

class Missile
{

private:
	glm::vec3 translationAmount;	// the x,y,z position the object will be translated by
	int speed;
	int updateFrameCount;
	bool smart = false;
	bool targetDetected = false;
	float radian;
	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;
	glm::vec3 direction;
	glm::vec3 target;
	float detectionRadius = 5000; // or 25?

public:

	/* Constructor */
	Missile(glm::mat4 passedTranslationMatrix,glm::mat4 passedRotationMatrix, glm::vec3 passedDirection, int passedMissleSpeed)
	{
		this->translationMatrix = passedTranslationMatrix;
		this->rotationMatrix = passedRotationMatrix;
		this->direction = passedDirection;
		this->speed = passedMissleSpeed;
	}

	/* Method returns true if the missile becomes smart, otherwise false */
	bool isSmart() 
	{
		return smart;
	}

	glm::mat4 getTranslationMatrix()
	{
		return this->translationMatrix;
	}

	void setTranslationMatrix(glm::mat4 passedTranslationMatrix)
	{
		this->translationMatrix = passedTranslationMatrix;
	}

	void setRotationMatrix(glm::mat4 passedRotationMatrix)
	{
		this->rotationMatrix = passedRotationMatrix;;
	}

	glm::mat4 getRotationMatrix()
	{
		return this->rotationMatrix;
	}

	/* Method to set the position of the object with the
	given 3 dimension vertex (x, y, z). */
	void setPosition(glm::vec3 newPosition) 
	{

	}


	/* Method to set the Missile Site to inactive,
	Missile is inactive if destroyed/ hit by a missile */
	void destroy() 
	{
		
	}



	/* Method that updates the orientation of an object by
	translation and rotation depending on translationAmount,
	radians, and rotationAxis.
	*/
	void update() 
	{

			// If missle is active, update smart missle.
			if (missleUpdateFrameCount >= missleActivationTimer)
			{
				handleSmartMissle();
			}
			else // Keep going in the direction its going from the ship.
			{
				shipMissleTranslationMatrix = glm::translate(shipMissleTranslationMatrix, shipMissleDirection * shipMissleSpeed);
			}
			missleUpdateFrameCount++;

		// Get the tranlation matrix the object is orbiting 
		//translationMatrix = glm::translate(identity, translationAmount);
		// Rotate the object by a given amount in radians
		//rotationMatrix = identity;
		// Update the location of the object
		//orientationMatrix = orientationMatrix * translationMatrix * rotationMatrix;
	}
};