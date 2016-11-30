/*
File: Warbird.hpp

Description: Class that handles the initialization and updates of the Warbird.
*/

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

class Warbird : public Object3D
{

protected:
	glm::vec3 distance;
	glm::vec3 pitchVector;
	glm::vec3 initialPosition;

	float speed;

	int step; // the direction the warbird will be moving: positive or negative
	int pitch, roll, yaw; // Determines the axis the warbird will orientate on.
	int missles = 9;

	bool alive;

public:
	/* Constructor */
	Warbird(float modelSize, float modelBoundingRadius, glm::vec3 passedInitialPosition)
		: Object3D(modelSize, modelBoundingRadius) 
	{
		initialPosition = passedInitialPosition;

		// Set the initial speed and the variables that will determine the movement and rotation
		speed = 10.0f;
		step = 0;
		pitch = 0;
		roll = 0;
		yaw = 0;

		alive = true;
	}


	/* Method that returns the number of missiles the warbird has.
	*/
	int getNumberOfMissiles() 
	{
		return missles;
	}


	/* Method returns true if the warbird is alive, otherwise false
	*/
	bool isAlive() 
	{
		return alive;
	}


	// Change warbird's movement speed.
	void setSpeed(float newSpeed) 
	{
		speed = newSpeed;
	}

	float getSpeed()
	{
		return speed;
	}

	/* Method to change the step value of the warbird. If given a positive
	value the warbird will move forward. If given a negative value the
	warbird will move backwards.
	*/
	void setMove(int value) 
	{
		step = value;
	}

	/* Method to set the pitch value of the warbird, If the value is a
	positive value of 1, the warbird will rotate about the x axis in
	an upward direction. If the value is a negative value of 1, the
	warbird will rotate about the x axis in a downward direction.
	*/
	void setPitch(int newPitch) 
	{
		pitch = newPitch;
	}

	/* Method to set the roll value of the warbird, If the value is a
	positive value of 1, the warbird will rotate about the z axis to
	the right. If the value is a negative value of 1, the warbird will
	rotate about the z axis in a to the left.
	*/
	void setRoll(int newRoll) 
	{
		roll = newRoll;
	}

	/* Method to set the pitch value of the warbird, If the value is a
	positive value of 1, the warbird will rotate about the y axis in
	to the right. If the value is a negative value of 1, the warbird
	will rotate about the y axis to the left.
	*/
	void setYaw(int newYaw) 
	{
		yaw = newYaw;
	}

	/* Method that changes the current position of the warbird
	to a new position without affecting the orientation of
	the warbird.
	*/
	void setPosition(glm::vec3 newPosition) 
	{
		orientationMatrix[3][0] = newPosition.x;
		orientationMatrix[3][1] = newPosition.y;
		orientationMatrix[3][2] = newPosition.z;
	}

	/* Method to reduce the number of missiles that warbird has, the
	number of missiles can not go lower than 0
	*/
	void reduceMissileCount() 
	{
		if (missles > 0)
			missles = missles - 1;
	}

	/* Method sets the location of the object in the center of the
	world and deactivates the warbird.
	*/
	void destroy() 
	{
		orientationMatrix = identity;
		alive = false;
	}


	/* Method to reset the warbird to its default state at the start
	of the simulation.
	*/
	void restart() 
	{
		alive = true;
		orientationMatrix = glm::translate(orientationMatrix, initialPosition);
		missles = 9;
	}



	/* Method that updates the location of an object and
	rotates the object by a given amount in radians if any
	rotation is set to occur.
	*/
	void update() 
	{

		if (!alive) {
			return;
		}

		// The distance the warbird will travel
		// if step is 0, there will be no translation of the warbird.
		distance = glm::vec3(0, 0, step * speed);

		// Determine the rotation axis of the warbird
		rotationAxis = glm::vec3(pitch, yaw, roll);

		// Set the rotationMatrix to the identity Matrix
		// The warbird, by default, will not have any rotation.
		rotationMatrix = identity;

		// If any of these values have been set to a value that is not
		// 0, then the warbird will rotate about an axis a set amount of
		// radians.
		if ((pitch != 0) || (yaw != 0) || (roll != 0))
		{
			rotationMatrix = glm::rotate(identity, rotationAmount, rotationAxis);
		}

		translationMatrix = glm::translate(identity, distance);

		// Update the location of the object
		orientationMatrix = orientationMatrix * translationMatrix * rotationMatrix;

		// Reset the values back to their default
		step = pitch = yaw = roll = 0;
	}
};