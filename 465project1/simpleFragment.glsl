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
uniform vec3 PointLightRuber; //PointLightPosition 
uniform vec3 PointLightPosition; //EyeDirection
uniform bool PointLightOn;

//headLamp Variables
uniform vec3 HeadLampPosition; // LightDirection
uniform bool HeadLampOn; // DirectionalLightOn

//light Variables
uniform vec3 LightColor; 
uniform vec3 Ambient;
uniform float ConstantAttenuation;
uniform float LinearAttenuation;
uniform float QuadraticAttenuation;
uniform float Shininess; 
uniform float Strength;
uniform bool AmbientLightOn; //AmbientOn
uniform bool Ruber; // Sunobject

//texture 
uniform sampler2D Texture;
uniform bool IsTexture;

//input info from vertex shader output
in vec3 Position;
in vec3 Normal;
in vec4 Color;
in vec2 TextCoord;

// output from fragment shader prog
out vec4 FragColor;

//pointLight method in GLSL
vec3 pointLight(vec3 lightPos){

	float ambient = 0.0f;

	//Check for whether to apply an ambient effect
	if (AmbientLightOn)
		ambient = 0.2f;

	// find the direction and distance of the light, which changes fragment to fragment for a local light
	vec3 lightDirection = lightPos - vec3(Position);
	float lightDistance = length(lightDirection);

	// normalize the light direction vector, so that a dot products give cosines
	lightDirection = lightDirection/lightDistance; 

	// Model how much light is available for this fragment:
	float attenuation = 1.0 / (ConstantAttenuation +
								LinearAttenuation * lightDistance +
								QuadraticAttenuation * lightDistance
								 * lightDistance);

	// The direction of maximum highlight also changes per fragment.
	vec3 halfVector = normalize(lightDirection + PointLightPosition);

	float diffuse = max(0.0, dot(Normal, lightDirection));
	float specular = max(0.0, dot(Normal, halfVector));

	if(diffuse == 0.0)
		specular = 0.0;
	else
		specular = pow(specular, Shininess) * Strength;

	vec3 scatteredLight = Ambient + LightColor * diffuse * attenuation;
	vec3 reflectedLight = LightColor * specular * attenuation ;
	vec3 rgb = min(Color.rgb * scatteredLight + reflectedLight, vec3(1.0));

	return rgb;
}

//HeadLamp Method
vec3 HeadLamp(){
	float ambient = 0.2f;

	vec3 halfVector = normalize(HeadLampPosition);

	float diffuse = max(dot(Normal, HeadLampPosition), 0.0);
	float specular = max(dot(Normal, halfVector), 0.0);

	if (diffuse == 0)
		specular = 0.0;
	else
		specular = pow(specular, Shininess);

	vec3 headLamp = (ambient * Strength + diffuse * Strength, vec3(1.0));

	return headLamp;
}

void main() {
 FragColor = Color;

  if(IsTexture) {
	FragColor = texture(Texture, TextCoord);
  }else {
		
		// Get the color of the fragment
		vec3 tempColor = vec3(Color) * 0.5f; // initial value

		// if the directional light is on, apply directional light effect to color
		if (HeadLampOn)
			tempColor += HeadLamp();

		// if the point light is on, apply point light effect to color
		if (PointLightOn) 
			tempColor += pointLight(PointLightPosition);
		
		//Output the fragment's color
		FragColor = vec4(tempColor, 1.0);
		
	}
  
}