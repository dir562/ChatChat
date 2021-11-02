#include "stdafx.h"
#include "Renderer.h"
#include "TextRenderer.h"

////////////////////////////

SCREEN screen;

////////////////////////////

void render_chatrecord(glm::vec3 color = {}, void* font = GLUT_BITMAP_HELVETICA_18)
{
	const auto& record = ChatManager::instance().get_record();
	TextOpt opt;
	opt._align = TEXT_ALIGNMENT::left;
	opt._space = { 0.0f, 0.0f };
	Pointf p = { -0.985f, -0.8f };
	auto rv_it_record = views::reverse(record).begin();
	for (int i = 0; i < min(10ull, record.size()); i++, rv_it_record++)
	{
		const auto& record = rv_it_record;
		glColor3f(color.r, color.g, color.b);
		auto chat_header = "player" + to_string(record->id) + ":: ";
		TextRenderer::rend_text(chat_header, p, opt, true, font);

		glColor3f(color.r, color.g, color.b);
		auto header_w = TextRenderer::get_width(chat_header.data(), font);;
		p._x += header_w;
		TextRenderer::rend_text(record->str, p, opt, true, font);
		p._x -= header_w;

		p._y += TextRenderer::get_height(font);
	}
}

////////////////////////////

Renderer::Renderer()
{
	glClearDepth(1.f);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT, GLU_FILL);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	init();
}


Renderer::~Renderer()
{
}

void Renderer::init()
{
	init_shader();
	init_resources();
}

void Renderer::init_shader()
{

	_default_shader = compile_shader("./Shader/vertex.glsl"sv, "./Shader/fragment.glsl"sv);

	_terrain_shader = compile_shader("./Shader/vertex.glsl"sv, "./Shader/terrain_fragment.glsl"sv);

	_billboard_shader = compile_shader("./Shader/grass_vertex.glsl"sv, "./Shader/png_fragment.glsl"sv);


}

void Renderer::init_resources()
{
	load_model();
	load_texture();
}

void Renderer::load_model()
{
	// create objs, give vaos for models
	auto box_vao = create_vao(_default_shader, box, 36);
	ObjDataPtr box_data = make_shared<OBJ_DATA>(box_vao);

	_cars.emplace_back(make_shared<OBJ>(box_data, _default_shader));
	_cars[0]->move({ 10.f,0.f,2.f });
	_cars.emplace_back(make_shared<OBJ>(box_data, _default_shader));
	_cars[1]->move({ 2.f,0.f,14.f });

	_player = make_shared<ControllObj>(size_t{ 0 }, box_data, _default_shader);

	_main_camera = make_shared<Camera>();
	_main_camera->set_ownner(_player.get());
	_main_camera->set_diff({ -5.f, 3.f, 0.f });

	box_vao = create_vao(_terrain_shader, box, 36);
	_terrain = make_shared<OBJ>(box_data, _terrain_shader);
	glm::vec3 scale_ = { 100.f, 0.25f, 100.f };
	_terrain->scaling(scale_);
	glm::vec3 move_ = { 0.f,(scale_.y * -1.f) - 1.f,0.f };
	_terrain->move(move_);

	auto grass_vao = create_vao(_billboard_shader, cross_billboard_3, 18);
	ObjDataPtr grass_data = make_shared<OBJ_DATA>(grass_vao);
	const auto grass_count = 8000;
	const auto grass_range = 50;
	_grasses.reserve(grass_count);
	for (int i = 0; i < grass_count; i++)
	{
		_grasses.emplace_back(grass_data, _billboard_shader);
	}
	for (auto& g : _grasses)
	{
		glm::vec3 pos = { rand() % (2 * grass_range) - grass_range, 0.f, rand() % (2 * grass_range) - grass_range };
		g.scaling(glm::vec3(1.0f));
		g.move(pos);
		g.move({ -HALF_ROOT3,-1.f,-1.f });
		g.rotate(glm::vec3{ 0.f, rand() % 360, 0.f });
	}
}

void Renderer::load_texture()
{
	std::string Dir{ "./Resource/Texture" };
	_player_tex = CreatePngTexture((Dir + "/RGB.png").c_str());

	_terrain_tex = CreatePngTexture((Dir + "/greenwool.png").c_str());

	_billboard_tex0 = CreatePngTexture((Dir + "/grass0.png").c_str());
	_billboard_tex1 = CreatePngTexture((Dir + "/grass1.png").c_str());
	_billboard_tex2 = CreatePngTexture((Dir + "/redflower.png").c_str());
	_billboard_tex3 = CreatePngTexture((Dir + "/blueflower.png").c_str());

}


GLuint Renderer::create_vao(GLuint shader, const VERTEX* vertices, GLsizei vertices_num)
{
	GLuint retvao;
	GLuint abo;
	GLuint ebo;

	glGenVertexArrays(1, &retvao);
	glBindVertexArray(retvao);

	glGenBuffers(1, &abo);
	glBindBuffer(GL_ARRAY_BUFFER, abo);
	glBufferData(GL_ARRAY_BUFFER, vertices_num * sizeof(VERTEX), vertices, GL_STATIC_DRAW);


	GLint positionAttribute = glGetAttribLocation(shader, "a_position");
	if (positionAttribute == -1) {
		std::cerr << "position 속성 설정 실패" << '\n';
		exit(-1);
	}
	glEnableVertexAttribArray(positionAttribute);
	glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (const GLvoid*)offsetof(VERTEX, pos));

	GLint normalAttribute = glGetAttribLocation(shader, "a_normal");
	if (normalAttribute == -1) {
		std::cerr << "normal 속성 설정 실패" << '\n';
		exit(-1);
	}
	glEnableVertexAttribArray(normalAttribute);
	glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (const GLvoid*)offsetof(VERTEX, nor));

	GLint texcoordAttribute = glGetAttribLocation(shader, "a_texcoord");
	if (texcoordAttribute == -1) {
		std::cerr << "texcoord 속성 설정 실패" << '\n';
		exit(-1);
	}
	glEnableVertexAttribArray(texcoordAttribute);
	glVertexAttribPointer(texcoordAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (const GLvoid*)offsetof(VERTEX, tex));


	glBindVertexArray(0);
	glDeleteBuffers(1, &abo);
	glDeleteBuffers(1, &ebo);

	return retvao;
}

