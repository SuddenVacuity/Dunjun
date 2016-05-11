#ifndef DUNJUN_SCENE_SCENENODE_HPP
#define DUNJUN_SCENE_SCENENODE_HPP

#include <Dunjun/Transform.hpp>

namespace Dunjun
{
	class SceneNode
	{
	public:
		using u_ptr = std::unique_ptr<SceneNode>; // quick typedef for unique pointers

		SceneNode();

		virtual ~SceneNode() {}

		SceneNode& attachChild(u_ptr child);
		u_ptr detachChild(const SceneNode& node);

		// children of only the node being searched
		SceneNode* findChildByName(const std::string& name) const;

		Transform getGlobaTransform() const;

		void onStart();
		void update(f32 dt);
		virtual void draw(Transform t = Transform());

		std::string name;
		Transform transform;
		ReadOnly<SceneNode*, SceneNode> parent;

	protected:
		virtual void onStartCurrent();
		void onStartChildren();

		virtual void updateCurrent(f32 dt);
		void updateChildren(f32 dt);

		virtual void drawCurrent(Transform t);
		void drawChildren(Transform t);


		std::vector<u_ptr> m_children;

	}; // end SceneNode
} // end Dunjun

#endif
