
#include <Dunjun/System/OpenGL_SDL.hpp>

#include <type_traits>

namespace Dunjun
{
	static_assert(std::is_same<GLubyte, u8>::value,
		"GLubyte is not the same as u8");
	static_assert(std::is_same<GLbyte, s8>::value,
		"GLbyte is not the same as s8");
	static_assert(std::is_same<GLushort, u16>::value,
		"GLushort is not the same as u16");
	static_assert(std::is_same<GLshort, s16>::value,
		"GLshort is not the same as s16");
	static_assert(std::is_same<GLuint, u32>::value,
		"GLuint is not the same as u32");
	static_assert(std::is_same<GLint, s32>::value,
		"GLint is not the same as s32");
	static_assert(std::is_same<GLsizei, s32>::value,
		"GLsizei is not the same as s32");
	static_assert(std::is_same<GLfloat, f32>::value,
		"GLfloat is not the same as f32");

	static_assert(GL_FALSE == false,
		"GL_FALSE is not the same as false");
	static_assert(GL_TRUE == true,
		"GL_TRUE is not the same as true");

	void glCheckError()
	{
#ifdef DUNJUN_DEBUG_MODE
		std::string message = "\nGetting OpenGL error report:";
		GLenum errorCode;

		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			switch (errorCode)
			{
			case GL_NO_ERROR:						 message += "\n    NO ERROR"; break;
			case GL_INVALID_ENUM:					 message += "\n    INVALID ENUM"; break;
			case GL_INVALID_VALUE:					 message += "\n    INVALID VALUE"; break;
			case GL_INVALID_INDEX:					 message += "\n    INVALID INDEX"; break;
			case GL_INVALID_OPERATION:				 message += "\n    INVALID OPERATION"; break;
			case GL_STACK_OVERFLOW:					 message += "\n    STACK OVERFLOW"; break;
			case GL_STACK_UNDERFLOW:				 message += "\n    STACK UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY:					 message += "\n    OUT OF MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:	 message += "\n    INVALID FRAMEBUFFER OPERATION"; break;
			case GL_CONTEXT_LOST:					 message += "\n    CONTEXT LOST"; break;
			case GL_TABLE_TOO_LARGE:				 message += "\n    TABLE TOO LARGE"; break;
			default: break;
			}

			errorCode = glGetError();
			if (errorCode == GL_NO_ERROR)
				std::cout << message;
		}

		std::cout << message;
#endif
	}
} // end Dunjun
