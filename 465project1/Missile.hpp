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
	glm::mat4 targetLocation;

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

	/* Handles "removing" the missle from the 3D scene */
	void destroy() 
	{
		smart = fired = targetLocked = false;
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
		return target;
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

	void setUpdateFrameCount(int passedUpdateFrameCount)
	{
		updateFrameCount = passedUpdateFrameCount;
	}

	// Method that handles the logic for when a missle becomes smart.
	void handleSmartMissle(glm::mat4 unumMissileSiloTransformMatrix, glm::mat4 duoMissileSiloTransformMatrix)
	{
		location = getPosition(translationMatrix);
		float missleSiteUnum = glm::distance(location, getPosition(unumMissileSiloTransformMatrix));
		float missleSiteSecundus = glm::distance(location, getPosition(duoMissileSiloTransformMatrix));

		if (smart && targetLocked)
		{
			target = getPosition(targetLocation) - getPosition(orientationMatrix);
			location = getIn(orientationMatrix);
		}
		// Determine which missile site is closer. Player's missile should target the closest one.
		if (missleSiteUnum > 5000.0f && missleSiteSecundus > 5000.0f)
			translationMatrix = glm::translate(translationMatrix, direction * speed);
		else
		{
			if (missleSiteUnum <= missleSiteSecundus)
			{
				printf("Ship Missle Target = Unum\n");

				// Get the position of the target:
				glm::vec3 targetPosition = getPosition(unumMissileSiloTransformMatrix);

				// Get the "looking at" vector from the chaser (missle)
				direction = getIn(rotationMatrix);

				// Get the distance between the target and the missle.
				glm::vec3 distance = targetPosition - location;

				// Normalize the vectors:
				distance = glm::normalize(distance);
				direction = glm::normalize(direction);

				if (!(colinear(distance, direction, 0.1f) || glm::distance(distance, glm::vec3(0, 0, 0)) == 0))
				{
					// Find the axis of rotation:
					AOR = glm::cross(distance, direction);
					AORDirection = AOR.x + AOR.y + AOR.z;

					// Get the rotation Amount of the Missile and Determine the Direction of Rotation.
					// This equation allows to get the angle of rotation between the two vectors,
					// The dot product of two vectors equals |A|*|B|*cos(angle), so to get the angle in between
					// divide by |A|*|B|.
					if (AORDirection > 0) {
						rotationAmount = -glm::acos(glm::dot(direction, targetPosition) /
							(glm::abs(glm::distance(targetPosition, glm::vec3(0, 0, 0))) * glm::abs(glm::distance(direction, glm::vec3(0, 0, 0)))));
					}



					else
					{
						rotationAmount = glm::acos(glm::dot(direction, targetPosition) /
							(glm::abs(glm::distance(targetPosition, glm::vec3(0, 0, 0))) * glm::abs(glm::distance(direction, glm::vec3(0, 0, 0)))));
					}
				}

				// Only rotate the Missile only a 4th of the rotation amount, 
				// this allows for smoother rotations in the simulation.
				rotationMatrix = glm::rotate(identity, rotationAmount / 4, AOR);

				translationMatrix = glm::translate(translationMatrix, direction * speed);

				// Update Orientation Of the Missle
				orientationMatrix = orientationMatrix * translationMatrix * rotationMatrix;
			}
			//else
			//{
			//	printf("Ship Missle Target = Secundus\n");

			//	// Get the position of the target:
			//	glm::vec3 targetPosition = getPosition(transformMatrix[SECONDMISSLESILOINDEX]);

			//	// Get the "looking at" vector from the chaser (missle)
			//	shipMissleDirection = getIn(shipMissleOrientationMatrix);

			//	// Get the distance between the target and the missle.
			//	glm::vec3 distance = targetPosition - shipMissleLocation;

			//	// Normalize the vectors:
			//	distance = glm::normalize(distance);
			//	shipMissleDirection = glm::normalize(shipMissleDirection);

			//	// Find the axis of rotation:
			//	shipMissleAOR = glm::cross(distance, shipMissleDirection);

			//	// Normalize the Axis:
			//	shipMissleAOR = glm::normalize(shipMissleAOR);

			//	if (colinear(distance, shipMissleDirection, 0.1f))
			//	{
			//		radian = (2.0f * PI) - acos(glm::dot(distance, shipMissleDirection));
			//	}
			//	else
			//	{
			//		radian = (2.0f * PI) + acos(glm::dot(distance, shipMissleDirection));
			//	}


			//	// Normalize the Axis:
			//	shipMissleAOR = glm::normalize(shipMissleAOR);

			//	shipMissleRotationMatrix = glm::rotate(shipMissleRotationMatrix, radian, shipMissleAOR);
			//	shipMissleTranslationMatrix = glm::translate(shipMissleTranslationMatrix, shipMissleDirection * shipMissleSpeed);
		}
	}

	/* Updates the direction of the missle and calls handle smart missile function */
	//void update() 
	//{
	//	if (fired == true)
	//	{
	//		// If missle is active, update smart missle.
	//		if (updateFrameCount >= missileActivationTimer)
	//		{
	//			handleSmartMissle();
	//		}
	//		else // Keep going in the direction its going from the ship.
	//		{
	//			translationMatrix = glm::translate(translationMatrix, direction * speed);
	//		}
	//		updateFrameCount++;
	//	}
	//}
};