# version 330 core
uniform sampler2D Texture;
uniform bool IsTexture;
in vec3 Position;
in vec3 Normal;
in vec4 color;
in vec2 TextCoord;
//ambient
uniform bool Ambient;
//point
uniform vec3 PointLightRuber;
uniform vec3 PointLightPosition;
uniform bool Point;
//headlamp
uniform vec3 HeadLampPosition;
uniform bool Headlamp;
//light variables
uniform bool Ruber;
uniform vec3 LightColor;
uniform float ConstantAttenuation;
uniform float LinearAttenuation;
uniform float QuadraticAttenuation;
uniform float Shininess;
uniform float Strength;
out vec4 FragColor;
//calc the point light effect
vec3 pointLight(vec3 LightPosition){
    
    //scale directional ambient
    float ambient = 2.0f; 
    if(Ambient == false)
        //no ambient
        ambient = 0.0f;
    
    //find direction and distance of light
    vec3 lightDirection = LightPosition - vec3(Position);
    float lightDistance = length(lightDirection);
    //normalize
    lightDirection = lightDirection / lightDistance;
    //model how much light is available 
    float attenuation = 1.0 / (ConstantAttenuation + LinearAttenuation
        * lightDistance + QuadraticAttenuation * lightDistance * lightDistance);
    vec3 halfVector = normalize(lightDirection + PointLightPosition);
    //direction of highlights coming from ruber, if coming towards
    //opposite
    float diffuse = max(0.0, dot(Normal, lightDirection));
    float specular = max(0.0, dot(Normal, halfVector));
    if(!Ruber){
        float diffuse = max(0.0, dot(Normal, lightDirection));
        float specular = max(0.0, dot(Normal, halfVector));
    }
    if (diffuse == 0.0)
        specular = 0.0;
    else
        specular = pow(specular, Shininess) * Strength;
    }
    vec3 scatteredLight = ambient + LightColor * diffuse * attenuation;
    vec3 reflectedLight = LightColor * specular * Strength *attenuation;
    vec3 rgb = min(Color.rgb * scatteredLight + reflectedLight, vec3(1.0));
    return rgb;
}

//headlamp
vec3 headLampLight(){
    
    float ambient = 2.0f; 
    if(Ambient == false)
        //no ambient
        ambient = 0.0f;
    
    vec3 halfVector = normalize(lightDirection); 
    float diffuse = max(dot(Normal, LightDirection),0.0);
    float specular = max(dot(Normal, halfVector),0.0);
    
    if (diffuse == 0.0)
        specular = 0.0;
    else
        specular = pow(specular, Shininess); // sharpen the highlight
    vec3 scatteredLight = Ambient + LightColor * diffuse;
    vec3 reflectedLight = LightColor * specular * Strength;
    vec3 rgb = min(Color.rgb * scatteredLight + reflectedLight, vec3(1.0));
    FragColor = vec4(rgb, Color.a);
}
void main() {
  FragColor = color;
  if(IsTexture) {
    FragColor = texture(Texture, TextCoord);
  }else{
    vec3 tempColor = vec3(Color) * 0.1f;
    //if headlamp on
    if(HeadLamp)
        tempColor += headLampLight();
    //if point light is on
    if(Point)
        tempColor += pointLight(PointLightPosition);
        FragColor = vec4(tempColor, 1.0);
  }
  
}