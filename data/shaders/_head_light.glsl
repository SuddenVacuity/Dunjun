#ifndef HEAD_LIGHT_GLSL
#define HEAD_LIGHT_GLSL

#include _head_common.glsl

struct Attenuation
{
	float constant;
	float linear;
	float quadratic;
};

struct BaseLight
{
	vec3 intensities; // color * brightness
};

struct DirectionalLight
{
	BaseLight base;
	vec3 direction;
};

struct PointLight
{
	BaseLight base;

	vec3 position;
	Attenuation attenuation;

	float range;
};

struct SpotLight
{
	PointLight pointLight;

	float coneAngle;
	vec3 direction;
};

#endif
