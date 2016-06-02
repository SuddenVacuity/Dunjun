#version 120

struct Material
{
	sampler2D diffuseMap;
	vec4 diffuseColor;

	vec4 specularColor;
	float specularExponent;
};

struct Transform
{
	vec3 position;
	vec4 orientation;
	vec3 scale;
};

vec3 quaternionRotate(vec4 q, vec3 v)
{
	vec3 t = 2.0 * cross(q.xyz, v);
	return (v + q.w * t + cross(q.xyz, t));
}
uniform vec3 u_cameraPosition;
uniform Transform u_transform; // transform function

uniform Material u_material;

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

	vec3 normal = normalize(quaternionRotate(u_transform.orientation, v_normal));

	gl_FragData[0].rgb = surfaceColor.rgb;

	gl_FragData[1].rgb = u_material.specularColor.rgb;
	gl_FragData[1].a = 1.0f / (1.0f + u_material.specularExponent); // 0.001 to never divide by 0

	gl_FragData[2].xyz = 0.5f * (0.001 + normal + vec3(1.0f));
	//gl_FragData[3].xyz = 

}
