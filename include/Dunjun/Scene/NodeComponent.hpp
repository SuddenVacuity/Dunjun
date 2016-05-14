#ifndef DUNJUN_SCENE_NODE_COMPONENT_HPP
#define DUNJUN_SCENE_NODE_COMPONENT_HPP

#include <Dunjun/Renderer.hpp>

namespace Dunjun
{
	class SceneNode;
	class NodeComponent : private NonCopyable
	{
	public:
		NodeComponent() = default;

		virtual ~NodeComponent() {}

		virtual void onStart() {}
		virtual void update(f32 dt) {}
		virtual void draw(Renderer& renderer, Transform t) {}

		SceneNode* parent = nullptr;

	private:
		friend class SceneNode;

	};





} // end Dunjun

#endif
