
#include <Dunjun/Scene/SceneNode.hpp>

//#include <cstdio> // included in Common.hpp

namespace Dunjun
{
	namespace 
	{
		//GLOBAL size_t idCount = 0;
		INTERNAL inline SceneNode::ID getUniqueSceneNodeId()
		{
			LOCAL_PERSIST SceneNode::ID lastId = 0;
			return lastId++;
		}
	} // end anon namespace

	// TODO idCount-- when destrucitng
	SceneNode::SceneNode()
		: id(getUniqueSceneNodeId())
		, componentArray()
	{
		name = stringFormat("node_%llu", id);
	}

	SceneNode& SceneNode::attachChild(u_ptr&& child)
	{
		// get the 
		child->parent = this;
		children.emplace_back(std::move(child));

		return *this;
	}

	SceneNode::u_ptr SceneNode::detachChild(const SceneNode& node)
	{
		// check through all children to find if the one to detach exists
		auto found = std::find_if(std::begin(children), 
								  std::end(children), 
		// lambda function: [](){}
		// leave square brackets blank to bring in no variables
		// [&] or [&variable] bring in all varibles by reference or single variable by reference respectively
		// [=] or [=variable] bring in all varibles by copy or single variable by copy respectively
		[&node] (u_ptr& child)
		{
			return child.get() == &node;
		}
		);

		// if child is found before the end of m_children allow it to be moved and return the pointer to it
		if(found != std::end(children))
		{
			u_ptr result = std::move(*found);

			result->parent = nullptr; // remove parent pointer form result
			children.erase(found); // remove child pointer from m_children

			return result;
		}

		return nullptr;
	}

	// TODO: return the id of all with the same name
	SceneNode* SceneNode::findChildByName(const std::string& name) const
	{
		for(const u_ptr& child : children)
		{
			if(child->name == name)
				return child.get();

			// check child nodes children as well
			SceneNode* subChild = child->findChildByName(name);
			if (subChild != nullptr)
				return subChild;
		}

		return nullptr;
	}

	SceneNode* SceneNode::findChildById(size_t id) const
	{
		for (const u_ptr& child : children)
		{
			if (child->id == id)
				return child.get();

			// check child nodes children as well
			SceneNode* subChild = child->findChildById(id);
			if(subChild != nullptr)
				return subChild;
		}

		return nullptr;
	}

	Transform SceneNode::getGlobalTransform() const
	{
		Transform result;

		// go through all parents applying their transforms
		for(const SceneNode* node = this; node != nullptr; node = node->parent)
			result *= node->transform;

		return result;
	}

	void SceneNode::init()
	{
		initCurrent();
		initChildren();

		// check node components
		for(auto& component : components)
		{
			component->init();
		}
	}

	void SceneNode::update(Time dt)
	{
		updateCurrent(dt);
		updateChildren(dt);

		for (auto& component : components)
		{
			component->update(dt);
		}
	}

	void SceneNode::handleEvent(const Event& event)
	{
		handleEventCurrent(event);
		handleEventChildren(event);

		for(auto& component : components)
			component->handleEvent(event);
	}

	void SceneNode::draw(SceneRenderer& renderer, Transform t) const
	{
		if(!enabled)
			return;

		t *= this->transform;

		drawCurrent(renderer, t);
		drawChildren(renderer, t);

		for (auto& component : components)
			component->draw(renderer, t);
	}

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					NODECOMPONENTS
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	//SceneNode* SceneNode::addComponent(NodeComponent::u_ptr component)
	//{
	//	component->parent = this;
	//
	//	const std::type_index id(typeid(*component));
	//
	//	m_components[id].push_back(std::move(component));
	//
	//	return this;
	//}

	/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	)				.
	)					PROTECTED
	)
	)				.
	)					.
	)
	)				.
	%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

	void SceneNode::initCurrent()
	{
		// Do nothing by default
	}

	void SceneNode::initChildren()
	{
		for(u_ptr& child : children)
			child->init();
	}

	void SceneNode::updateCurrent(Time dt)
	{
		// Do nothing by default
	}

	void SceneNode::updateChildren(Time dt)
	{
		for (u_ptr& child : children)
			child->update(dt);
	}

	void SceneNode::handleEventCurrent(const Event& event)
	{
		// do nothing by default
	}

	void SceneNode::handleEventChildren(const Event& event)
	{
		for(u_ptr& child : children)
			child->handleEvent(event);
	}

	void SceneNode::drawCurrent(SceneRenderer& renderer, Transform t) const
	{
		// Do nothing by default
	}

	void SceneNode::drawChildren(SceneRenderer& renderer, Transform t) const
	{
		for (const u_ptr& child : children)
			child->draw(renderer, t);
	}

} // end Dunjun
