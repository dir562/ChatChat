#include "stdafx.h"
#include "VAO_OBJ.h"
#include "Renderer.h"


void OBJ::update_uniform_vars(GLuint shader)const
{
	auto& renderer = Renderer::instance();
	auto camera = renderer.get_main_camera();
	glm::mat4 vp = renderer.vp_mat();
	glm::mat4 m = model_mat();
	glUniformMatrix4fv(glGetUniformLocation(shader, "u_vp_mat"), 1, GL_FALSE, glm::value_ptr(vp));
	glUniformMatrix4fv(glGetUniformLocation(shader, "u_m_mat"), 1, GL_FALSE, glm::value_ptr(m));

}

// [-1, 1]
glm::vec3 OBJ::get_project_pos(glm::vec3 origin)
{
	auto m = model_mat();
	auto v = Renderer::instance().get_main_camera()->view_mat();
	auto p = screen.proj_mat();
	auto res = p * v * m * glm::vec4{ origin ,1.f };
	return glm::vec3{ res } / res.w;
}


/// ////////////////////////////////////////////

void Billboard::update_uniform_vars(GLuint shader) const
{
	auto& renderer = Renderer::instance();
	auto camera = renderer.get_main_camera();
	glm::mat4 vp = renderer.vp_mat();
	glm::mat4 p = glm::translate(get_position());
	glm::mat4 rs = glm::toMat4(get_rotation()) * glm::scale(get_scale());

	glUniformMatrix4fv(glGetUniformLocation(shader, "u_vp_mat"), 1, GL_FALSE, glm::value_ptr(vp));
	glUniformMatrix4fv(glGetUniformLocation(shader, "u_p_mat"), 1, GL_FALSE, glm::value_ptr(p));
	glUniformMatrix4fv(glGetUniformLocation(shader, "u_rs_mat"), 1, GL_FALSE, glm::value_ptr(rs));


}
