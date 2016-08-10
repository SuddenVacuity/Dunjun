#version 120

#include _head_light.glsl

uniform sampler2D u_specular;
uniform sampler2D u_normal;
uniform sampler2D u_depth;

uniform PointLight u_light;

uniform mat4 u_cameraInverse;

varying vec2 v_texCoord;

vec4 calculatePointLight(vec3 surfaceToLight, float distanceToLight, vec3 normal)
{
	float diffuseCoefficient = max(0.0f, dot(normal, surfaceToLight));

	float attenuation = (u_light.attenuation.constant + 
						 u_light.attenuation.linear * distanceToLight + 
						 u_light.attenuation.quadratic * distanceToLight * distanceToLight);
	attenuation = 1.0f / attenuation;

	//attenuation *= square(1 - clamp(square(square(distanceToLight / u_light.range)), 0.0f, 1.0f));
	attenuation *= clamp(square(1.0 - (square(distanceToLight / u_light.range))), 0, 1);

	vec4 diffuse = vec4(0.0f);

	diffuse.rgb = diffuseCoefficient * u_light.base.intensities.rgb * attenuation;
	diffuse.a = 1.0f;

	return diffuse;
}

void main()
{
	vec3 specularColor = texture2D(u_specular, v_texCoord).rgb;
	vec3 normalEncoded = texture2D(u_normal, v_texCoord).xyz;
	float depth = texture2D(u_depth, v_texCoord).r;

	vec3 position = calculatePositionFromDepth(v_texCoord, gl_FragCoord.w, depth, u_cameraInverse);
	vec3 normal = normalize(2.0f * normalEncoded - vec3(1.0f));

	vec3 surfaceToLight = normalize(u_light.position - position);
	float distanceToLight = length(u_light.position - position);

	vec4 lightColor = calculatePointLight(surfaceToLight, distanceToLight, normal);

	//gl_FragColor = vec4(lightColor.rgb, 1.0f);
	gl_FragColor = lightColor;
}
