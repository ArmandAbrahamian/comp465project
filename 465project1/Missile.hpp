/*
File: Missile.hpp

Description: Class that handles the intitialization and updates of a missile.
*/

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

class Missile : public Object3D
{

// Variables:
private:

	const int missleLifetime = 2000;
	const int missileActivationTimer = 200;
	float speed;
	float AORDirection;
	int updateFrameCount;
	bool smart;		// Flag to check if the missile becomes smart
	bool targetLocked; // Flag for if a target was locked
	bool fired; // Flag to check if the missle has fired
	float radian;
	glm::vec3 translationAmount;	// the x,y,z position the object will be translated by
	glm::vec3 direction;
	glm::vec3 AOR;
	glm::vec3 location;
	glm::vec3 target;
	glm::vec3 distance;
	float detectionRadius = 5000; // or 25?

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
		this->speed = passedMissleSpeed; 
	}

	/* Handles "removing" the missle from the 3D scene */
	void destroy() 
	{
		smart = fired = targetLocked = false;
		updateFrameCount = 0;
		setTranslationMatrix(identity);
		setOrientationMatrix(translationMatrix);
		printf("Ship Missle #%d Destroyed", warbird->getNumberOfMissiles() + 1);
	}

	int getUpdateFrameCount()
	{
		return updateFrameCount;
	}

	glm::vec3 getTargetLocation()
	{
		return target;
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

	void display()
	{
		// Ship Missle is alive:
		if (updateFrameCount <= missleLifetime)
		{
			// Update missle model:
			orientationMatrix = translationMatrix * rotationMatrix;

			modelMatrix[SHIPMISSILEINDEX] = translationMatrix * translationMatrix[SHIPMISSILEINDEX] * rotationMatrix * glm::scale(identityMatrix, glm::vec3(scale[SHIPMISSILEINDEX]));
		}

		// Ship Missle is dead:
		else
		{
			destroy();
		}
	}

	/* Updates the direction of the missle and calls handle smart missile function */
	void update() 
	{
		if (fired == true)
		{
			// If missle is active, update smart missle.
			if (updateFrameCount >= missileActivationTimer)
			{
				handleSmartMissle();
			}
			else // Keep going in the direction its going from the ship.
			{
				translationMatrix = glm::translate(translationMatrix, direction * speed);
			}
			updateFrameCount++;
		}
	}
};