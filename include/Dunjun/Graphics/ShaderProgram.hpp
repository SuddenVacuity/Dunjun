#ifndef DUNJUN_GRAPHICS_SHADERPROGRAM_HPP
#define DUNJUN_GRAPHICS_SHADERPROGRAM_HPP

#include <Dunjun/Graphics/RenderTexture.hpp>

namespace Dunjun
{
	enum class ShaderType
	{
		Vertex,
		Fragment,
	};
	class ShaderProgram
	{
	public:
		u32 handle = 0;
		b32 isLinked = false;
		String errorLog = "";

		ShaderProgram(); // Progarm for the shader
		~ShaderProgram();

		bool attachShaderFromFile(ShaderType type, const String& filename); // get shader info from file
		bool attachShaderFromMemory(ShaderType type, const String& filename);

		void use() const; // use
		bool isInUse() const; // check if in use
		void stopUsing() const;
		void checkInUse() const;

		bool link();

		void bindAttribLocation(s32 location, const String& name);

		s32 getAttribLocation( const String& name) const;
		s32 getUniformLocation( const String& name) const;

		void setUniform(const String& name, f32 x) const;
		void setUniform(const String& name, f32 x, f32 y) const;
		void setUniform(const String& name, f32 x, f32 y, f32 z) const;
		void setUniform(const String& name, f32 x, f32 y, f32 z, f32 w) const;
		void setUniform(const String& name, u32 x) const;
		void setUniform(const String& name, s32 x) const; // in case an interger is needed
							  
		void setUniform(const String& name, bool x) const; // incase a bool is needed
							  
		void setUniform(const String& name, const Vector2& v) const; // set u_model in vertex shader
		void setUniform(const String& name, const Vector3& v) const;
		void setUniform(const String& name, const Vector4& v) const;
		void setUniform(const String& name, const Matrix4& m) const;
							  
		void setUniform(const String& name, const Quaternion& q) const;
		void setUniform(const String& name, const Transform& t) const;
		void setUniform(const String& name, const Color& c) const;

	private:
		mutable HashMap<s32> m_attribLocations;
		mutable HashMap<s32> m_uniformLocations;

		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
	}; // end class ShaderProgram
}



#endif
