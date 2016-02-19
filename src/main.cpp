/*

Programed by: Gerald Coggins
			  02/11/2016 -
			  
This is a project to make a rogue-like game from scratch. This program is my 
first attempt to program anything major so it's a big learning experience for me.

For this project I'll be following the video tutorial series Dunjun by Ginger Games
found here: https://www.youtube.com/playlist?list=PL93bFkoCMJslJJb15oQddnmABNUl6iz8e

*/
 
//#include "../include/Dunjun/Common.hpp" "../" means back one directory form the folder the .exe is in
#include <Dunjun/Common.hpp> // set iclude folder in RMB(Dunjun)>>properties>>C++>>General>>Include include;
#include <Dunjun/ShaderProgram.hpp>
#include <Dunjun/Image.hpp>

#include <GL/glew.h>	// Download GLEW libraries online. Add them to /external. Add GLEW_STATIC to preprocessor
#include <GLFW/glfw3.h> // Download GLFW libraries online. Add them to /external
						// RMB(Dunjun)>>properties>>C++>>>>General>>Include |add| external/glfw-3.1.2.bin.WIN32/include;
						// RMB(Dunjun)>>properties>>Linker>>General>Additional Library Directories |add| external\glfw-3.1.2.bin.WIN32\lib-vc2015
						// RMB(Dunjun)>>properties>>Linker>>Input>>Additional Dependencies |add| opengl32.lib;glfw3.lib;glfw3dll.lib; 
						// http://www.glfw.org/documentation.html



//#include <OpenGL/OpenGL.h> // Include openGL ADJUST FOR MAC
//#include <gl/GL.h> // Include openGL ADJUST FOR WINDOWS

#include <iostream>
#include <cmath>
#include <string> // include to use strings
#include <fstream> // include to open exteranl files
//
//
//
GLOBAL const int G_windowwidth = 854; // set global window width
GLOBAL const int G_windowheight = 488; // set global window height

class ShaderProgram
{
public:
	ShaderProgram();
private:
};



INTERNAL void glfwHints() // use this when creating a window to define what version of glfw is used
{
	glfwWindowHint(GLFW_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_VERSION_MINOR, 1);
}

