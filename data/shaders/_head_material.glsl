#ifndef HEAD_MATERIAL_GLSL
#define HEAD_MATERIAL_GLSL


struct Material
{
	sampler2D diffuseMap;
	vec4 diffuseColor;

	vec4 specularColor;
	float specularExponent;
};


#endif
