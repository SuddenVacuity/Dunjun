#ifndef DUNJUN_SCENE_MESH_RENDERER_HPP
#define DUNJUN_SCENE_MESH_RENDERER_HPP

#include <Dunjun/Material.hpp>

namespace Dunjun
{
	class MeshRenderer : public NodeComponent
	{
	public:
		explicit MeshRenderer(const Mesh& mesh, const Material& material);

		virtual void draw(Transform t);

		const Mesh* m_mesh;
		const Material* m_material;

	};

} // end Dunjun

#endif
