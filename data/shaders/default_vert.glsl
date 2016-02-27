#version 120

attribute vec2 a_position; // vertex position
attribute vec3 a_color; // vertex color
attribute vec2 a_texCoord; // vertex texture coordinates

uniform mat4 u_camera; // view function
uniform mat4 u_model; // matrix function

varying vec3 v_color; // vert color to be passed to frag.glsl
varying vec2 v_texCoord; // texture coord to be passed to frag.glsl

void main()
{
	v_color = a_color; // moves info from a_color to v_color
	// v_texCoord = vec2(a_texCoord.s, 1.0 - a_texCoord.t); // subtract 1 from t to make texture pixels align with shader pixels
	v_texCoord = a_texCoord.st;

	// v' = proj * view * model * v;
	gl_Position = u_camera * u_model * vec4(a_position, 0.0, 1.0); // defines position
}