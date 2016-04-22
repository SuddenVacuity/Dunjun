#version 120

uniform sampler2D u_tex; // sampler for colors

varying vec2 v_texCoord; // attribute for texture coord from vert.glsl
varying vec3 v_color; // attribute for color from vert.glsl

void main()
{
	vec4 texColor = texture2D(u_tex, v_texCoord).rgba; // get the color from texture coordinates to add to v_color

	if(texColor.a < 0.5) // check alpha value
		discard;

	vec3 gamma = vec3(1.0/2.2);
	vec3 color = texColor.rgb * v_color;

	gl_FragColor = vec4(pow(color, gamma), 1.0);
}