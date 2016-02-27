#version 120

uniform sampler2D u_tex; // sampler for colors

varying vec3 v_color; // attribute for color from vert.glsl
varying vec2 v_texCoord; // attribute for texture coord from vert.glsl

void main()
{
	vec3 texColor = texture2D(u_tex, v_texCoord).rgb; // get the color from texture coordinates to add to v_color
	vec3 gamma = vec3(1.0/2.2);

	vec3 color = texColor * v_color;

	gl_FragColor = vec4(pow(color, gamma), 1.0);
}