/*

Programed by: Gerald Coggins
			  02/11/2016 -
			  
This is a project to make a rogue-like game from scratch. This program is my 
first attempt to program anything major so it's a big learning experience for me.

For this project I'll be following the video tutorial series Dunjun by Ginger Games
found here: https://www.youtube.com/playlist?list=PL93bFkoCMJslJJb15oQddnmABNUl6iz8e

==============================================================
HEADER MAP
==============================================================

Types.hpp >> Common.hpp >> Constants.hpp >> Unit.hpp >> Angle.hpp >> Vector2.hpp >> Vector3.hpp >> Vector4.hpp >> Matrix4.hpp
///<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<///
>> Matrix.hpp >> Quaternion.hpp >> Functions.hpp >> Math.hpp >> NonCopyable.hpp >> OpenGL.hpp >> Clock.hpp >> TickCounter.hpp
///<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<///
>> Image.hpp >> Texture.hpp >> Color.hpp >> ShaderProgram.hpp >> main.cpp

*/


#include <Dunjun/ShaderProgram.hpp>

#include <stb/stb_easy_font.h>

#include <iostream>
#include <cmath>
#include <fstream> // include to open exteranl files
#include <sstream>
#include <string> // include to use strings
#include <vector>
//
//
//
GLOBAL int g_windowWidth = 854; // set global window width
GLOBAL int g_windowHeight = 488; // set global window height

struct Vertex // must come before render
{
	Dunjun::Vector2 position;
	Dunjun::Color color;
	Dunjun::Vector2 texCoord;
};

struct ModelAsset // includes the shader, the texture and the vbo's
{ 
	Dunjun::ShaderProgram* shaders;
	Dunjun::Texture* texture;

	// create a vertex buffer object to move vertex data to the graphics card
	GLuint vbo; 
	GLuint ibo;

	GLenum drawType;
	//GLint drawStart;
	GLint drawCount;
};

struct ModelInstance // copies an asset to use
{
	ModelAsset* asset;
	Dunjun::Matrix4 transform;

};

GLOBAL Dunjun::ShaderProgram* g_defaultShader;
GLOBAL ModelAsset g_sprite;
GLOBAL std::vector<ModelInstance> g_instances;
GLOBAL Dunjun::Matrix4 g_cameraMatrix;

INTERNAL void glfwHints() // use this when creating a window to define what version of glfw is used
{
	glfwWindowHint(GLFW_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_VERSION_MINOR, 1);
	glfwSwapInterval(1); // replace this with working vsync later
}

