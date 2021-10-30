#pragma once

#include "Camera.h"


/////////////////////////////////////////////////////////////////

struct OBJ_DATA
{
	GLuint vao;
};
using ObjDataPtr = shared_ptr<OBJ_DATA>;
/////////////////////////////////////////////////////////////////

const glm::vec3 HEADING_DEFAULT = X_DEFAULT;

/* 그려지는 모든 물체 */
class OBJ
{
public:
	explicit OBJ(ObjDataPtr obj_data, GLuint shader) : _obj_data{ obj_data }, _shader{ shader }{}

	glm::mat4 model_mat()const { return glm::translate(_translate) * glm::toMat4(_quaternion) * glm::scale(_scale); }

	glm::vec3 get_position()const { return glm::vec3{ _translate }; }
	glm::quat get_rotation()const { return _quaternion; }
	glm::vec3 get_scale()const { return _scale; }

	glm::vec3 get_head_dir()const { return get_rotation() * HEADING_DEFAULT; }

	void rotate(glm::quat q) { _quaternion = q * _quaternion; }
	void move(glm::vec3 dif) { _translate += dif; }
	void scaling(glm::vec3 ratio) { _scale *= ratio; }

public:
	void bind_vao()const { glBindVertexArray(_obj_data->vao); }
	void update_uniform_vars(GLuint shader)const;

public:
	glm::vec3 get_project_pos(glm::vec3 origin = {});


private:
	glm::vec3 _translate;
	glm::quat _quaternion;
	glm::vec3 _scale{ V3_DEFAULT };

	ObjDataPtr _obj_data;
	GLuint _shader;

};
using ObjPtr = shared_ptr<OBJ>;

///////////////////////////////////////////

class Billboard : public OBJ
{
public:
	explicit Billboard(ObjDataPtr obj_data, GLuint shader) : OBJ{ obj_data ,shader } {}
public:
	void update_uniform_vars(GLuint shader)const;
};

///////////////////////////////////////////
