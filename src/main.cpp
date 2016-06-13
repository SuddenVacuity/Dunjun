/*

Programed by: Gerald Coggins
			  02/11/2016 -
			  
This is a project to make a rogue-like game from scratch. This program is my 
first attempt to program anything major so it's a big learning experience for me.

For this project I'll be following the video tutorial series Dunjun by Ginger Games
found here: https://www.youtube.com/watch?v=fRUYl6_5m3o

==============================================================
HEADER MAP
==============================================================

Config.hpp >> Types.hpp >> Common.hpp >> ResourceHolder.hpp >> ReadOnly.hpp >> NonCopyable.hpp >> Constants.hpp >> 
Unit.hpp >> Angle.hpp >>MathLib.hpp >> Vector2.hpp >> Vector3.hpp >> Vector4.hpp >> Matrix4.hpp >> Quaternion.hpp >>
Matrix.hpp >> Interpolation.hpp >> Random.hpp >> Functions.hpp >> Rectangle.hpp >> Math.hpp >>Transform.hpp >> 
Camera.hpp >> OpenGL.hpp >> Time.hpp >> Clock.hpp >> TickCounter.hpp >> Window.hpp >> Input.hpp >> Event.hpp >> Image.hpp >> 
Texture.hpp >> GBuffer.hpp >> RenderTexture.hpp >> Color.hpp >> Vertex.hpp >> Mesh.hpp >> ShaderProgram.hpp >> 
Material.hpp >> ModelAsset.hpp >> Lighting.hpp >> ResourceHolder_objects.hpp >> SceneRenderer.hpp >> NodeComponent.hpp >>
SceneNode.hpp >> MeshRenderer.hpp >> FaceCamera.hpp >> Scene.hpp >> Room.hpp >> Level.hpp >> Game.hpp >> 
main.cpp

*/


#include <Dunjun/Game.hpp>

int main(int argc, char** argv)
{
	Dunjun::Game::init();

	Dunjun::Game::run();

	Dunjun::Game::cleanUp();

	return 0;
}

//#include <Dunjun/Input.hpp>
//#include <Dunjun/Event.hpp>

//using namespace Dunjun;

//int main(int argc, char** argv)
//{
	//SDL_Init(SDL_INIT_EVERYTHING);

	//Window window;
	//Window window2;
	//window.create("Dunjun", 800, 600);
	//std::string textBuffer = {};
	//bool acceptInput = true; // this is to prevent rapid input

	//std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << std::endl;
	//std::cout << "Type in a command and press enter. [HELP] [EXIT]" << std::endl;
	//std::cout << ">> " << textBuffer << std::endl;

	//while(window.isOpen())
	//{
		//SDL_Event event;
		//SDL_PollEvent(&event);
		//
		//if(event.type == SDL_QUIT || Input::isKeyPressed(Input::Key::Escape))
		//{
		//	window.close();
		//	window2.close();
		//}
		//
		//// check for key presses
		//if((event.type == SDL_KEYDOWN) && acceptInput == true)
		//{
		//	std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << std::endl;
		//	// check for a command
		//	if (Input::isKeyPressed(Input::Key::Return))
		//	{
		//		if (textBuffer == "HELP")
		//		{
		//			std::cout << "Commands suck for now.\n--------------------" << std::endl;
		//			std::cout << "HELP: shows help" << std::endl;
		//			std::cout << "OPEN: open 2nd window - bugs the first windows rendering" << std::endl;
		//			std::cout << "CLOSE: closes 2nd window" << std::endl;
		//			std::cout << "EXIT/QUIT: closes program" << std::endl;
		//			textBuffer.clear();
		//		}
		//		else if (textBuffer == "OPEN")
		//		{
		//			window2.create("Dunjun2", 80, 60);
		//			textBuffer.clear();
		//		}
		//		else if (textBuffer == "CLOSE")
		//		{
		//			window2.close();
		//			textBuffer.clear();
		//		}
		//		else if (textBuffer == "EXIT" || textBuffer == "QUIT")
		//		{
		//			window.close();
		//			window2.close();
		//		}
		//		else
		//		{
		//			std::cout << "Invalid command." << std::endl;
		//			textBuffer.clear();
		//		}
		//	}
		//
		//	// Erase letters
		//	if (Input::isKeyPressed(Input::Key::Backspace))
		//		if(textBuffer.size() > 0)
		//			textBuffer.erase(textBuffer.size() - 1);
		//
		//	// Add letters TODO: do this without ififififififififififififififififififififififif
		//	if (Input::isKeyPressed(Input::Key::C))
		//		textBuffer.append("C");
		//	if (Input::isKeyPressed(Input::Key::E))
		//		textBuffer.append("E");
		//	if (Input::isKeyPressed(Input::Key::H))
		//		textBuffer.append("H");
		//	if (Input::isKeyPressed(Input::Key::I))
		//		textBuffer.append("I");
		//	if (Input::isKeyPressed(Input::Key::L))
		//		textBuffer.append("L");
		//	if (Input::isKeyPressed(Input::Key::N))
		//		textBuffer.append("N");
		//	if (Input::isKeyPressed(Input::Key::O))
		//		textBuffer.append("O");
		//	if (Input::isKeyPressed(Input::Key::P))
		//		textBuffer.append("P");
		//	if (Input::isKeyPressed(Input::Key::Q))
		//		textBuffer.append("Q");
		//	if (Input::isKeyPressed(Input::Key::S))
		//		textBuffer.append("S");
		//	if (Input::isKeyPressed(Input::Key::T))
		//		textBuffer.append("T");
		//	if (Input::isKeyPressed(Input::Key::U))
		//		textBuffer.append("U");
		//	if (Input::isKeyPressed(Input::Key::X))
		//		textBuffer.append("X");
		//
		//	acceptInput = false;
		//}
		//else if((event.type == SDL_KEYUP) && (acceptInput == false))
		//{
		//	acceptInput = true;
		//	std::cout << "\n\n\n\nType in a command and press enter. [HELP]" << std::endl;
		//	std::cout << ">> " << textBuffer << std::endl;
		//}
		//
		//if (Input::isMouseButtonPressed(Input::MouseButton::MouseButton_1))
		//	Input::setCursorPosition({ 400, 300 });
		//
		//
		//window2.display();
		//glClear(GL_COLOR_BUFFER_BIT);
		//glClearColor(1, 0, 0, 1);
		//
		//window.display();
		//glClear(GL_COLOR_BUFFER_BIT);
		//glClearColor(0, 1, 0, 1);


	//	Event event;
	//	while(window.pollEvent(event))
	//	{
	//		if(event.type == Event::Closed)
	//			window.close();
	//	}
	//	window.display();
	//}


	//SDL_Quit();

	//return 0;
//}

