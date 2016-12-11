/* 
SimpleFragment.glsl

Fragment shader with color input and output.

Mike Barnes
8/16/2014

updated to our Warbird Simulation by
Armand Abrahamian
Ben Villalobos
Bryant Barron
*/

# version 330 core

//pointLight Variables
uniform vec3 PointLightPosition;
uniform vec3 EyeDirection;
uniform bool PointLightOn;

//headLamp Variables
uniform vec3 LightDirection; // Direction toward the light.
uniform bool DirectionalLightOn; 

// Spot Light Variables
uniform bool SpotLightOn;
uniform vec3 ConeDirection; // adding spotlight attributes
uniform float SpotCosCutoff; // how wide the spot is, as a cosine
uniform float SpotExponent; // control light fall-off in the spot

//light Variables
uniform vec3 LightColor; 
uniform float ConstantAttenuation;
uniform float LinearAttenuation;
uniform float QuadraticAttenuation;
uniform float Shininess; // exponent for sharping highlights
uniform float Strength; // extra factor to adjust brightness of shininess
uniform bool AmbientOn; 
uniform bool SunObject;

//texture 
uniform sampler2D Texture;
uniform bool IsTexture;

//input info from vertex shader output
in vec4 Color;
in vec3 Position;
in vec3 Normal; // surface normal, interpolated between vertices
in vec2 TextCoord;

// output from fragment shader prog
out vec4 FragColor;

//pointLight method in GLSL
vec3 pointLight(vec3 LightPosition){
	
	float ambient;

	// Check for whether to apply an ambient effect
	if (AmbientOn)
		ambient = 0.2f;
	else
		ambient = 0.0f;

	// find the direction and distance of the light, which changes fragment to fragment for a local light
	vec3 lightDirection = LightPosition - vec3(Position);
	float lightDistance = length(lightDirection);

	// normalize the light direction vector, so that a dot products give cosines
	lightDirection = lightDirection / lightDistance; 

	// Model how much light is available for this fragment:
	float attenuation = 1.0 / (ConstantAttenuation +
								LinearAttenuation * lightDistance +
								QuadraticAttenuation * lightDistance
								 * lightDistance);

	// The direction of maximum highlight also changes per fragment.
	vec3 halfVector = normalize(lightDirection + EyeDirection);

	float diffuse;
	float specular;

	// compute cosine of the directions, using dot products,
	// to see how much light would be reflected

	if (SunObject) 
	{ 
		diffuse = max(0.0, dot(-Normal, lightDirection));
	    specular = max(0.0, dot(-Normal, halfVector));
	}
	else 
	{
		diffuse = max(0.0, dot(Normal, lightDirection));
		specular = max(0.0, dot(Normal, halfVector));
	}

	if(diffuse == 0.0)
		specular = 0.0;
	else
		specular = pow(specular, Shininess) * Strength;

	vec3 scatteredLight = ambient + LightColor * diffuse;
	vec3 reflectedLight = LightColor * specular * Strength;

	// don’t modulate the underlying color with reflected light,
	// only with scattered light

	vec3 rgb = min(Color.rgb * scatteredLight + reflectedLight, vec3(1.0));
	
	return rgb;
}

vec3 directionalLight()
{
	float ambient;

	// Check for whether to apply an ambient effect
	if (AmbientOn)
		ambient = 0.2f;
	else
		ambient = 0.0f;

	vec3 halfVector = normalize(LightDirection);

	// compute cosine of the directions, using dot products,
	// to see how much light would be reflected

	float diffuse = max(dot(Normal, LightDirection), 0.0);
	float specular = max(dot(Normal, halfVector), 0.0);

	// surfaces facing away from the light (negative dot products)
	// won’t be lit by the directional light

	if (diffuse == 0)
		specular = 0.0;
	else
		specular = pow(specular, Shininess); // sharpen the highlight

	// Calculate the scattered light and reflected light,
	// then apply their effect to the color value of the fragment.
	// Return the color value or white, whichever is minimal
	vec3 scatteredLight = ambient + LightColor * diffuse;
	vec3 reflectedLight = LightColor * specular * Strength;
	vec3 rgb = min(Color.rgb * scatteredLight + reflectedLight, vec3(1.0));

	return rgb;
}

/* Spot light from book:
vec3 spotLight()
{
	float ambient;

	// Check for whether to apply an ambient effect
	if (AmbientOn)
		ambient = 0.2f;
	else
		ambient = 0.0f;

	// find the direction and distance of the light, which changes fragment to fragment for a local light
	vec3 lightDirection = LightPosition - vec3(Position);
	float lightDistance = length(lightDirection);

	// normalize the light direction vector, so that a dot products give cosines
	lightDirection = lightDirection / lightDistance; 

	// Model how much light is available for this fragment:
	float attenuation = 1.0 / (ConstantAttenuation +
								LinearAttenuation * lightDistance +
								QuadraticAttenuation * lightDistance
								 * lightDistance);

	// how close are we to being in the spot?
	float spotCos = dot(lightDirection, -ConeDirection);

	// attenuate more, based on spot-relative position
	if (spotCos < SpotCosCutoff)
		attenuation = 0.0;
	else
		attenuation *= pow(spotCos, SpotExponent);

	vec3 halfVector = normalize(lightDirection + EyeDirection);

	// compute cosine of the directions, using dot products,
	// to see how much light would be reflected

	float diffuse = max(0.0, dot(Normal, lightDirection));
	float specular = max(0.0, dot(Normal, halfVector));

	if (diffuse == 0)
		specular = 0.0;
	else
		specular = pow(specular, Shininess) * Strength; // sharpen the highlight

	// Calculate the scattered light and reflected light,
	// then apply their effect to the color value of the fragment.
	// Return the color value or white, whichever is minimal
	vec3 scatteredLight = ambient + LightColor * diffuse * attenuation;
	vec3 reflectedLight = LightColor * specular * attenuation;
	vec3 rgb = min(Color.rgb * scatteredLight + reflectedLight, vec3(1.0));

	return rgb;
}
*/

void main() {

  if(IsTexture) 
  {
	FragColor = texture(Texture, TextCoord);
  }

  else 
  {
		// Get the color of the fragment
		vec3 tempColor = vec3(Color) * 0.1f; // initial value

		// if the directional light is on, apply directional light effect to color
		if (DirectionalLightOn)
			tempColor += directionalLight();
			
		// if the point light is on, apply point light effect to color
		if (PointLightOn) 
			tempColor += pointLight(PointLightPosition);
		
		//Output the fragment's color
		FragColor = vec4(tempColor, 1.0);	
	}
}