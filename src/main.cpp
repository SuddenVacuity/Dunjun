
//#include "../include/Dunjun/Common.hpp" "../" means back one directory form the folder the .exe is in
#include <Dunjun/Common.hpp> // set iclude folder in RMB(Dunjun)>>properties>>C++>>General>>Include include;

#include <GL/glew.h>	// Download GLEW libraries online. Add them to /external. Add GLEW_STATIC to preprocessor
#include <GLFW/glfw3.h> // Download GLFW libraries online. Add them to /external
						// RMB(Dunjun)>>properties>>C++>>>>General>>Include |add| external/glfw-3.1.2.bin.WIN32/include;
						// RMB(Dunjun)>>properties>>Linker>>General>Additional Library Directories |add| external\glfw-3.1.2.bin.WIN32\lib-vc2015
						// RMB(Dunjun)>>properties>>Linker>>Input>>Additional Dependencies |add| opengl32.lib;glfw3.lib;glfw3dll.lib; 
						// http://www.glfw.org/documentation.html

//#include <OpenGL/OpenGL.h> // Include openGL ADJUST FOR MAC
//#include <gl/GL.h> // Include openGL ADJUST FOR WINDOWS

#include <iostream>
//
// 
// Shader Buffer Source
const char* vertexShaderText = {// create vertex shader
	"#version 120\n" // define GLSL version
	"\n"
	"attribute vec2 position;" // vec2 means the vector has 2 values, here it's x and y
	"void main()"
	"{"
	"gl_Position = vec4(position, 0.0, 1.0);"			  // vec4 means it has 4 values, x and y from position and z=0 and w=1
											  //"gl_Position = vec4(position.x, position.y, 0.0, 1,0);" //This works the same as the above
	"}"
};


const char* fragmentShaderText = {// create fragment shader
	"#version 120\n" // define GLSL version
	"\n"
	//"uniform vec3 triangleColor;"
	"void main()"
	"{"
	//"	gl_FragColor = vec4(triangleColor, 1.0);" // assign color
	"gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
	"}"
};



GLOBAL const int G_windowwidth = 854; // set global window width
GLOBAL const int G_windowheight = 488; // set global window height

void glfwHints() // use this when creating a window to define what version of glfw is used
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
	glfwMakeContextCurrent(window); // aet the context for the window

	glewInit();

	// Here is where you add vertice information
	//
	float vertices[] = { // define vertexes for a triangle
		 0.0f,  0.5f, // 1st vertex in counter-clockwise order
		-0.5f, -0.5f, // 2nd vertex in counter-clockwise order
		 0.5f, -0.5f // 3rd vertex in counter-clockwise order
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

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); //load the vertex shader in memory
	glShaderSource(vertexShader, 1, &vertexShaderText, nullptr); // assign vertex shader source to a shader (shader name, shader number, &shader source text, length)
	glCompileShader(vertexShader); // make vertexShader program available

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // load the fragment shader into memory
	glShaderSource(fragmentShader, 1, &fragmentShaderText, nullptr); // assign fragment shader source to a shader (shader name, shader number, &shader source text, length)
	glCompileShader(fragmentShader); // make fragmentShader program available

	GLuint shaderProgram = glCreateProgram(); // make a program that uses vertexShader and fragmentShader together
	glAttachShader(shaderProgram, vertexShader); // attach vertexShader
	glAttachShader(shaderProgram, fragmentShader); // attach fragmentShader

	glBindAttribLocation(shaderProgram, 0, "position"); // defines attribute vec2 position from vertexShaderText
	//glBindFragDataLocation(shaderProgram, 0, "triangleColor");

	// any modification to the attached programs must be done before linking
	glLinkProgram(shaderProgram); // link vertexShader and fragmentShader together
	glUseProgram(shaderProgram); // use the program

	//Get the location of triangle color
	//GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");

	bool running = true;
	bool fullscreen = false; // sets fullscreen to be off by default
	while(running) // create a loop that works until the window closes
	{
		// set the triangle color
		//glUniform3f(uniColor, 1.0, 0.0, 0.0);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // set the default color (R,G,B,A)
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
			//GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
			//glEnableVertexAttribArray(posAttrib);
			//glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
			//glDrawArrays(GL_TRIANGLES, 0, 3); // (mode to draw in, first, total vertices)
			//glDisableVertexAttribArray(posAttrib);
			
			glEnableVertexAttribArray(0); // enables attribute array[0] position from glBindAttribLocation(shaderProgram)
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr); // pointer for attribute position (att position, size of vertices x/y/z, int type, normalized?, stride, pointer)
			glDrawArrays(GL_TRIANGLES, 0, 3); // (mode to draw in, first, total vertices)
			glDisableVertexAttribArray(0); // disables attribute array[0]
		}

					
		glfwSwapBuffers(window); // switches information between the front buffer and the back buffer
		glfwPollEvents(); // waits for input and considers that input lower priorty than interrupt input

		if (glfwWindowShouldClose(window)|| // check if window should close
			glfwGetKey(window, GLFW_KEY_ESCAPE)) // checks if the escape key is pressed in window
			running = false;

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

	}

	glfwDestroyWindow(window); // closes window named window
	glfwTerminate(); // terminates GLFW
	return EXIT_SUCCESS;
}