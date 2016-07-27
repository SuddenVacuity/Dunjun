#version 120

#include _head_material.glsl

#include _head_light.glsl

#include _head_transform.glsl

uniform vec3 u_cameraPosition;
uniform Transform u_transform; // transform function

uniform Material u_material;

uniform PointLight u_light;

varying vec3 v_position_ws; // world space

varying vec3 v_position;
varying vec2 v_texCoord; // attribute for texture coord from vert.glsl
varying vec3 v_color; // attribute for color from vert.glsl
varying vec3 v_normal;

void main()
{
	vec4 texColor = texture2D(u_material.diffuseMap, v_texCoord).rgba; // get the color from texture coordinates to add to v_color

	if(texColor.a < 0.5) // check alpha value
		discard;

	vec3 surfaceColor = u_material.diffuseColor.rgb * texColor.rgb * v_color;

// start calculate light
	vec3 normal = normalize(quaternionRotate(u_transform.orientation, v_normal));

	vec3 distance = u_light.position - v_position_ws;
	vec3 surfaceToLight = normalize(distance);

	float distanceToLight = length(distance); 

	// diffuse light
	float diffuseCoefficient = max(0.0f, dot(normal, surfaceToLight));

	vec3 diffuse = diffuseCoefficient * u_light.base.intensities.rgb;

	// ambient light
	vec3 ambient = vec3(0.001f, 0.001f, 0.001f);

	// specular light
	float specularCoefficient = 0.0f;

	vec3 incidence = -surfaceToLight;
	vec3 reflection = reflect(incidence, normal);
	vec3 surfaceToCamera = normalize(u_cameraPosition - v_position_ws);

	float cosTheta = max(0.0f, dot(surfaceToCamera, reflection));

	if(diffuseCoefficient > 0.0f) // if() is more efficient than high pow()
	{
		specularCoefficient = pow(cosTheta, u_material.specularExponent); // increase 2nd value to sharpen reflection
	}

	vec3 specular = specularCoefficient * u_light.base.intensities.rgb * u_material.specularColor.rgb; // last value is specular color

// end calculate light
	float attenuation = (u_light.attenuation.constant + 
						 u_light.attenuation.linear * distanceToLight + 
						 u_light.attenuation.quadratic * distanceToLight * distanceToLight);
	attenuation = 1.0f / attenuation;

	// TODO: break apart so it's easier to read
	attenuation *= pow(1 - clamp(pow(distanceToLight / u_light.range, 4.0f), 0.0f, 1.0f), 2.0f);

	vec3 finalColor = (ambient.rgb + (diffuse.rgb + specular.rgb) * attenuation) * surfaceColor.rgb;

	vec3 gamma = vec3(1.0/2.2);
	gl_FragColor = vec4(pow(finalColor, gamma), 1.0);
}
