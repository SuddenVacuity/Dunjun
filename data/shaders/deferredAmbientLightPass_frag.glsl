#version 120

#include _head_light.glsl

uniform BaseLight u_light;

void main()
{
	gl_FragColor = vec4(u_light.intensities.rgb, 1.0f);
}
