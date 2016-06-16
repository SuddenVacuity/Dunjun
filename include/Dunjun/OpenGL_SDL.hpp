#ifndef DUNJUN_OPENGL_HPP
#define DUNJUN_OPENGL_HPP

#include <Dunjun/Camera.hpp>
// this hpp file includes open glew and sdl for various operating systems

#ifndef GLFW_STATIC
#define GLFW_STATIC
#endif

#include <GL/glew.h>	
// Download GLEW libraries online. Add them to /external. Add GLEW_STATIC to preprocessor
// Download GLFW libraries online. Add them to /external
// RMB(Dunjun)>>properties>>C++>>>>General>>Include |add| external/glfw-3.1.2.bin.WIN32/include;
// RMB(Dunjun)>>properties>>Linker>>General>Additional Library Directories |add| external\glfw-3.1.2.bin.WIN32\lib-vc2015
// RMB(Dunjun)>>properties>>Linker>>Input>>Additional Dependencies |add| opengl32.lib;glfw3.lib;glfw3dll.lib; 
// http://www.glfw.org/documentation.html


// check the operating system of the machine
//#if defined(_WIN32) || defined(_WIN64) || defined (__WIN32__) || defined(__WINDOWS__)

//#include <GL/gl.h>

#if defined(__APPLE__) && defined(__MACH__)

#include <OpenGL/GL.h>

//#elif defined(__unix__)

//#include <GL/GL.h>

#else

#include <GL/gl.h>
//#include <GLFW/glfw3.h>

#endif

#include <SDL/SDL.h>

#endif