INTERNAL void handleInput(GLFWwindow* window, bool* running, bool* fullscreen)
{
	if (glfwWindowShouldClose(window) || // check if window was closed
		glfwGetKey(window, GLFW_KEY_ESCAPE)) // checks if the escape key is pressed in window
		*running = false;
		

	/*
	if (glfwGetKey(window, GLFW_KEY_F11)) // press F11 to toggle between default and fullscreen
	{
		*fullscreen = !(*fullscreen); // toggles true/false boolean for fullscreen

		GLFWwindow* newwindow;

		glfwHints(); // define glfw version before opening a window
		if (*fullscreen) // action to take if fullscreen is true
		{
			int count;												// declares mode count for monitor
			const GLFWvidmode* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count); // creates GLFWvidmode array of monitor modes
			newwindow = glfwCreateWindow(modes[count - 1].width,	// modes[count - 1].width gets the width from the last mode
						modes[count - 1].height,								// modes[count - 1].height gets the height from the last mode
						"Dunjun", glfwGetPrimaryMonitor(), window);				// and create the window
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

// File path for shader files and define and bind attributes
INTERNAL void loadShaders()
{
	// Shader Program
	g_defaultShader = new Dunjun::ShaderProgram();
	if (!g_defaultShader->attachShaderFromFile(Dunjun::ShaderType::Vertex, "data/shaders/default_vert.glsl")) // check if the file loaded
		throw std::runtime_error(g_defaultShader->getErrorLog());

	if (!g_defaultShader->attachShaderFromFile(Dunjun::ShaderType::Fragment, "data/shaders/default_frag.glsl")) // check if the file loaded
		throw std::runtime_error(g_defaultShader->getErrorLog());


	g_defaultShader->bindAttribLocation(0, "a_position"); // bind the position of 1st attribute in shaders
	g_defaultShader->bindAttribLocation(1, "a_color"); // bind the position of 2nd attribute in shaders
	g_defaultShader->bindAttribLocation(2, "a_texCoord"); // bind the position of 3rd attribute in shaders

	if (!g_defaultShader->link())
		throw std::runtime_error(g_defaultShader->getErrorLog());

}

// vertex info, vbo and ibo
INTERNAL void loadSpriteAsset()
{
	using namespace Dunjun;

	// Here is where you add vertice information
	//
	Vertex vertices[] = { // define vertexes for a triangle
		 //  x	    y		 r	   g	 b	   a		s	  t
		{ {  0.5f,  0.5f },{ 0x00, 0xFF, 0xFF, 0xFF },{ 1.0f, 1.0f } },	// 0 vertex         1 ---- 0        
		{ { -0.5f,  0.5f },{ 0xFF, 0xFF, 0x00, 0xFF },{ 0.0f, 1.0f } },	// 1 vertex           \             
		{ {  0.5f, -0.5f },{ 0x00, 0x00, 0xFF, 0xFF },{ 1.0f, 0.0f } },	// 2 vertex              \           
		{ { -0.5f, -0.5f },{ 0xFF, 0x00, 0xFF, 0xFF },{ 0.0f, 0.0f } },	// 3 vertex         3 -----2       
																		// for triangle strips organize vertexes in a backwards Z
	};

	//g_sprite.vbo;
	glGenBuffers(1, &g_sprite.vbo); // make vbo an array with a single entry
	glBindBuffer(GL_ARRAY_BUFFER, g_sprite.vbo); // bind the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // upload the data to the buffer
				 /* Types of DRAW
				   GL_STATIC DRAW   / Draws once and is repeated many times
				   GL_DYNAMIC_DRAW  / Draw can be changed but repeats more often than it changes
				   GL_STREAM_DRAW   / Redraws every frame (memory intensive)
				*/

	u32 indices[] = {0, 1, 2, 2, 3, 1}; // vertex draw order for GL_TRIANGLES

	glGenBuffers(1, &g_sprite.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_sprite.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	g_sprite.shaders = g_defaultShader; // apply the default shader to sprite
	g_sprite.texture = new Texture(); // apply new texture to sprite
	g_sprite.texture->loadFromFile("data/textures/dunjunText.jpg");

	g_sprite.drawType = GL_TRIANGLES;
	// g_sprite.drawStart = 0;
	g_sprite.drawCount = 6;
}

// create instances of vertex info
INTERNAL void loadInstances()
{
	using namespace Dunjun;

	ModelInstance cat;
	cat.asset = &g_sprite;
	cat.transform = translate({2, 0, 0});
	g_instances.push_back(cat);

	ModelInstance dog;
	dog.asset = &g_sprite;
	dog.transform = translate({-2, 0, 0});
	g_instances.push_back(dog);
}

// shader info
INTERNAL void renderInstance(const ModelInstance& inst)
{
	ModelAsset* asset = inst.asset;
	Dunjun::ShaderProgram* shaders = asset->shaders;

	shaders->setUniform("u_camera", g_cameraMatrix); // set u_camera to apply view functions
	shaders->setUniform("u_model", inst.transform); // set u_model to apply matrix transform functions
	shaders->setUniform("u_tex", 0); // set texture position

	asset->texture->bind(0);
	glBindBuffer(GL_ARRAY_BUFFER, g_sprite.vbo); // bind the buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_sprite.ibo); // bind the buffer

	// Speicify the layout of the vertex data
	glEnableVertexAttribArray(0); // enables attribute array[0] a_position from glBindAttribLocation(shaderProgram)
	glEnableVertexAttribArray(1); // enables attribute array[1] a_color ''
	glEnableVertexAttribArray(2); // enable attribute [2] a_texCoord

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)0); // pointer for attribute position (att position, size of vertices x/y/z, int type, normalized?, stride, pointer)
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (const GLvoid*)sizeof(Dunjun::Vector2));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)(sizeof(Dunjun::Vector2) + sizeof(Dunjun::Color)));
	// stride says how many floats there are per vertex
	// const void *pointer says how far offset the information starts

	// get the draw info from ModelAsset asset
	//glDrawArrays(asset->drawType, asset->drawStart, asset->drawCount); // (mode to draw in, first vertex, total vertices)
	glDrawElements(asset->drawType, asset->drawCount, GL_UNSIGNED_INT, nullptr);

	glDisableVertexAttribArray(0); // disables attribute array[0]
	glDisableVertexAttribArray(1); // disables attribute array[1]
	glDisableVertexAttribArray(2); // disables attribute array[2]
}

