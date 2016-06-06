#version 120

struct BaseLight
{
	vec3 intensities; // color * brightness
};

uniform BaseLight u_light;

void main()
{
	gl_FragColor = vec4(u_light.intensities.rgb, 1.0f);
}
