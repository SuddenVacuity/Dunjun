#ifndef DUNJUN_RESOURCE_HOLDER_OBJECTS_HPP
#define DUNJUN_RESOURCE_HOLDER_OBJECTS_HPP

#include <Dunjun/ResourceHolder.hpp>

namespace Dunjun
{
	//class TextureHolder : public ResourceHolder<Texture, std::string>
	//{
	//public:
	//	bool insertFromFile(IdentifierType id, const std::string& filename,
	//		TextureFilter minMagFilter = TextureFilter::Linear,
	//		TextureWrapMode wrapMode = TextureWrapMode::ClampToEdge)
	//	{
	//		std::unique_ptr<Texture> tex = make_unique<Texture>();
	//		bool t = tex->loadFromFile(BaseDirectories::Textures + filename, minMagFilter, wrapMode);
	//
	//		if(t == true)
	//			insert(id, std::move(tex));
	//
	//		return t;
	//	}
	//
	//	bool insertFromImage(IdentifierType id, const Image& image,
	//		TextureFilter minMagFilter = TextureFilter::Linear,
	//		TextureWrapMode wrapMode = TextureWrapMode::ClampToEdge)
	//	{
	//		std::unique_ptr<Texture> tex = make_unique<Texture>();
	//		bool t = tex->loadFromImage(image, minMagFilter, wrapMode);
	//
	//		if (t == true)
	//			insert(id, std::move(tex));
	//
	//		return t;
	//	}
	//};

	class ShaderHolder : public ResourceHolder<ShaderProgram, std::string>
	{
	public:
		bool insertFromFile(const std::string& id, 
			const char* vertexFilename,
			const char* fragmentFilename,
			bool bindDefaultAttribLocation = true)
		{
			std::unique_ptr<ShaderProgram> shaders = make_unique<ShaderProgram>();

			if (!shaders->attachShaderFromFile(Dunjun::ShaderType::Vertex, vertexFilename)) // check if the file loaded
			{
				std::cerr << shaders->errorLog << std::endl;
				return false;
			}

			if (!shaders->attachShaderFromFile(Dunjun::ShaderType::Fragment, fragmentFilename)) // check if the file loaded
			{
				std::cerr << shaders->errorLog << std::endl;
				return false;
			}

			if(bindDefaultAttribLocation)
			{
				shaders->bindAttribLocation((s32)AttribLocation::Position, "a_position"); // bind the position of 1st attribute in shaders
				shaders->bindAttribLocation((s32)AttribLocation::TexCoord, "a_texCoord"); // bind the position of 3rd attribute in shaders
				shaders->bindAttribLocation((s32)AttribLocation::Color, "a_color"); // bind the position of 2nd attribute in shaders
				shaders->bindAttribLocation((s32)AttribLocation::Normal, "a_normal"); // bind the position of 2nd attribute in shaders
			}

			if (!shaders->link())
			{
				std::cerr << (const std::string&)shaders->errorLog << std::endl;
				return false;
			}

			insert(id, std::move(shaders));

			return true;
		}

	};

	using MeshHolder = ResourceHolder<Mesh, std::string>;
	using MaterialHolder = ResourceHolder<Material, std::string>;


	//extern TextureHolder g_textureHolder;
	extern ShaderHolder g_shaderHolder;
	extern MeshHolder g_meshHolder;
	extern MaterialHolder g_materialHolder;
} // end Dunjun

#endif