int main(int argc, char** argv)
{
	GLFWwindow* window;

	if (!glfwInit()) // check if GLFW is initialized
		return EXIT_FAILURE;

	glfwHints(); // define glfw version before opening a window
	window = glfwCreateWindow(G_windowwidth, G_windowheight, "Dunjun", nullptr, nullptr); // create a window and set its name and context
	if (!window) // check if window is a null pointer
	{
		glfwTerminate(); // if it is terminate
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window); // set the context for the window

	glewInit();

	glEnable(GL_CULL_FACE); // enable culling faces
	glCullFace(GL_BACK); // specify to cull the back face

	// Here is where you add vertice information
	//
	float vertices[] = { // define vertexes for a triangle
	//   x		y		r	  g		b		s	  t
		 0.5f,  0.5f,	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,	// 0 vertex         1 ---- 0        
		-0.5f,  0.5f,	1.0f, 1.0f, 1.0f,	0.0f, 1.0f,	// 1 vertex           \             
		 0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,	// 2 vertex              \           
		-0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f	// 3 vertex         3 -----2       
												 // for triangle strips organize vertexes in a backwards Z
	};

	// create a vertex buffer object to move vertex data to the graphics card
	GLuint vbo;
	glGenBuffers(1, &vbo); // make vbo an array with a single entry

	glBindBuffer(GL_ARRAY_BUFFER, vbo); // bind the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // upload the data to the buffer
	/* Types of DRAW
	   GL_STATIC DRAW   / Draws once and is repeated many times
	   GL_DYNAMIC_DRAW  / Draw can be changed but repeats more often than it changes
	   GL_STREAM_DRAW   / Redraws every frame (memory intensive)
	 */

	Dunjun::ShaderProgram shaderProgram;
	shaderProgram.attachShaderFromFile(Dunjun::ShaderType::Vertex, "data/shaders/default_vert.glsl"); // get the shaders form file
	shaderProgram.attachShaderFromFile(Dunjun::ShaderType::Fragment, "data/shaders/default_frag.glsl");

	shaderProgram.bindAttribLocation(0, "vertPostition"); // bind the position of 1st attribute in shaders
	shaderProgram.bindAttribLocation(1, "vertColor"); // bind the position of 2nd attribute in shaders
	shaderProgram.bindAttribLocation(2, "vertTexCoord"); // bind the position of 3rd attribute in shaders

	shaderProgram.link();
	shaderProgram.use();

	GLuint tex; // declare a texture
	glGenTextures(1, &tex); // generate texture tex
	glBindTexture(GL_TEXTURE_2D, tex); // bind the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set the s axis (x) to repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // set the t axis (y) to repeat
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL REPEAT); // set the r axis (z) to repeat
	
	/* set the border color for GL_CLAMP_TO_BORDER
	float color[] = {1.0f, 0.0f, 0.0f, 1.0f}; // create float array with the color in it
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color); // apply the color to the border
	*/

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // set the texture min filter type
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // set the texture max filter type
															// there are 3 types of texture filters
															// GL_NEAREST	keeps it as close as possible pixel to pixel
															// GL_LINEAR	blurs the pixels
															// GL_MIPMAP	blurs the pixel differently


	Dunjun::Image image;
	image.loadFromFile("data/textures/dunjunText.jpg");

	/*
	unsigned char* image; // declare the name of the texture image
	int width, height, comp; // make variables for image
	image = stbi_load("data/textures/dunjunText.jpg", &width, &height, &comp, 0); // load the file and assign variables
	*/


	// create an array of pixels (checker board pattern)
	float pixels[] = {
		0,0,1,	1,0,0,
		0,1,0,	1,1,0,
	};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.pixelPtr()); // tell how to get the image from the pixels

	glActiveTexture(GL_TEXTURE0); // activate the texture
	shaderProgram.setUniform("uniTex", 0); // set uniform for GL_TEXTURE0 as uniTex

	/*
	std::string vertexShaderSource = stringfromfile("data/shaders/default_vert.glsl"); // load vertex shader from file
	const char* vertexShaderText = vertexShaderSource.c_str();

	std::string fragmentShaderSource = stringfromfile("data/shaders/default_frag.glsl"); // load fragment shader form file
	const char* fragmentShaderText = fragmentShaderSource.c_str();

	// start creating shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); //load the vertex shader in memory
	glShaderSource(vertexShader, 1, &vertexShaderText, nullptr); // assign vertex shader source to a shader (shader name, shader number, &shader source text, length)
	glCompileShader(vertexShader); // make vertexShader program available

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // load the fragment shader into memory
	glShaderSource(fragmentShader, 1, &fragmentShaderText, nullptr); // assign fragment shader source to a shader (shader name, shader number, &shader source text, length)
	glCompileShader(fragmentShader); // make fragmentShader program available
	
	GLuint shaderProgram = glCreateProgram(); // make a program that uses vertexShader and fragmentShader together
	glAttachShader(shaderProgram, vertexShader); // attach vertexShader
	glAttachShader(shaderProgram, fragmentShader); // attach fragmentShader

	glBindAttribLocation(shaderProgram, 0, "vertPosition"); // defines attribute vec2 vertPosition from vertexShaderText
	glBindAttribLocation(shaderProgram, 1, "vertColor"); // defines attribute vec3 vertColor from vertexShaderText

	// any modification to the attached programs must be done before linking
	glLinkProgram(shaderProgram); // link vertexShader and fragmentShader together

	glUseProgram(shaderProgram); // use the program
	*/

	//=================================================================================================
	// OPENING THE MAIN WINDOW
	//=================================================================================================
	//=================================================================================================
	bool running = true;
	bool fullscreen = false; // sets fullscreen to be off by default
	while(running) // create a loop that works until the window closes
	{
		{ // vbo viewport sizing hack
			int width, height; // vars used to define the size of the viewport
			glfwGetWindowSize(window, &width, &height);
			glViewport(0, 0, width, height);
		}

		glClearColor(0.3f, 0.6f, 0.9f, 1.0f); // set the default color (R,G,B,A)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the color buffer | out of the 3 main buffers
								/* Error occured here "glfw3.dll is missing" 
								    -copied glfw3.dll to Dunjun\Debug and it didn't work
								    -copied glfw3.dll to C:\Windows and it worked
									-went to make the dll included in the project instead of in C:\Windows
								    -tried RMB(Dunjun)>>properties>>Linker>>General>>Link Additional Dependencies |YES| and it still worked
								    -Deleted glfw3.dll in C:\Windows and Dunjun\Debug || and it still worked
								    -RMB(Dunjun)>>properties>>Linker>>Link Additional Dependencies |NO| and it still worked
								   I have no idea what fixed it*/

		// Draw things GLEW
		{	
			// Speicify the layout of the vertex data
			glEnableVertexAttribArray(0); // enables attribute array[0] vertPosition from glBindAttribLocation(shaderProgram)
			glEnableVertexAttribArray(1); // enables attribute array[1] vertColor ''
			glEnableVertexAttribArray(2); // enable attribute [2] vertTexCoord

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (const GLvoid*)0); // pointer for attribute position (att position, size of vertices x/y/z, int type, normalized?, stride, pointer)
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (const GLvoid*)(2 * sizeof(float)));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (const GLvoid*)(5 * sizeof(float)));
			// stride says how many floats there are per vertex
			// const void *pointer says how far offset the information starts

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); // (mode to draw in, first vertex, total vertices)

			glDisableVertexAttribArray(0); // disables attribute array[0]
			glDisableVertexAttribArray(1); // disables attribute array[1]
			glDisableVertexAttribArray(2); // disables attribute array[2]
		}

					
		glfwSwapBuffers(window); // switches information between the front buffer and the back buffer
		glfwPollEvents(); // waits for input and considers that input lower priorty than interrupt input

		if (glfwWindowShouldClose(window)|| // check if window should close
			glfwGetKey(window, GLFW_KEY_ESCAPE)) // checks if the escape key is pressed in window
			running = false;

		/* COMMENTED OUT BECAUSE IT CAUSES A CRASH
		if (glfwGetKey(window, GLFW_KEY_F11)) // press F11 to toggle between default and fullscreen
		{
			fullscreen = !fullscreen; // toggles true/false boolean for fullscreen
			GLFWwindow* newwindow;

			glfwHints(); // define glfw version before opening a window
			if (fullscreen) // action to take if fullscreen is true
			{
				int count; // declares mode count for monitor
				const GLFWvidmode* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count); // creates GLFWvidmode array of monitor modes
				newwindow = glfwCreateWindow(modes[count - 1].width, // modes[count - 1].width gets the width from the last mode
											 modes[count - 1].height, // modes[count - 1].height gets the height from the last mode
											 "Dunjun", glfwGetPrimaryMonitor(), window);  // and create the window
			}
			else // action to take if fullsscreen is not true
			{
				newwindow = glfwCreateWindow(G_windowwidth, G_windowheight, "Dunjun", nullptr, window);
			}

			glfwDestroyWindow(window); // destroys old window
			window = newwindow;
			glfwMakeContextCurrent(window);
		}
		*/
	}

	glfwDestroyWindow(window); // closes window named window
	glfwTerminate(); // terminates GLFW
	return EXIT_SUCCESS;
}