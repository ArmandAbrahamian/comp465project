/*
File: Missile.hpp

Description: Class that handles the intitialization and updates of a missile.
*/

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

# include <glm/gtc/quaternion.hpp> 
# include <glm/gtx/quaternion.hpp>

class Missile : public Object3D
{

// Variables:
private:

	float speed;
	float AORDirection;
	int updateFrameCount;
	float detectionRadius = 5000.0f; // or 25?
	bool smart;		// Flag to check if the missile becomes smart
	bool targetLocked; // Flag for if a target was locked
	bool fired; // Flag to check if the missle has fired
	glm::vec3 translationAmount;	// the x,y,z position the object will be translated by
	glm::vec3 direction;
	glm::vec3 AOR;
	glm::vec3 targetVector;
	glm::vec3 distance;
	glm::vec3 missileVector;
	glm::mat4 targetMatrixLocation;
	glm::mat4 missileLocation;

	const int missleLifetime = 2000;
	const int missileActivationTimer = 200;

	// Creates an identity quaternion (no rotation)
	glm::quat MyQuaternion;

// Constructor and functions:
public:

	Missile(float modelSize, float modelBoundingRadius, float passedMissleSpeed)
		: Object3D(modelSize, modelBoundingRadius)
	{
		smart = false; // the missle isn't initially smart.
		targetLocked = false; // the missle doesn't have a target until it becomes smart.
		fired = false; // The missile hasn't been fired yet.
		updateFrameCount = 0;
		AORDirection = 0;
		speed = passedMissleSpeed; 
	}

	/* Handles "removing" the missile from the 3D scene */
	void destroy() 
	{
		smart = false;
		fired = false;
		targetLocked = false;
		updateFrameCount = 0;
		setTranslationMatrix(identity);
		setOrientationMatrix(translationMatrix);
	}

	int getUpdateFrameCount()
	{
		return updateFrameCount;
	}

	glm::vec3 getTargetLocation()
	{
		return targetVector;
	}

	glm::vec3 getDirection()
	{
		return direction;
	}

	float getSpeed()
	{
		return speed;
	}

	/* Checks the "smart" state of the missle */
	bool isSmart()
	{
		return smart;
	}

	bool isTargetLocked()
	{
		return targetLocked;
	}

	bool hasFired()
	{
		return fired;
	}

	void fireMissile()
	{
		fired = true;
	}

	void activateSmart()
	{
		smart = true;
	}

	void setDirection(glm::vec3 passedInDirection)
	{
		direction = passedInDirection;
	}

	glm::mat4 getTargetMatrixLocation()
	{
		return targetMatrixLocation;
	}

	void setTargetLocation(glm::mat4 newLocation) 
	{
		targetMatrixLocation = newLocation;
		targetLocked = true;
	}

	void update() 
	{

		// Initialy the missile does not rotate or translate
		rotationMatrix = identity;
		translationMatrix = identity;

		// If the missile has fired, start moving the missile and check its lifespan
		if (fired == true)
		{
			// Keep Count of the Number of Updates
			updateFrameCount = updateFrameCount + 1;

			// The distance the missile will travel
			distance = glm::vec3(0, 0, -speed);
			translationMatrix = glm::translate(identity, distance);

			// If the Missile exceeds its lifespan, destroy it
			if (updateFrameCount > missleLifetime) 
			{
				destroy();
			}

			// The Missile will only attempt to reorient itself if it has a target and is smart
			if (smart && targetLocked) 
			{
				targetVector = getPosition(targetMatrixLocation) - getPosition(orientationMatrix); // Gets the distance the target is from the missile
				missileVector = getIn(orientationMatrix); // Gets the direction the missile is going.

				// Normalize the vectors
				targetVector = glm::normalize(targetVector);
				missileVector = glm::normalize(missileVector);

				// If the two vectors are colinear, do not attempt to do any rotations
				if (!(colinear(missileVector, targetVector, 0.1)))
				{
					// The rotation axis the missile will be rotating about
					AOR = glm::cross(missileVector, targetVector);
					AOR = glm::normalize(AOR);
					
					AORDirection = AOR.x + AOR.y + AOR.z;

					if (AORDirection <= 0)
					{
						rotationAmount = -glm::acos(glm::dot(targetVector, missileVector));;
					}
					else
					{
						rotationAmount = 2 * PI + glm::acos(glm::dot(targetVector, missileVector));;
					}

					rotationAxis = AOR;

					// Conversion from Euler angles (in radians) to Quaternion
					//MyQuaternion = glm::quat(rotationAxis);

					MyQuaternion = glm::angleAxis(rotationAmount, rotationAxis);

					rotationAxis = glm::axis(MyQuaternion);

					rotationAmount = glm::angle(MyQuaternion);

					// Only rotate the Missile only a 4th of the rotation amount, 
					// this allows for smoother rotations in the simulation.
					rotationMatrix = glm::rotate(identity, rotationAmount, rotationAxis);
				}
			}
		}

		// Update the orientation matrix of the missile
		orientationMatrix = orientationMatrix * translationMatrix * rotationMatrix;
	}
};