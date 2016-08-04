#ifndef HEAD_COMMON_GLSL
#define HEAD_COMMON_GLSL


float square(float x) { return x * x; }

vec3 calculatePositionFromDepth(vec2 st, float w, float depth, mat4 inverseMatrix)
{
	float x_hs = 2.0f * st.x - 1.0f;
	float y_hs = 2.0f * st.y - 1.0f;
	float z_hs = 2.0f * depth - 1.0f;

	vec4 position_hs = vec4(x_hs, y_hs, z_hs, 1.0f) / w;

	vec4 position_ws = inverseMatrix * position_hs;

	return position_ws.xyz / position_ws.w;
}

#endif
