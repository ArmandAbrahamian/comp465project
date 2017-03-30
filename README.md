# Warbird Simulator

Computer graphics class semester project at CSUN using OpenGL Version 3.3, Freeglut, Glew, glm, and GLSL (Shading Language), and irrklang where the player must destroy two missile sites located on different planets.

## Phase 1
* Created Ruber Solar System
    * Sun (Ruber)
    * Plantetary Bodies that orbit Ruber:
        * Two Planets: Unum and Duo
        * Duo has two moons: Primus, and Secundus 
* Missile installations on Unum and Duo's moon Secundus. 
    
* Camera Views:
  * Front, Top, Unum, Duo, and Ship.
  
* Created and imported models for Warbird, planets, moons, and sun.

## Phase 2
### Updates from Phase 1:
1. Fixed moon orbits for Primus and Secundus on planet Duo.
1. Dynamic Cameras were added for Unum and Duo.
1. Using separate rotation matrices for each space body.
1. Refactored to OOP approach.
    1. Object3D, Warbird, Missile classes were added with their own methods and attributes.
        1. Warbird and Missile classes are child classes of Object3D with more specific functionality.
1. Huge code cleanup.

### Phase 2:
1. Missiles
    1. Tracking
        1.	Missile silo missiles acquire the Warbird as a target and tracks its location moving towards (chasing) it until it collides with it.
        1.	Warbird missile acquires either the Unum or Duo Missile Silos as targets (whichever one is closer) and tracks its location moving towards (chasing) it until it collides with it.
        1.	Tracking algorithm utilizes Dot product to figure out the amount of rotation the missile needs to do and the Cross product to figure out the axis of rotation the missile needs to rotate on.
        1.	Sometimes (rarely) the missile goes away from the target and then turns around and chases it again.
        1.	It’s somewhat difficult for the missile to collide with the Duo Missile Silo. It’s easier for it to hit the Unum Missile Silo due to its slower rotation speed. So it’s somewhat difficult to win the game.
1.	Created and utilized custom models for missiles.
    1.	Warbird Missile
        1.	When it’s fired the warbird can’t fire another one until the current missile explodes after its lifetime or when it collides with an object.
1.	Missile Silo Missiles
    1.	Each Missile silo can shoot one missile at a time and lasts for its lifetime or if it collides with an object.
1.	Missiles face its target correctly as it travels to it.
1.	Missile Silos
    1.	There are two missile silos added to the project one on Duo and one on Unum.
        1.	Model created and placed on top of each planet.
1.	Collision Detection
    1.	The warbird is able to collide with all the objects in the space.
    1.	The missiles collide with all the objects in the space.
        1.	Missile collision only applies when it becomes smart (activates tracking).
    1.	Collision detection is done by calculating the distances between the objects and comparing the sum of their model’s bounding radius. When their distance is less than the sum a collision is triggered.
1.	Utilized new model for Duo called Mountain Planet since the old model made it difficult to place the missile silo on top of it.
1.	Warbird Movement
    1.	The warbird is able to move in six degrees of freedom. 
        1.	Move forward and backward
        1.	Turn left or right.
        1.	Rotate left or right.
        1.	Rotate up or down.
1.	Speed
    1.	The warbird has three levels of speed.
1.	Time Quantum
    1.	Added four levels of time quantum which alters the rate of updates.
1.	Camera
    1.	Added chase camera to warbird.
1.	Gravity
    1.	There is a gravity affect from Ruber that applies to the space ship only.
    1.	Is set off initially when the simulation starts.
1.	Warping
    1.	The ship is able to warp to Unum and Duo and back to its original starting position. The ship warps to the planet’s orbit path and then rotates 180 degrees to be looking at the planet.
    1.	The player has the ability to pass or resign from the war college. 
        1.	If the ship is hit or runs out of missiles then the player loses.
        1.	If the ship destroys both missile silos the user wins.
        1.	Title screen changes based on this status.

* Extra things we added:
    *	Missile Silo Missiles
        *	Can be seen at the bottom base of the missile silos when it hasn’t been fired at the warbird yet.
    *	Warbird Missile 
        *	Appears on the ship when it’s ready to fire.
    *	Tracking
        *	Extra Credit: Utilizes Quaternions for smoother rotations, eliminating nervousness from missiles.
    *	Restart Button
        *	Added a restart button ‘r’ to reset the ship to its original starting position and resets the missile counts for the warbird and missile silos.
    *	Collision
        *	Missiles can collide with each other.
    *	Camera
        *	When the warbird is destroyed the camera sits at that location. When the game is restarted the user needs to cycle to the warbird again.

