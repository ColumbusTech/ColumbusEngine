#include <Engine.h>

using namespace Columbus;

int FPS_LIMIT = 120;

void kod_rle(std::ifstream& inpf, std::ofstream& outf)
{
	char sym;//������, ������� �� ����� ���������
	int kol = 1;// ���������� ������������� ��������
	while (!inpf.eof())
	{
		inpf.get(sym);//��������� ������
		if (sym != inpf.peek())// ���� ������ �� ��������� �� �������� � �����
		{
			//outf&lt; &lt; kol&lt; &lt; sym;// ���������� ���������� � �������� ����
			outf << kol << sym;
			kol = 0;
		}
		kol++;
	}

}


int main(int argc, char** argv)
{
	C_SDLWindowConfig config;
	config.Resizable = true;
	C_SDLWindow window(config);
	C_EventSystem event;
	event.addWindow(&window);
	GUI::C_IO io;
	C_Input input;
	input.setWindow(&window);
	input.setIO(&io);

	C_Camera camera;

	float i = 0;

	C_Cubemap cubemap("Data/Skyboxes/1.cubemap");
	C_Skybox skybox(&cubemap);

	C_Timer timer;

	int FPS = 0;

	//window.setVerticalSync(true);
	window.setFPSLimit(60);

	input.showMouseCursor(false);

	bool cursor = false;

	C_Scene scene;

	scene.load("Data/1.scene");

	scene.setSkybox(&skybox);
	scene.setCamera(&camera);

	//C_Image img("Data/Textures/metal2.tga");
	//img.save("Data/Textures/spark.tga", E_IMAGE_SAVE_FORMAT_TGA);
	
	while (window.isOpen())
	{
		float RedrawTime = window.getRedrawTime();

		event.pollEvents();
		input.update();

		window.clear(0, 0, 0.75, 1);

		C_SetPerspective(60, window.aspect(), 0.001, 1000);

		if (input.getKey(SDL_SCANCODE_W))
			camera.addPos(camera.direction() * RedrawTime * 5);
		if (input.getKey(SDL_SCANCODE_S))
			camera.addPos(-camera.direction() * RedrawTime * 5);
		if (input.getKey(SDL_SCANCODE_A))
			camera.addPos(-camera.right() * RedrawTime * 5);
		if (input.getKey(SDL_SCANCODE_D))
			camera.addPos(camera.right() * RedrawTime * 5);
		if (input.getKey(SDL_SCANCODE_UP))
			camera.addRot(C_Vector3(-125 * RedrawTime, 0, 0));
		if (input.getKey(SDL_SCANCODE_DOWN))
			camera.addRot(C_Vector3(125 * RedrawTime, 0, 0));
		if (input.getKey(SDL_SCANCODE_LEFT))
			camera.addRot(C_Vector3(0, 125 * RedrawTime, 0));
		if (input.getKey(SDL_SCANCODE_RIGHT))
			camera.addRot(C_Vector3(0, -125 * RedrawTime, 0));

		if (input.getKey(SDL_SCANCODE_LSHIFT))
			camera.addPos(-camera.up() * RedrawTime * 5);
		if (input.getKey(SDL_SCANCODE_SPACE))
			camera.addPos(camera.up() * RedrawTime * 5);
		if (input.getKey(SDL_SCANCODE_Q))
			camera.addRot(C_Vector3(0, 0, 125 * RedrawTime));
		if (input.getKey(SDL_SCANCODE_E))
			camera.addRot(C_Vector3(0, 0, -125 * RedrawTime));

		if (input.getKeyDown(SDL_SCANCODE_ESCAPE))
		{
			cursor = !cursor;
			input.showMouseCursor(cursor);
		}

		if (!cursor)
		{
			C_Vector2 deltaMouse = input.getMouseMovement();
			camera.addRot(C_Vector3(deltaMouse.y, -deltaMouse.x, 0) * 0.3);
			input.setMousePos(window.getSize() * 0.5);
		}

		camera.update();

		scene.setContextSize(window.getSize());
		scene.update();
		scene.render();

		window.display();

		if ((timer.elapsed()) > 1.0)
		{
			printf("%i\n", window.getFPS());
			timer.reset();
		}
	}

	return 0;
}


