#version 120

struct PointLight
{
	vec3 position;
	vec3 intensities; // color * brightness
};

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

uniform Transform u_transform = {vec3(0), vec4(0, 0, 0, 1), vec3(1)}; // transform function

uniform mat4 u_camera; // view function
uniform sampler2D u_tex; // sampler for colors

uniform PointLight u_light = {vec3(0, 0, 0), vec3(0, 0, 0)};

varying vec3 v_position;
varying vec2 v_texCoord; // attribute for texture coord from vert.glsl
varying vec3 v_color; // attribute for color from vert.glsl
varying vec3 v_normal;

void main()
{
	vec4 texColor = texture2D(u_tex, v_texCoord).rgba; // get the color from texture coordinates to add to v_color

	if(texColor.a < 0.5) // check alpha value
		discard;

// start calculate light
	vec3 normal = normalize(quaternionRotate(u_transform.orientation, v_normal));

	// surface position
	vec3 position = v_position;
	position = u_transform.scale * position;
	position = quaternionRotate(u_transform.orientation, position);
	position = u_transform.position + position;

	vec3 surfaceToLight = u_light.position - position;

	float brightness = dot(normal, surfaceToLight) / (length(normal)*length(surfaceToLight));
	brightness = clamp(brightness, 0, 1);

	brightness /= 1 + dot(surfaceToLight, surfaceToLight); // = 1 / r^2
	
// end calculate light

	vec3 gamma = vec3(1.0/2.2);
	vec3 color = texColor.rgb * v_color;

	color = brightness * u_light.intensities * color;

	gl_FragColor = vec4(pow(color, gamma), 1.0);
}