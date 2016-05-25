#ifndef DUNJUN_SCENE_SCENENODE_HPP
#define DUNJUN_SCENE_SCENENODE_HPP

#include <Dunjun/Scene/NodeComponent.hpp>

namespace Dunjun
{
	using ComponentId = size_t;

	namespace Impl
	{
		//template <class ComponentType>
		inline ComponentId getUniqueComponentId()
		{
			LOCAL_PERSIST ComponentId lastId = 0;
			return lastId++;
		}
	} // end Impl namespace

	template <class ComponentType>
	inline ComponentId getComponentTypeId()
	{
		static_assert(std::is_base_of<NodeComponent, ComponentType>::value, "Dunjun::getComponentTypeId() <<<< Component Type must inherit from NodeComponent.");

		LOCAL_PERSIST ComponentId typeId = Impl::getUniqueComponentId();
		return typeId;
	}

	class SceneNode : private NonCopyable
	{
	public:
		using u_ptr = std::unique_ptr<SceneNode>; // quick typedef for unique pointers
		//using GroupedComponentMap = std::map<std::type_index, std::vector<NodeComponent::u_ptr>>;

		// used to track what kinds of components are added
		GLOBAL const size_t MaxComponents = 32;
		using ComponentBitset = std::bitset<MaxComponents>;
		using ComponentArray = std::array<NodeComponent*, MaxComponents>;

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

		using ID = u64;
		const ID id;
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

		//SceneNode* addComponent(NodeComponent::u_ptr component);

		template <class ComponentType, class ... Args>
		ComponentType& addComponent(Args&& ... args)
		{
			////return addComponent(make_unique<Derived>(args...));
			//NodeComponent::u_ptr c = make_unique<ComponentType>(args...);
			//c->parent = this;
			//ComponentType* rawPtr = c.get();
			//m_components.push_back(std::move(c));

			assert(!hasComponent<ComponentType>());

			ComponentType* component = new ComponentType(std::forward<Args>(args)...);
			component->parent = this;

			m_components.push_back(std::unique_ptr<NodeComponent>(component));

			// mark this type of component as added
			m_componentArray[getComponentTypeId<ComponentType>()] = component;
			m_componentBitset[getComponentTypeId<ComponentType>()] = true;

			return *component;
		}
		
		//inline void removeAllComponents()
		//{
		//	for(auto& group : m_groupedComponents)
		//		group.second.clear();
		//	m_groupedComponents.clear();
		//}
		//
		//template <class ComponentType>
		//std::vector<NodeComponent::u_ptr>* getComponents()
		//{
		//	if(!std::is_base_of<NodeComponent, ComponentType>::value)
		//		return nullptr;
		//
		//	if(m_groupedComponents.size() == 0)
		//		return nullptr;
		//
		//	const std::type_index id(typeid(ComponentType));
		//
		//	for(auto& group : m_groupedComponents)
		//	{
		//		if(group.first == id)
		//			return &m_groupedComponents[id];
		//	}
		//
		//	return nullptr;
		//}

		template <class ComponentType>
		bool hasComponent() const
		{
			return m_componentBitset[getComponentTypeId<ComponentType>()];
		}

		template <class ComponentType>
		NodeComponent& getComponent()
		{
			//auto c = getComponents<ComponentType>();
			//
			//if(c)
			//	return std::static_pointer_cast<ComponentType>(c->at(0).get());
			//
			//return nullptr;

			assert(hasComponent<ComponentType>());

			auto ptr = m_componentArray[getComponentTypeId<ComponentType>()];

			return *reinterpret_cast<ComponentType*>(ptr);
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
		friend class SceneRenderer;

		void draw(SceneRenderer& renderer, Transform t = Transform()) const;

		virtual void onStartCurrent();
		void onStartChildren();

		virtual void updateCurrent(f32 dt);
		void updateChildren(f32 dt);

		virtual void drawCurrent(SceneRenderer& renderer, Transform t) const;
		void drawChildren(SceneRenderer& renderer, Transform t) const;

		std::deque<u_ptr> m_children;
		// A GroupedComponentMap groups components of the same type together by type_index(...).hash_code()
		//GroupedComponentMap m_groupedComponents;

		std::deque<NodeComponent::u_ptr> m_components;
		ComponentArray m_componentArray;
		ComponentBitset m_componentBitset;

	}; // end SceneNode
} // end Dunjun

#endif