INTERNAL void render()
{
	Dunjun::ShaderProgram* currentShaders = nullptr;

	for (const auto& inst : g_instances)
	{
		if (inst.asset->shaders != currentShaders) // swap to new shaders
		{
			currentShaders = inst.asset->shaders;
			currentShaders->use();
		}
		renderInstance(inst);

	}

	if (currentShaders) // checkif currentshader is in use
		currentShaders->stopUsing();
}

void renderUpdate(const ModelInstance& inst)
{
	ModelAsset* asset = inst.asset;
	Dunjun::ShaderProgram* shaders = asset->shaders;

	shaders->setUniform("u_camera", g_cameraMatrix); // set u_camera to apply view functions
	shaders->setUniform("u_model", inst.transform); // set u_model to apply matrix transform functions

}

namespace Debug
{
	union Color
	{
		Dunjun::u8 rgba[4];
		struct
		{
			Dunjun::u8 r, g, b, a;
		};
	};

	struct stb_font_vertex
	{
		Dunjun::f32 x, y, z;
		Color color;
	};

	// Text in the corner of the screen
	INTERNAL void drawString(GLFWwindow* window,
							 const std::string& text,
							 float x, float y, Color color)
	{
		LOCAL_PERSIST stb_font_vertex buffer[6000]; // ~500 chars
		int numQuads = stb_easy_font_print(
			x, y, (char*)text.c_str(), nullptr, buffer, sizeof(buffer));

		glPushMatrix();
		{
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			int width, height;
			glfwGetWindowSize(window, &width, &height);
			glOrtho(0.0f, (GLfloat)height, (GLfloat)width , 0.0f, -1.0f, 1.0f); // position of the text

			glColor4ubv(color.rgba);
			glBegin(GL_QUADS);
			for (int i = 0; i < numQuads; i++)
			{
				glVertex2f(buffer[4 * i + 3].x, buffer[4 * i + 3].y);
				glVertex2f(buffer[4 * i + 2].x, buffer[4 * i + 2].y);
				glVertex2f(buffer[4 * i + 1].x, buffer[4 * i + 1].y);
				glVertex2f(buffer[4 * i + 0].x, buffer[4 * i + 0].y);
			}
			glEnd();
		}
		glPopMatrix();
	}
} // namespace Debug


