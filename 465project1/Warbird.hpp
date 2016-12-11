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

	/* Changes the step value of the warbird. If positive
	 the warbird will move forward. If negative the
	warbird will move backwards.
	*/
	void setMove(int value) 
	{
		step = value;
	}

	/* Sets the pitch value of the warbird, If the value is a
	positive 1, the warbird will rotate about the x axis in
	an upward direction. If the value is a negative 1, the
	warbird will rotate about the x axis in a downward direction.
	*/
	void setPitch(int newPitch) 
	{
		pitch = newPitch;
	}

	/* Sets the roll value of the warbird, If the value is a
	positive 1, the warbird will rotate about the z axis to
	the right. If the value is a negative 1, the warbird will
	rotate about the z axis in a to the left.
	*/
	void setRoll(int newRoll) 
	{
		roll = newRoll;
	}

	/* Sets the pitch value of the warbird, If the value is a
	positive 1, the warbird will rotate about the y axis in
	to the right. If the value is a negative 1, the warbird
	will rotate about the y axis to the left.
	*/
	void setYaw(int newYaw) 
	{
		yaw = newYaw;
	}

	/* Sets the location of the object in the center of the
	world and deactivates the warbird.
	*/
	void destroy() 
	{
		orientationMatrix = identity;
		alive = false;
		printf("The warbird is dead. \n");
	}


	/* Resets the warbird to its default state at the start
	of the simulation.
	*/
	void restart() 
	{
		alive = true;
		translationMatrix = glm::translate(identity, initialPosition);
		rotationMatrix = glm::rotate(identity, 0.0f, glm::vec3(0, 1, 0));
	}

	/* Updates the location of an object and
	rotates the object by a given amount in radians if any
	rotation is set to occur.
	*/
	void update() 
	{
		if (!alive) 
		{
			return;
		}

		// The distance the warbird will travel
		// if step is 0, there will be no translation of the warbird.
		distance = getIn(orientationMatrix) * (-step * speed);

		// Determine the rotation axis of the warbird
		rotationAxis = glm::vec3(pitch, yaw, roll);

		// If any of these values have been set to a value that is not
		// 0, then the warbird will rotate about an axis a set amount of
		// radians.
		if ((pitch != 0) || (yaw != 0) || (roll != 0))
		{
			rotationMatrix = glm::rotate(rotationMatrix, rotationAmount, rotationAxis);
		}

		setTranslationMatrix(distance);

		// Update the overall location of the object
		orientationMatrix = translationMatrix * rotationMatrix;

		// Reset the values back to their default
		step = pitch = yaw = roll = 0;
	}
};