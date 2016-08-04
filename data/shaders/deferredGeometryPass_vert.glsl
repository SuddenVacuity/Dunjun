#version 120

#include _head_transform.glsl

attribute vec3 a_position; // vertex position
attribute vec2 a_texCoord; // vertex texture coordinates
attribute vec3 a_color; // vertex color
attribute vec3 a_normal; // surface normals
attribute vec3 a_tangent; // surface tangent

varying vec3 v_position_ws;

varying vec3 v_position;
varying vec2 v_texCoord;
varying vec3 v_color;
varying mat3 v_tbNormalInv; // {tangent, bitangent, normal}

uniform mat4 u_camera; // view function
uniform Transform u_transform; // transform function

void main()
{
	v_position = a_position;
	v_texCoord = a_texCoord.st;
	v_color = a_color; // moves info from a_color to v_color

	// normal
	vec3 n = normalize(quaternionRotate(u_transform.orientation, a_normal));
	// tangent
	vec3 t = normalize(quaternionRotate(u_transform.orientation, a_tangent));
	t = normalize(t - dot(t, n) * n); // orthogonalize so matrix can be transposed
	// bi-tangent
	vec3 b = cross(t, n);
	v_tbNormalInv = transpose(mat3(t, b, n));

	vec3 pos = a_position;
	
	pos = u_transform.scale * pos;
	pos = quaternionRotate(u_transform.orientation, pos);
	pos = u_transform.position + pos;
	v_position_ws = pos;

	gl_Position = u_camera * vec4(pos, 1.0); // defines position
}
