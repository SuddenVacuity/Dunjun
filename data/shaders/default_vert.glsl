#version 120

attribute vec2 vertPosition; // vertex position
attribute vec3 vertColor; // vertex color
attribute vec2 vertTexCoord; // vertex texture coordinates

varying vec3 fragColor; // vert color to be passed to frag.glsl
varying vec2 fragTexCoord; // texture coord to be passed to frag.glsl

void main()
{
	fragColor = vertColor; // moves info from vertColor to fragColor
	fragTexCoord = vec2(vertTexCoord.s, 1.0 - vertTexCoord.t); // subtract 1 from t to make texture pixels align with shader pixels
	gl_Position = vec4(vertPosition, 0.0, 1.0); // defines position
}