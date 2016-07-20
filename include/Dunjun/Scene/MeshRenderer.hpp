

#include <Dunjun/Scene/SceneNode.hpp>
#ifdef DUNJUN_SCENE_MESH_RENDERER_HPP
#define DUNJUN_SCENE_MESH_RENDERER_HPP
namespace Dunjun
{
	struct MeshRenderer : NodeComponent
	{
	public:
		const Mesh* mesh;
		Material* material;

		explicit MeshRenderer(const Mesh* mesh, Material* material)
			: mesh(mesh)
			, material(material)
		{
		}

		explicit MeshRenderer(const ModelAsset& asset)
			: mesh(asset.mesh)
			, material(asset.material)
		{
		}

		virtual void draw(SceneRenderer& renderer, Transform t) const override
		{
			if(!material || !mesh)
				return;

			renderer.addModelInstance(*this, t);

			//renderer.setMaterial(material);
			//renderer.setUniforms(t);
			//
			//renderer.draw(mesh);

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
	};

} // end Dunjun

#endif
