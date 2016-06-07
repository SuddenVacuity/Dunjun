#version 120

#include <_head_material.glsl>

#include <_head_transform.glsl>

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