
//#include "../include/Dunjun/Common.hpp" "../" means back one directory form the folder the .exe is in
#include <Dunjun/Common.hpp> // set iclude folder in RMB(Dunjun)>>properties>>C++>>General>>Include include;

#include <GLFW/glfw3.h> // Download GLFW libraries online. Add them to /external
						// RMB(Dunjun)>>properties>>C++>>>>General>>Include |add| external/glfw-3.1.2.bin.WIN32/include;
						// RMB(Dunjun)>>properties>>Linker>>General>Additional Library Directories |add| external\glfw-3.1.2.bin.WIN32\lib-vc2015
						// RMB(Dunjun)>>properties>>Linker>>Input>>Additional Dependencies |add| opengl32.lib;glfw3.lib;glfw3dll.lib; 
						// http://www.glfw.org/documentation.html
#include <iostream>

GLOBAL const int G_windowwidth = 854; // set global window width
GLOBAL const int G_windowheight = 488; // set global window height

int main(int argc, char** argv)
{
	GLFWwindow* window;

	if (!glfwInit()) // initialize GLFW library
		return EXIT_FAILURE;

	window = glfwCreateWindow(G_windowwidth, G_windowheight, "Dunjun", nullptr, nullptr); // create a window and set its name and context
	if (!window) // check if window is a null pointer
	{
		glfwTerminate(); // if it is terminate
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window); // aet the context for the window

	bool running = true;
	bool fullscreen = false; // sets fullscreen to be off by default
	while(running) // create a loop that works until the window closes
	{
		glClearColor(0.5f, 0.69f, 1.0f, 1.0f); // set the default color (R,G,B,A)
		glClear(GL_COLOR_BUFFER_BIT); // clear the color buffer | out of the 3 main buffers
								/* Error occured here "glfw3.dll is missing" 
								    -copied glfw3.dll to Dunjun\Debug and it didn't work
								    -copied glfw3.dll to C:\Windows and it worked
									-went to make the dll included in the project instead of in C:\Windows
								    -tried RMB(Dunjun)>>properties>>Linker>>General>>Link Additional Dependencies |YES| and it still worked
								    -Deleted glfw3.dll in C:\Windows and Dunjun\Debug || and it still worked
								    -RMB(Dunjun)>>properties>>Linker>>Link Additional Dependencies |NO| and it still worked
								   I have no idea what fixed it*/
					
		glfwSwapBuffers(window); // switches information between the front buffer and the back buffer
		glfwPollEvents(); // waits for input and considers that input lower priorty than interrupt input

		if (glfwWindowShouldClose(window)|| // check if window should close
			glfwGetKey(window, GLFW_KEY_ESCAPE)) // checks if the escape key is pressed in window
			running = false;

		if (glfwGetKey(window, GLFW_KEY_F11)) // press F11 to toggle between default and fullscreen
		{
			fullscreen = !fullscreen; // toggles true/false boolean for fullscreen
			GLFWwindow* newwindow;

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