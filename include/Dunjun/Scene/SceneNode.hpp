
#include <Dunjun/Scene/NodeComponent.hpp>
#ifdef DUNJUN_SCENE_SCENENODE_HPP
#define DUNJUN_SCENE_SCENENODE_HPP

namespace Dunjun
{
	using ComponentId = size_t;

	namespace 
	{
		//template <class ComponentType>
		INTERNAL inline ComponentId getUniqueComponentId()
		{
			LOCAL_PERSIST ComponentId lastId = 0;
			return lastId++;
		}
	} // end anon namespace

	template <class ComponentType>
	inline ComponentId getComponentTypeId()
	{
		static_assert(std::is_base_of<NodeComponent, ComponentType>::value, "Dunjun::getComponentTypeId() <<<< Component Type must inherit from NodeComponent.");

		LOCAL_PERSIST ComponentId typeId = getUniqueComponentId();
		return typeId;
	}

	class SceneNode
	{
	public:
		using u_ptr = std::unique_ptr<SceneNode>; // quick typedef for unique pointers
		//using GroupedComponentMap = std::map<std::type_index, std::vector<NodeComponent::u_ptr>>;

		// used to track what kinds of components are added
		GLOBAL const size_t MaxComponents = 32;
		using ComponentBitset = std::bitset<MaxComponents>;
		using ComponentArray = std::array<NodeComponent*, MaxComponents>;

		using ID = u64;
		const ID id;
		std::string name;
		Transform transform = Transform();
		//ReadOnly<SceneNode*, SceneNode> parent;
		bool enabled = true;

		SceneNode* parent = nullptr;
		std::deque<u_ptr> children{};

		std::deque<NodeComponent::u_ptr> components{};
		ComponentArray componentArray{};
		ComponentBitset componentBitset{};

		explicit SceneNode();

		virtual ~SceneNode() {}

		SceneNode& attachChild(u_ptr&& child);
		u_ptr detachChild(const SceneNode& node);

		// children of only the node being searched
		SceneNode* findChildByName(const std::string& name) const;
		SceneNode* findChildById(size_t id) const;

		Transform getGlobalTransform() const;

		void init();
		void update(Time dt);
		void handleEvent(const Event& event);

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

		template <typename ComponentType, typename ... Args>
		ComponentType& addComponent(Args&& ... args)
		{
			////return addComponent(make_unique<Derived>(args...));
			//NodeComponent::u_ptr c = make_unique<ComponentType>(args...);
			//c->parent = this;
			//ComponentType* rawPtr = c.get();
			//components.push_back(std::move(c));

			static_assert(std::is_base_of<NodeComponent, ComponentType>::value, "Component must inherit from NodeComponent.");
			assert(!hasComponent<ComponentType>() && "SceneNode::addComponent component of that type already exists.");

			ComponentType* component = new ComponentType(std::forward<Args>(args)...);
			component->parent = this;

			components.emplace_back(std::unique_ptr<NodeComponent>(component));

			// mark this type of component as added
			componentArray[getComponentTypeId<ComponentType>()] = component;
			componentBitset[getComponentTypeId<ComponentType>()] = true;

			return *component;
		}
		
		//inline void removeAllComponents()
		//{
		//	for(auto& group : groupedComponents)
		//		group.second.clear();
		//	groupedComponents.clear();
		//}
		//
		//template <class ComponentType>
		//std::vector<NodeComponent::u_ptr>* getComponents()
		//{
		//	if(!std::is_base_of<NodeComponent, ComponentType>::value)
		//		return nullptr;
		//
		//	if(groupedComponents.size() == 0)
		//		return nullptr;
		//
		//	const std::type_index id(typeid(ComponentType));
		//
		//	for(auto& group : groupedComponents)
		//	{
		//		if(group.first == id)
		//			return &groupedComponents[id];
		//	}
		//
		//	return nullptr;
		//}

		template <class ComponentType>
		bool hasComponent() const
		{
			static_assert(std::is_base_of<NodeComponent, ComponentType>::value, "Component must inherit from NodeComponent.");
			return componentBitset[getComponentTypeId<ComponentType>()];
		}

		template <class ComponentType>
		ComponentType& getComponent()
		{
			//auto c = getComponents<ComponentType>();
			//
			//if(c)
			//	return std::static_pointer_cast<ComponentType>(c->at(0).get());
			//
			//return nullptr;

			static_assert(std::is_base_of<NodeComponent, ComponentType>::value, "Component must inherit from NodeComponent.");
			assert(hasComponent<ComponentType>() && "SceneNode::getComponent component not in this node.");

			auto ptr = componentArray[getComponentTypeId<ComponentType>()];

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

		virtual void initCurrent();
		void initChildren();

		virtual void updateCurrent(Time dt);
		void updateChildren(Time dt);

		virtual void handleEventCurrent(const Event& event);
		void handleEventChildren(const Event& event);

		virtual void drawCurrent(SceneRenderer& renderer, Transform t) const;
		void drawChildren(SceneRenderer& renderer, Transform t) const;

	private:
		SceneNode(const SceneNode&) = delete;
		SceneNode& operator=(const SceneNode&)= delete;
	}; // end SceneNode
} // end Dunjun

#endif
