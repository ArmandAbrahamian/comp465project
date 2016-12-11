/* 
SimpleFragment.glsl

Fragment shader with color input and output.

12/10/2016
*/

# version 330 core

// Variables for point light calculations
uniform vec3 PointLightPosition;
uniform vec3 EyeDirection;
uniform bool PointLightOn;

// Variables for directional light calculations
uniform vec3 LightDirection;
uniform bool DirectionalLightOn;

// Variables used in lighting calculations
uniform vec3 LightColor;
uniform float ConstantAttenuation;
uniform float LinearAttenuation;
uniform float QuadraticAttenuation;
uniform float Shininess;
uniform float Strength;
uniform bool AmbientOn;
uniform bool SunObject;

// Texture information:
uniform sampler2D Texture;
uniform bool IsTexture;

// Input information from the vertex shader:
in vec3 Position;
in vec3 Normal;
in vec4 color;
in vec2 TextCoord;

// Output information:
out vec4 FragColor;

// Calculates the pointlight effect on fragments
vec3 pointLight(vec3 LightPosition) {

	float ambient;

	// Check for whether to apply an ambient effect
	if (AmbientOn)
		ambient = 0.2f;
	else
		ambient = 0.0f;

	// Determine the direction of the light 
	vec3 lightDirection = LightPosition - vec3(Position);

	// Determine the distance of the light direction vector
	float lightDistance = length(lightDirection);

	// Normalize the light direction vector
	lightDirection = lightDirection / lightDistance;

	// Calculate the attenuation
	float attenuation = 1.0 / (ConstantAttenuation +
								LinearAttenuation * lightDistance +
								QuadraticAttenuation * lightDistance * lightDistance);

	// Calculate the half vector and normalize
	vec3 halfVector = normalize(lightDirection + EyeDirection);
	float diffuse;
	float specular;

	// If the current fragments belong to the sun object calculation of these fragments
	// are different in order to light up the sun. Reverse the normals and calculate the
	// diffuse and specular light effects, otherwise don't reverse the normals and calculate
	// the specular and diffuse light effects.
	if (SunObject) { 
		diffuse = max(0.0, dot(-Normal, lightDirection));
	    specular = max(0.0, dot(-Normal, halfVector));
		}
	else {
		diffuse = max(0.0, dot(Normal, lightDirection));
		specular = max(0.0, dot(Normal, halfVector));
	}

	// If no diffuse effect, no specular. Otherwise increase specular effect.
	if (diffuse == 0.0)
		specular = 0.0;
	else
		specular = pow(specular, Shininess) * Strength;

	// Calculate the scattered light and reflected light,
	// then apply their effect to the color value of the fragment.
	// Return the color value or white, whichever is minimal
	vec3 scatteredLight = ambient + LightColor * diffuse;
	vec3 reflectedLight = LightColor * specular * Strength;
	vec3 rgb = min(Color.rgb * scatteredLight + reflectedLight, vec3(1.0));

	return rgb;
}


// Method to calculate the directional light effect on the fragments
// Returns a vec3 structure that represents a color value
vec3 directionalLight() {

	float ambient;

	// Check for whether to apply an ambient effect
	if (AmbientOn)
		ambient = 0.2f;
	else
		ambient = 0.0f;

	// Calculate the half vector and normalize
	vec3 halfVector = normalize(LightDirection);

	// Calculate the diffuse and specular light effects
	float diffuse = max(dot(Normal, LightDirection), 0.0);
	float specular = max(dot(Normal, halfVector), 0.0);

	// If no diffuse effect, no specular. Otherwise increase specular effect.
	if (diffuse == 0)
		specular = 0.0;
	else
		specular = pow(specular, Shininess);

	// Calculate the scattered light and reflected light,
	// then apply their effect to the color value of the fragment.
	// Return the color value or white, whichever is minimal
	vec3 scatteredLight = ambient + LightColor * diffuse;
	vec3 reflectedLight = LightColor * specular * Strength;
	vec3 rgb = min(Color.rgb * scatteredLight + reflectedLight, vec3(1.0));

	return rgb;

}

void main() {
	
	// Check if the fragment is going to have a texture or not
	if (IsTexture) {

	// If so, apply the texture
	FragColor = texture(Texture, TextCoord); 

	}
	else {
		
		// Get the color of the fragment
		vec3 tempColor = vec3(Color) * 0.1f;

		// if the directional light is on, apply directional light effect to color
		if (DirectionalLightOn)
			tempColor += directionalLight();

		// if the point light is on, apply point light effect to color
		if (PointLightOn) 
			tempColor += pointLight(PointLightPosition);

		// Output the fragment's color
		FragColor = vec4(tempColor, 1.0);
	}
}