void Renderer::add_shader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = (GLint)strlen(pShaderText);

	glShaderSource(ShaderObj, 1, p, Lengths);


	glCompileShader(ShaderObj);

	GLint success;

	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::read_file(string_view filename, std::string& target)
{
	std::ifstream file(filename.data());
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target.append(line.c_str());
		target.append("\n");
	}
	return true;
}

GLuint Renderer::compile_shader(string_view filenameVS, string_view filenameFS)
{
	GLuint ShaderProgram = glCreateProgram();

	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	if (!read_file(filenameVS, vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	if (!read_file(filenameFS, fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	add_shader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	add_shader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(0);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}

/////////////////////////////////////////////////////////////////////


void Renderer::reshape(const int w, const int h)
{
	const auto s_w = _screen.width;
	const auto s_h = _screen.height;
	const float s_aspect = _screen.aspect();
	const float aspect = (float)w / h;

	if (aspect < s_aspect)
	{
		const float ratio_W = (float)w / s_w;
		const int new_h = static_cast<int>(s_h * ratio_W);
		glViewport(0, (h - new_h) / 2, w, new_h);
	}
	else
	{
		const float ratio_h = (float)h / s_h;
		const int new_w = static_cast<int>(s_w * ratio_h);
		glViewport((w - new_w) / 2, 0, new_w, h);
	}


	draw();
}


/////////////////////////////////////////////////////////////////////

void Renderer::ready_draw()
{
	_vp_mat = proj_mat() * _main_camera->view_mat();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
}

void Renderer::draw()
{
	//timer::TIMER::instance().start();
	ready_draw();

	auto update_texture =
		[](GLuint shader, GLuint uloc, GLuint texture_id)
	{
		glProgramUniform1i(shader, uloc, texture_id);
		glActiveTexture(GL_TEXTURE0 + texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
	};

	/* logic */
	Sleep(1);
	float milli_tick = static_cast<float>(GAME_SYSTEM::instance().tick_time().count());
	float tick = milli_tick / 1000.f;
	_player->update(tick);
	_main_camera->update(tick);
	GAME_SYSTEM::instance().tick();
	// 로직분리, 카메라 회전 , 스크롤로 거리조절 >>  1tick == 최소 1ms.



	/* draw */


	glUseProgram(_default_shader);
	_cars[0]->bind_vao();
	update_texture(_default_shader, glGetUniformLocation(_default_shader, "u_tex_sampler"), _player_tex);
	for (auto& car : _cars)
	{
		car->update_uniform_vars(_default_shader);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	_player->update_uniform_vars(_default_shader);
	glDrawArrays(GL_TRIANGLES, 0, 36);



	glUseProgram(_terrain_shader);
	_terrain->bind_vao();
	_terrain->update_uniform_vars(_terrain_shader);
	update_texture(_terrain_shader, glGetUniformLocation(_terrain_shader, "u_tex_sampler"), _terrain_tex);
	glDrawArrays(GL_TRIANGLES, 0, 36);





	glUseProgram(_billboard_shader);
	glPolygonMode(GL_FRONT_AND_BACK, GLU_FILL);
	glDisable(GL_CULL_FACE);
	auto time = GAME_SYSTEM::instance().game_time();
	_grasses[0].bind_vao();
	for (int i = 0; auto & grass : _grasses)
	{
		glm::mat4 shear = glm::mat4(1);
		auto t = float(time) / 500 + grass.get_position().x / 10;
		auto ww = glm::cos(t) / 8;
		shear[1][0] = ww;
		shear[1][2] = ww;
		grass.update_uniform_vars(_billboard_shader);
		glUniformMatrix4fv(glGetUniformLocation(_billboard_shader, "u_shear_mat"), 1, GL_FALSE, glm::value_ptr(shear));
		update_texture(_billboard_shader, glGetUniformLocation(_billboard_shader, "u_tex_sampler"), _billboard_tex0 + (i++ % 4));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glDrawArrays(GL_TRIANGLES, 0, 18);
	}
	glPolygonMode(GL_FRONT, GLU_FILL);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);


	// text
	_player->render_chat({ 1,0,1 });
	render_chatrecord();

	//timer::TIMER::instance().end("T::");
	// 16.6	-> 60fps
	// 33	-> 30fps
	auto fps = 1000 / (GAME_SYSTEM::instance().tick_time().count() + 1);
	string title = "("s + to_string(fps) + " fps)"s;
	glutSetWindowTitle(title.c_str());
}


struct data
{
	mutex m;

	vector<player> players;
}

void render()
{
	while (true)
	{
		m.lock();
		read_data();
		draw();
		m.unlock();
	}
}

void networking()
{
	while (true)
	{
		if (recv_sometihing)
		{
			m.lock();
			writedata();
			m.unlock();
		}
	}
}

int main()
{
	thread renderthread{ render };
	thread networkingthred{ networking };
}