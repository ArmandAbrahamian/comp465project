/*
MissileSite.hpp
Class that handles the intitialization and updates of the missile site.
*/

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

class MissileSite3D : public Object3D {

private:

	glm::vec3 translationAmount;	// the x,y,z position the object will be translated by

	int numberOfMissiles;	// the number of Missiles a Missile site has

	bool active;

public:

	/* Constructor */
	MissileSite3D(float modelSize, float modelBoundingRadius)
		: Object3D(modelSize, modelBoundingRadius) {

		// Missile sites have 5 missiles initially
		numberOfMissiles = 5;

		// The Missile Site is initialy active
		active = true;
	}


	/* Method that returns the current number of Missiles the Missile site has.
	*/
	int getNumberOfMissiles() {
		return numberOfMissiles;
	}


	/* Method returns true if the missile is active, otherwise false
	*/
	bool isActive() {
		return active;
	}


	/* Method that sets the tranlationMatrix of the object by the given
	translation in 3 dimensions, this is the desired position of the
	object from the origin
	*/
	void setTranslationAmount(glm::vec3 translation) {
		translationAmount = translation;
	}


	/* Method to set the position of the object with the
	given 3 dimension vertex (x, y, z).
	*/
	void setPosition(glm::vec3 newPosition) {
		orientationMatrix[3][0] = newPosition.x;
		orientationMatrix[3][1] = newPosition.y;
		orientationMatrix[3][2] = newPosition.z;
	}


	/* Method to set the Missile Site to inactive,
	Missile is inactive if destroyed/ hit by a missile
	*/
	void destroy() {
		active = false;
	}


	/* Method to reset the missile site to its
	default state at the start of the simulation
	*/
	void restart() {
		active = true;
		numberOfMissiles = 5;
	}


	/* Method that reduces the missile count by one.
	The missile count can not go below zero.
	*/
	void reduceMissileCount() {
		if (numberOfMissiles != 0) {
			numberOfMissiles = numberOfMissiles - 1;
		}
		else {
			active = false;
		}
	}



	/* Method that updates the orientation of an object by
	translation and rotation depending on translationAmount,
	radians, and rotationAxis.
	*/
	void update() {
		// Get the tranlation matrix the object is orbiting 
		translationMatrix = glm::translate(identity, translationAmount);
		// Rotate the object by a given amount in radians
		rotationMatrix = identity;
		// Update the location of the object
		orientationMatrix = orientationMatrix * translationMatrix * rotationMatrix;
	}
};