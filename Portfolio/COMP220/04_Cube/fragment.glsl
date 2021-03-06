#version 330 core

in vec3 colour;
in vec3 normal;
in vec2 uv;
uniform sampler2D textureSampler;

uniform vec3 lightDirection;
uniform vec3 eyeDirection;
uniform float specularIntensity;
uniform vec3 LightColor;
uniform vec3 ObjectColor;
uniform float LightPower;
uniform vec3 lightPos;


out vec4 fragmentColour;

void main()
{
	

	// Calculate Ambient
	vec3 MaterialAmbientColor = ObjectColor * colour;
    vec3 n = normalize( normal );
    vec3 l = normalize( lightDirection );

    float cosTheta = dot(n,l);

	// Calculate Diffuse
	vec3 E = normalize(eyeDirection);
	vec3 R = (-lightDirection, normal);
	vec3 lightDirectionNorm = normalize(lightDirection);
	vec3 Diffuse = (colour * LightColor * cosTheta);

	// Calculate Attenuation
	float distance = length(lightPos - n);

	// Calculate Specular
	float cosAlpha = clamp( dot( E,R ), 0,1 );
	float diffuseIntensity = dot(normal, lightDirectionNorm);
	float lightIntensity = diffuseIntensity + pow(cosAlpha, specularIntensity);
	vec3 Specular = (lightIntensity * ObjectColor * LightColor * cosAlpha);
	
	fragmentColour = vec4(MaterialAmbientColor + Diffuse + Specular, 1.0) * texture(textureSampler, uv);
}