int main(int argc, char** argv)
{

	{ // test quaternions
		using namespace Dunjun;

		Quaternion q;
		Vector3 p;

		q = angleAxis(Degree(45), Vector3(0,0,1));
		p = {2,0,0};

		std::cout << q << std::endl;
		std::cout << p << std::endl;

		std::cout << (q * Quaternion(p, 0) * conjugate(q)).xyz << std::endl;
		std::cout << q * p << std::endl;
	}


	GLFWwindow* window;

	if (!glfwInit()) // check if GLFW is initialized
		return EXIT_FAILURE;

	glfwHints(); // define glfw version before opening a window
	window = glfwCreateWindow(g_windowWidth, g_windowHeight, "Dunjun", nullptr, nullptr); // create a window and set its name and context
	if (!window) // check if window is a null pointer
	{
		glfwTerminate(); // if it is terminate
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window); // set the context for the window

	glewInit();

	// Temporarily disable culling
	//glEnable(GL_CULL_FACE); // enable culling faces
	//glCullFace(GL_BACK); // specify to cull the back face

	// load internal render functions
	loadShaders();
	loadSpriteAsset();
	loadInstances();

	/*  Old Texture Loader
	GLuint tex; // declare a texture
	glGenTextures(1, &tex); // generate texture tex
	glBindTexture(GL_TEXTURE_2D, tex); // bind the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set the s axis (x) to repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // set the t axis (y) to repeat
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL REPEAT); // set the r axis (z) to repeat
	*/
	/* set the border color for GL_CLAMP_TO_BORDER
	float color[] = {1.0f, 0.0f, 0.0f, 1.0f}; // create float array with the color in it
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color); // apply the color to the border
	*/
	/*
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // set the texture min filter type
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // set the texture max filter type
															// there are 3 types of texture filters
															// GL_NEAREST	keeps it as close as possible pixel to pixel
															// GL_LINEAR	blurs the pixels
															// GL_MIPMAP	blurs the pixel differently
															*/
	// Replaced by Dunjun::Texture
	//Dunjun::Image image;
	//image.loadFromFile("data/textures/dunjunText.jpg");
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

	glBindAttribLocation(shaderProgram, 0, "a_position"); // defines attribute vec2 a_position from vertexShaderText
	glBindAttribLocation(shaderProgram, 1, "a_color"); // defines attribute vec3 a_color from vertexShaderText

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

	Dunjun::TickCounter tc;
	Dunjun::Clock frameClock;

	while(running) // create a loop that works until the window closes
	{
		// vbo viewport sizing hack
		int width, height; // vars used to define the size of the viewport
		glfwGetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);
		g_windowWidth = width; // change these to keep aspect ratio
		g_windowHeight = height; // ------/


		{
			using namespace Dunjun;


			
			// how the matrix moves
			Matrix4 model
				= translate({ 0.0f, 0.0f, 0.0f }) // translation { x, y, z }
				* rotate(Degree(glfwGetTime() * 60.0f), { 0, 1, 0 }) // rotation amount in radians { x, y, z axis to rotate on }
				* scale({ 2.0f, 1.0f, 1.0f }); // scale { x, y, z }

													   // where the camera is
			Matrix4 view = lookAt({ 0.0f, 0.0f, 5.0f } // {camera/eye position x, y, z}
								, { 0.0f, 0.0f, 0.0f } // {lookat target 0,0,0 is center x, y, z}
								, { 0, 1, 0 }); // {up direction x, y, z}

							// field of view perspective(Radian(TAU / x.xf),... or perspective(Degree(x),... changes the fov|| Radians must be floats
			Matrix4 proj = perspective(Degree(60.0f), (f32)g_windowWidth / (f32)g_windowHeight, 0.1f, 100.0f);

			g_cameraMatrix = proj * view; // combine to make camera

			void renderUpdate(const ModelInstance& inst);
		}

		glClearColor(0.3f, 0.6f, 0.9f, 1.0f); // set the default color (R,G,B,A)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		render();

		if (tc.update(0.5))
		{
			//std::cout << tc.getTickRate() << std::endl;
			std::stringstream ss;
			ss << "Dunjun - ms/F: " << 1000.0 / tc.getTickRate() << " - In need of vSync!"; // dynamic window title
			glfwSetWindowTitle(window, ss.str().c_str());
		}

		//Debug
		//Debug::drawString(
		//	window, titleStream.str(), 0, 0{{0xFF, 0xFF, 0xFF}});
					
		glfwSwapBuffers(window); // switches information between the front buffer and the back buffer
		glfwPollEvents(); // waits for input and considers that input lower priorty than interrupt input

		handleInput(window, &running, &fullscreen); // input handler

		while (frameClock.getElapsedTime() < 1.0 / 240.0)
			;
		frameClock.restart();

	}
	glfwDestroyWindow(window); // closes window named window
	glfwTerminate(); // terminates GLFW
	return EXIT_SUCCESS;
}
