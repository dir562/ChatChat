#pragma once


#include "VAO_OBJ.h"
#include "DynamicObj.h"

//////////////////////////////////////////////////////

struct SCREEN
{
	GLfloat fovy{ 45.0f }; //glm::Radians(fovy)
	GLsizei width{ g_WindowSizeX };	//W/H
	GLsizei height{ g_WindowSizeY };	//W/H
	GLfloat n{ 0.1f };
	GLfloat f{ 20000.0f };

	GLfloat aspect()const { return (GLfloat)width / height; }
	glm::mat4 proj_mat()const {
		return glm::perspective(glm::radians(fovy), aspect(), n, f);
	}
};
extern SCREEN screen;

//////////////////////////////////////////////////////

struct RESOURCE
{

};

//////////////////////////////////////////////////////

/* clients drawer */
class Renderer
{
	friend Billboard;
public:
	static Renderer& instance()
	{
		static Renderer _instance;
		return _instance;
	}

public:
	void draw();
	void ready_draw();

	void reshape(int w, int h);

	glm::mat4 vp_mat()const { return _vp_mat; }
	glm::mat4 proj_mat()const { return _screen.proj_mat(); }


	CameraPtr get_main_camera()const { return _main_camera; }
	Player0Ptr get_player()const { return _player; }

private:
	void init();
	void init_shader();
	void init_resources();

	bool read_file(string_view filename, std::string& target);
	void add_shader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint compile_shader(string_view filenameVS, string_view filenameFS);
	GLuint create_vao(GLuint shader, const VERTEX* vertices, GLsizei vertices_num);
	void load_texture();
	void load_model();

private:
	Renderer();
	~Renderer();

	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;


private:
	unique_ptr<struct RESOURCE> _resource;
	CameraPtr _main_camera;
	SCREEN& _screen = screen;
	glm::mat4 _vp_mat;

	//
	GLuint _default_shader;
	vector<ObjPtr> _cars;
	Player0Ptr _player;
	GLuint _player_tex;

	//
	GLuint _terrain_shader;
	GLuint _terrain_tex;
	ObjPtr _terrain;

	//
protected:
	GLuint _billboard_shader;
	GLuint _billboard_tex0;
	GLuint _billboard_tex1;
	GLuint _billboard_tex2;
	GLuint _billboard_tex3;
	vector<Billboard> _grasses;

};




