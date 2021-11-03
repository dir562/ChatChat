

#include "stdafx.h"
#include "Renderer.h"
#include "VAO_OBJ.h"
#include "Camera.h"
#include "Game.h"


void RenderScene(void)
{
	Game::instance().renderer.draw();

}

void Idle(void)
{
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
	RenderScene();
}

void MouseWheel(int button, int dir, int x, int y) {
	auto camera = Renderer::instance().get_main_camera();
	if (dir > 0)
	{
		auto diff = camera->get_diff();
		camera->set_diff(diff * 0.875f);
	}
	else
	{
		auto diff = camera->get_diff();
		camera->set_diff(diff * 1.125f);
	}

	RenderScene();
}

void KeyInput(unsigned char key, int x, int y)
{
	Game::instance().player->apply_input_press(key);
	return;
	switch (key)
	{
	case 'w':
	case 'W':
	case 's':
	case 'S':
	case 'a':
	case 'A':
	case 'd':
	case 'D':
	case ' ':
		break;
	case 164: { /* 한글입력 */

	}break;
	default: break;
	}

	//RenderScene();
}

void KeyInputUp(unsigned char key, int x, int y)
{
		Game::instance().player->apply_input_unpress(key);
		return;
	switch (key)
	{
	case 'w':
	case 'W':
	case 's':
	case 'S':
	case 'd':
	case 'D':
	case 'a':
	case 'A':
	case ' ':
		break;
	case 164: { /* 한글입력 */

	}break;
	default: break;
	}

	//RenderScene();
}

void SpecialKeyInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_SHIFT_L:
		Game::instance().renderer.get_main_camera()->camera_shake(0.2f);
		break;
	default: break;
	}
}

void Reshape(int w, int h)
{
	Renderer::instance().reshape(w, h);
}

void Position(int x, int y)
{
	RenderScene();
}

int main(int argc, char** argv)
{


	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(g_WindowSizeX, g_WindowSizeY);
	glutCreateWindow("OPGL_SUNKUE");
	glewInit();
	if (glewIsSupported("GL_VERSION_4_5"))
	{
		std::cout << " GL Version is 4.5\n ";
	}
	else
	{
		std::cout << "GLEW 4.5 not supported\n ";
	}
	

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyInput);
	glutKeyboardUpFunc(KeyInputUp);
	glutMouseFunc(MouseInput);
	glutMouseWheelFunc(MouseWheel);
	glutSpecialFunc(SpecialKeyInput);
	glutReshapeFunc(Reshape);
	glutPositionFunc(Position);

	glutMainLoop();
}

