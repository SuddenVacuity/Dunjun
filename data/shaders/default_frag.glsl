#version 120

uniform sampler2D uniTex; // sampler for colors

varying vec3 fragColor; // attribute for color from vert.glsl
varying vec2 fragTexCoord; // attribute for texture coord from vert.glsl

void main()
{
	vec3 texColor = texture2D(uniTex, fragTexCoord).rgb; // get the color from texture coordinates to add to fragColor
	gl_FragColor = vec4(texColor * fragColor, 1.0); // color for the shader
}