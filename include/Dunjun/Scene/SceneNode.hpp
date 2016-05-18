#ifndef DUNJUN_SCENE_SCENENODE_HPP
#define DUNJUN_SCENE_SCENENODE_HPP

#include <Dunjun/Scene/NodeComponent.hpp>

namespace Dunjun
{
	class SceneNode : public Drawable, private NonCopyable
	{
	public:
		using u_ptr = std::unique_ptr<SceneNode>; // quick typedef for unique pointers
		using GroupedComponentMap = std::map<std::type_index, std::vector<NodeComponent::u_ptr>>;

		SceneNode();

		virtual ~SceneNode() {}

		SceneNode& attachChild(u_ptr child);
		u_ptr detachChild(const SceneNode& node);

		// children of only the node being searched
		SceneNode* findChildByName(const std::string& name) const;
		SceneNode* findChildById(size_t id) const;

		Transform getGlobalTransform() const;

		void onStart();
		void update(f32 dt);

		const size_t id;
		std::string name;
		Transform transform;
		ReadOnly<SceneNode*, SceneNode> parent;
		bool visible = true;

		/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		)				.
		)					NODECOMPONENTS
		)
		)				.
		)					.
		)
		)				.
		%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

		SceneNode* addComponent(NodeComponent::u_ptr component);

		template <class Derived, class ... Args>
		inline SceneNode* addComponent(Args&& ... args)
		{
			return addComponent(make_unique<Derived>(args...));
		}
		
		inline void removeAllComponents()
		{
			for(auto& group : m_groupedComponents)
				group.second.clear();
			m_groupedComponents.clear();
		}

		template <class ComponentType>
		std::vector<NodeComponent::u_ptr>* getComponents()
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
				return std::static_pointer_cast<ComponentType>(c->at(0).get());
			
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
		friend class Renderer;

		void draw(Renderer& renderer, Transform t = Transform()) const;

		virtual void onStartCurrent();
		void onStartChildren();

		virtual void updateCurrent(f32 dt);
		void updateChildren(f32 dt);

		virtual void drawCurrent(Renderer& renderer, Transform t) const;
		void drawChildren(Renderer& renderer, Transform t) const;

		std::deque<u_ptr> m_children;
		// A GroupedComponentMap groups components of the same type together by type_index(...).hash_code()
		GroupedComponentMap m_groupedComponents;
	}; // end SceneNode
} // end Dunjun

#endif
