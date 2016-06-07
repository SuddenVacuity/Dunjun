#ifndef DUNJUN_SCENE_NODE_COMPONENT_HPP
#define DUNJUN_SCENE_NODE_COMPONENT_HPP

#include <Dunjun/Scene/SceneRenderer.hpp>

namespace Dunjun
{
	class SceneNode;
	class NodeComponent : private NonCopyable
	{
	public:
		using u_ptr = std::unique_ptr<NodeComponent>;

		NodeComponent() = default;

		virtual ~NodeComponent() {}

		virtual void onStart() {}
		virtual void update(Time dt) {}
		virtual void draw(SceneRenderer& renderer, Transform t) const {}

		SceneNode* parent = nullptr;

	private:
		friend class SceneNode;

	};





} // end Dunjun

#endif
