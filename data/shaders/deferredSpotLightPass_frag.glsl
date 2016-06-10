#version 120

#include <_head_light.glsl>

uniform sampler2D u_diffuse;
uniform sampler2D u_specular;
uniform sampler2D u_normal;
uniform sampler2D u_depth;

uniform SpotLight u_light;

uniform mat4 u_cameraInverse;

varying vec2 v_texCoord;

vec4 calculateSpotLight(float lightToSurfaceAngle, 
						vec3 surfaceToLight, 
						float distanceToLight, 
						vec3 normal)
{
	float diffuseCoefficient = max(0.0f, dot(normal, surfaceToLight));

	float attenuation = (u_light.pointLight.attenuation.constant + 
						 u_light.pointLight.attenuation.linear * distanceToLight + 
						 u_light.pointLight.attenuation.quadratic * distanceToLight * distanceToLight);
	attenuation = 1.0f / attenuation;

	// light fall off from distance
	attenuation *= square(1 - clamp(square(square(
						  distanceToLight / u_light.pointLight.range
						  )), 0.0f, 1.0f));

	// this part removes the hard edges from the spot light
	attenuation *= square(1 - clamp(square(square(square(
						  lightToSurfaceAngle / u_light.coneAngle
						  ))), 0.0f, 1.0f));

	vec4 diffuse = vec4(0.0f);

	diffuse.rgb = diffuseCoefficient * u_light.pointLight.base.intensities.rgb * attenuation;
	diffuse.a = 1.0f;

	return diffuse;
}

void main()
{
	vec3 specularColor = texture2D(u_specular, v_texCoord).rgb;
	vec3 normalEncoded = texture2D(u_normal, v_texCoord).xyz;
	float depth = texture2D(u_depth, v_texCoord).r;

	vec3 position = calculatePositionFromDepth(v_texCoord, depth, u_cameraInverse);
	vec3 normal = normalize(2.0f * normalEncoded - vec3(1.0f));

	vec3 surfaceToLight = normalize(u_light.pointLight.position - position);
	float distanceToLight = length(u_light.pointLight.position - position);
	float lightToSurfaceAngle = acos(dot(-surfaceToLight, normalize(u_light.direction)));

	vec4 lightColor = calculateSpotLight(lightToSurfaceAngle, surfaceToLight, distanceToLight, normal);

	gl_FragColor = vec4(lightColor.rgb, 1.0f);
}