## Phase 3
* Star Field
    * The star field is implemented as 6 planes that form a cube around the playable area. Vertex and fragment shaders have been edited to render the texture properly. We have a boolean "IsTexture" that gets toggled on/off when rendering the texture specifically. The texture is a 640x425 .raw file edited via irfanview. The placement / rotation axes of each plane (or square) are stored in the following arrays.

![Code Snippet](/images/skyboxCodeSnippet.png)

* Lights
    * Point light from Ruber
        * Ruber is supposed to be set up as a point light that is diffuse when facing away from the light. Meaning that when you look at a planet and Ruber’s light is hitting it you will see the planet light up. In the pointLightPosition method in the simpleFragment.glsl file we attempted to calculate the diffuse by taking the dot product of the point’s normal surface by the direction of the light ( basically where the camera was in place in our space box ).   Following the example form the shaders, lights slides and the example given in chapter 7 of the OpenGL programming textbook we attempted to add attenuation to the point light off Ruber to make it seem more like a real system. 


### Additions to Phase 2:
* When the user restarts the game the camera is reset back to the warbird camera.

 * Things we wanted to do but couldn’t get around to it due to Time constraints:
    The extra credit spot light for the ship. We found an example of the spot light from the Red Book and implemented it in the Fragment Shader but we didn’t get around to finish implementing it in the project due to time constraints. 
    * We wanted to have a graphic or texture displayed on the screen when the player wins or loses.
    * Lastly we wanted to have a way to display credits displaying the team’s names when the player wins the game.

### Extra Work we Added:
**Warning: Make sure your sound is on low initially since it varies from system to system.**

* Sounds and music were added using irrklang a cross platform sound library using C++, C#, and all .NET languages. We Simply had to download and copy the files to the project, link the library and create an object from that library and we were able to utilize its functionality.

* *Sounds*:
    * Added various sounds from Star Trek: The Original Series, Star Trek: The Next Generation television series, and Star Trek: Armada video game.
        * When the user restarts the game all the sounds reset, the randomly selected music track replays and a uncloaking sound plays with a new warbird appears since a new student is trying out for the war college.
        * When the user’s ship collides with an object it plays an explosion sound from Star Trek TOS.
        * When the user warps it plays a warping sound from Star Trek: The Next Generation.
        * When the user fires a missile it plays a photon torpedo sound from Star Trek TNG.
        * We were planning on adding a sound when a missile site missile  is locked onto the warbird but didn’t get around it. The sound is still in our project folder/media its called: MRom300.wav
        * We were also planning on using the Romulan cloaking sound when the warbird is hit but it didn’t make sense for a Star Trek fan!
        * We were planning on having a special missile firing sound for the missile site but didn’t get around to it.
        * Finally we were planning on adding 3D sound sources where the sound volume is based on its location or distance from the camera.

* *Voices*
    * Various voices were added from Star Trek Armada 1.
        * When the program is started there are three voice lines that are randomly selected to play. You can keep pressing the restart button ‘r’ to hear all the three different voice lines!
        * When already fired the missile there are two different voice lines that may play to give feedback.

* *Music*:
    * When the user restarts the game the music resets.
    * There are two different music tracks that can play.
        * Music is the Romulan Faction themes from Star Trek Armada 1 and Star Trek: Armada 2 Video Games.
    * Added special “surprise” music if you win the game! When you win the game all the other sounds and music are stopped so you can listen to the wonderful victory music.

## Authors

* **Armand Abrahamian** - [ArmandAbrahamian](https://github.com/ArmandAbrahamian)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## Acknowledgments

* Voice lines are from the Romulan Faction in Star Trek Armada 1.

* Various sounds from Star Trek: The Original Series, Star Trek: The Next Generation television series, and Star Trek: Armada video game.

* Music is the Romulan Faction themes from Star Trek Armada 1 and Star Trek: Armada 2 Video Games, and Star Trek: TOS theme.

* Warbird Model Credits:
    * The model we imported and used for our Warbird is modeled after the Warbird design from Star Trek TOS. We changed it to a tri model and colored it based on how it looked in the original series.

Romulan Bird of Prey, Classic Star Trek by jpcotedsbn is licensed under CC Attribution-NonCommercial-ShareAlike

![Warbird Model](/images/warbirdModel.png)

This picture doesn't represent exactly what the model looks like but what its based on from the warbird from an episode of Star Trek TOS.
 
