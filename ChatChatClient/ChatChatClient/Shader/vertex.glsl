#version 450


uniform mat4 u_vp_mat;
uniform mat4 u_m_mat;


attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texcoord;

varying vec3 v_normal;
varying vec2 v_texcoord;

void main()
{	
	gl_Position = u_vp_mat * u_m_mat * vec4(a_position, 1.0f);

	v_normal = a_normal;
	v_texcoord = a_texcoord;
}
