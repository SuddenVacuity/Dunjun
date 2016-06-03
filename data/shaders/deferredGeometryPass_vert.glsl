#version 120

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

attribute vec3 a_position; // vertex position
attribute vec2 a_texCoord; // vertex texture coordinates
attribute vec3 a_color; // vertex color
attribute vec3 a_normal; // surface normals

varying vec3 v_position_ws;

varying vec3 v_position;
varying vec2 v_texCoord;
varying vec3 v_color;
varying vec3 v_normal;

uniform mat4 u_camera; // view function
uniform Transform u_transform; // transform function

void main()
{
	v_position = a_position;
	v_texCoord = a_texCoord.st;
	v_color = a_color; // moves info from a_color to v_color
	v_normal = a_normal;

	vec3 pos = a_position;
	
	pos = u_transform.scale * pos;
	pos = quaternionRotate(u_transform.orientation, pos);
	pos = u_transform.position + pos;
	v_position_ws = pos;

	gl_Position = u_camera * vec4(pos, 1.0); // defines position
}