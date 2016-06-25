#ifndef DUNJUN_GRAPHICS_SHADERPROGRAM_HPP
#define DUNJUN_GRAPHICS_SHADERPROGRAM_HPP

#include <Dunjun/Graphics/Mesh.hpp>

namespace Dunjun
{
	enum class ShaderType
	{
		Vertex,
		Fragment,
	};
	class ShaderProgram : public NonCopyable
	{
	public:
		u32 handle = 0;
		bool isLinked = false;
		std::string errorLog = "";

		ShaderProgram() = default; // Progarm for the shader
		virtual ~ShaderProgram();

		bool attachShaderFromFile(ShaderType type, const std::string& filename); // get shader info from file
		bool attachShaderFromMemory(ShaderType type, const std::string& filename);

		void use() const; // use
		bool isInUse() const; // check if in use
		void stopUsing() const;
		void checkInUse() const;

		bool link();

		void bindAttribLocation(u32 location, const std::string& name);

		s32 getAttribLocation( const std::string& name) const;
		s32 getUniformLocation( const std::string& name) const;

		void setUniform(const std::string& name, f32 x) const;
		void setUniform(const std::string& name, f32 x, f32 y) const;
		void setUniform(const std::string& name, f32 x, f32 y, f32 z) const;
		void setUniform(const std::string& name, f32 x, f32 y, f32 z, f32 w) const;
		void setUniform(const std::string& name, u32 x) const;
		void setUniform(const std::string& name, s32 x) const; // in case an interger is needed
							
		void setUniform(const std::string& name, bool x) const; // incase a bool is needed
						
		void setUniform(const std::string& name, const Vector2& v) const; // set u_model in vertex shader
		void setUniform(const std::string& name, const Vector3& v) const;
		void setUniform(const std::string& name, const Vector4& v) const;
		void setUniform(const std::string& name, const Matrix4& m) const;
						
		void setUniform(const std::string& name, const Quaternion& q) const;
		void setUniform(const std::string& name, const Transform& t) const;
		void setUniform(const std::string& name, const Color& c) const;

	private:
		mutable std::unordered_map<std::string, s32> m_attribLocations;
		mutable std::unordered_map<std::string, s32> m_uniformLocations;
	};
}



#endif
