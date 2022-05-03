attribute vec4 a_position;
attribute vec4 a_normal;
attribute vec4 a_tangent;
attribute vec4 a_biTangent;
attribute vec2 a_texcoord;
uniform mat4 u_mvp;
uniform mat4 u_mv;
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;
varying vec2 v_texcoord;
varying vec3 v_viewPos;
varying vec3 v_lightPos;
varying mat3 v_TBN;
varying vec3 v_surface;
void main()
{
	gl_Position =  u_mvp * a_position;
	v_texcoord = a_texcoord;
	vec3 T = normalize(vec3(u_mv * vec4(a_tangent.xyz, 0.0)));
	vec3 B = normalize(vec3(u_mv * vec4(a_biTangent.xyz, 0.0)));
	vec3 N = normalize(vec3(u_mv * vec4(a_normal.xyz, 0.0)));
	v_TBN = mat3(T, B, N);
	v_surface = vec3(u_mv * a_position);
	v_lightPos =  u_lightPos;
	v_viewPos = u_viewPos;                                           
}