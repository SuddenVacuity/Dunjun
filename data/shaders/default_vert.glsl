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
attribute vec3 a_color; // vertex color
attribute vec2 a_texCoord; // vertex texture coordinates

varying vec3 v_color; // vert color to be passed to frag.glsl
varying vec2 v_texCoord; // texture coord to be passed to frag.glsl

uniform mat4 u_camera; // view function
//uniform mat4 u_model; // transform function
uniform Transform u_transform = {vec3(0), vec4(0, 0, 0, 1), vec3(1)}; // transform function

void main()
{
	v_color = a_color; // moves info from a_color to v_color
	v_texCoord = a_texCoord.st;

	// v' = proj * view * transform * v;
	vec3 pos = a_position;
	pos = u_transform.scale * pos;
	pos = quaternionRotate(u_transform.orientation, pos);
	pos = u_transform.position + pos;

	gl_Position = u_camera * vec4(pos, 1.0); // defines position
	//gl_Position = u_camera * u_model * vec4(a_position, 0.0, 1.0); // defines position
}