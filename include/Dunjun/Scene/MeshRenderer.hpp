#ifndef DUNJUN_SCENE_MESH_RENDERER_HPP
#define DUNJUN_SCENE_MESH_RENDERER_HPP

#include <Dunjun/Scene/SceneNode.hpp>

namespace Dunjun
{
	class MeshRenderer : public NodeComponent
	{
	public:
		explicit MeshRenderer(const Mesh& mesh, const Material& material)
			: mesh(&mesh)
			, material(&material)
		{
		}

		explicit MeshRenderer(const ModelAsset& asset)
			: mesh(asset.mesh)
			, material(asset.material)
		{
		}

		virtual void draw(Renderer& renderer, Transform t) const
		{
			ShaderProgram* shaders = material->shaders;
			const Texture* tex = material->texture;

			renderer.setShaders(shaders);
			renderer.setTexture(tex);
			renderer.setUniforms(t);

			mesh->draw();

			//shaders->use();
			//Texture::bind(tex, 0);
			//
			//shaders->setUniform("u_camera", camera.getMatrix()); // shaderprogram.cpp
			//shaders->setUniform("u_transform", t); // shaderprogram.cpp
			//shaders->setUniform("u_tex", (Dunjun::u32)0); // shaderprogram.cpp
			//
			//mesh->draw();
			//
			//shaders->stopUsing();
			//
			//Texture::bind(nullptr, 0);
		}

		const Mesh* mesh;
		const Material* material;

	};

} // end Dunjun

#endif
