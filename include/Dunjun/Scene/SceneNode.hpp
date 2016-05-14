#ifndef DUNJUN_SCENE_SCENENODE_HPP
#define DUNJUN_SCENE_SCENENODE_HPP

#include <Dunjun/Scene/NodeComponent.hpp>

namespace Dunjun
{
	class SceneNode : private NonCopyable
	{
	public:
		using u_ptr = std::unique_ptr<SceneNode>; // quick typedef for unique pointers
		using GroupedComponentMap = std::map<std::type_index, std::vector<NodeComponent*>>;

		SceneNode();

		virtual ~SceneNode() {}

		SceneNode& attachChild(u_ptr child);
		u_ptr detachChild(const SceneNode& node);

		// children of only the node being searched
		SceneNode* findChildByName(const std::string& name) const;

		Transform getGlobalTransform() const;

		void onStart();
		void update(f32 dt);
		virtual void draw(Renderer& renderer, Transform t = Transform());

		std::string name;
		Transform transform;
		ReadOnly<SceneNode*, SceneNode> parent;

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					NODECOMPONENTS
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		SceneNode* addComponent(NodeComponent* component);

		template <class Derived, class ... Args>
		inline SceneNode* addComponent(Args&& ... args)
		{
			return addComponent(new Derived(args...));
		}
		
		inline void removeAllComponents()
		{
			for(auto& group : m_groupedComponents)
				group.second.clear();
			m_groupedComponents.clear();
		}

		template <class ComponentType>
		std::vector<NodeComponent*>* getComponents()
		{
			if(!std::is_base_of<NodeComponent, ComponentType>::value)
				return nullptr;

			if(m_groupedComponents.size() == 0)
				return nullptr;

			const std::type_index id(typeid(ComponentType));

			for(auto& group : m_groupedComponents)
			{
				if(group.first == id)
					return &m_groupedComponents[id];
			}

			return nullptr;
		}

		template <class ComponentType>
		NodeComponent* getComponent()
		{
			auto c = getComponents<ComponentType>();

			if(c)
				return std::static_pointer_cast<ComponentType>(c->at(0));
			
			return nullptr;
		}

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					PROTECTED
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
	protected:
		virtual void onStartCurrent();
		void onStartChildren();

		virtual void updateCurrent(f32 dt);
		void updateChildren(f32 dt);

		virtual void drawCurrent(Renderer& renderer, Transform t);
		void drawChildren(Renderer& renderer, Transform t);

		std::vector<u_ptr> m_children;
		// A GroupedComponentMap groups components of the same type together by type_index(...).hash_code()
		GroupedComponentMap m_groupedComponents;

	}; // end SceneNode
} // end Dunjun

#endif
