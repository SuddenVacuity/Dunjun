#version 120

uniform sampler2D uniTex; // sampler for colors

varying vec3 fragColor; // attribute for color from vert.glsl
varying vec2 fragTexCoord; // attribute for texture coord from vert.glsl

void main()
{
	vec3 texColor = texture2D(uniTex, fragTexCoord).rgb; // get the color from texture coordinates to add to fragColor
	vec3 gamma = vec3(1.0/2.2);

	vec3 color = texColor * fragColor;

	gl_FragColor = vec4(pow(color, gamma), 1.0);
}