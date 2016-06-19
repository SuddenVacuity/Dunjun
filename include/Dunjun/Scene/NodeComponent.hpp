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

		//NodeComponent() = default;

		virtual ~NodeComponent() {}

		virtual void init() {}
		virtual void update(Time dt) {}
		virtual void handleEvent(const Event& event) {}

		virtual void draw(SceneRenderer& renderer, Transform t) const {}

		SceneNode* getParent() const
		{
			return m_parent;
		}

	private:
		friend class SceneNode;

		SceneNode* m_parent = nullptr;
	};





} // end Dunjun

#endif
