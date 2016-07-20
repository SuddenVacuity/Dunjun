

#include <Dunjun/Scene/SceneRenderer.hpp>
#ifdef DUNJUN_SCENE_NODE_COMPONENT_HPP
#define DUNJUN_SCENE_NODE_COMPONENT_HPP
namespace Dunjun
{
	class SceneNode;
	struct NodeComponent
	{
	public:
		SceneNode* parent = nullptr;

		using u_ptr = std::unique_ptr<NodeComponent>;

		//NodeComponent() = default;

		NodeComponent() = default;
		virtual ~NodeComponent() {}

		virtual void init() {}
		virtual void update(Time dt) {}
		virtual void handleEvent(const Event& event) {}

		virtual void draw(SceneRenderer& renderer, Transform t) const {}

	private:
		NodeComponent(const NodeComponent&) = delete;
		NodeComponent& operator=(const NodeComponent&) = delete;
	};

} // end Dunjun

#endif
