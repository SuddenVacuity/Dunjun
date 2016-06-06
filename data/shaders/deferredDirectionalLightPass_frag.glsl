#version 120

struct BaseLight
{
	vec3 intensities; // color * brightness
};

struct DirectionalLight
{
	BaseLight base;
	vec3 direction;
};

uniform sampler2D u_specular;
uniform sampler2D u_normal;

uniform DirectionalLight u_light;

varying vec2 v_texCoord;

float square(float x) { return x; }

vec4 calculateDirectionalLight(DirectionalLight light, vec3 normal)
{
	float diffuseCoefficient = max(0.0f, dot(normal, -light.direction));

	vec4 diffuse = vec4(0.0f);

	diffuse.rgb = diffuseCoefficient * light.base.intensities.rgb;
	diffuse.a = 1.0f;

	return diffuse;
}

void main()
{
	vec3 specularColor = texture2D(u_specular, v_texCoord).rgb;
	vec3 normalEncoded = texture2D(u_normal, v_texCoord).xyz;

	vec3 normal = normalize(2.0f * normalEncoded - vec3(1.0f));

	vec4 lightColor = calculateDirectionalLight(u_light, normal);

	gl_FragColor = vec4(lightColor.rgb, 1.0f);
}
