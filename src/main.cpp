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

Types.hpp >> ReadOnly.hpp >> Common.hpp >> Constants.hpp >> 
Unit.hpp >> Angle.hpp >> Vector2.hpp >> Vector3.hpp >> Vector4.hpp >> 
Matrix4.hpp >> Matrix.hpp >> Quaternion.hpp >> Interpolation.hpp >>
Functions.hpp >> Math.hpp >> Transform.hpp >> Camera.hpp >> NonCopyable.hpp >> 
OpenGL.hpp >> Clock.hpp >> TickCounter.hpp >> Image.hpp >> Texture.hpp >> 
Color.hpp >> Vertex.hpp >> ShaderProgram.hpp >> Game.hpp >>  Input.hpp >> main.cpp

*/


#include <Dunjun/Input.hpp>


int main(int argc, char** argv)
{
	Dunjun::Game::init();

	Dunjun::Game::run();

	Dunjun::Game::cleanUp();

	return EXIT_SUCCESS;
}
