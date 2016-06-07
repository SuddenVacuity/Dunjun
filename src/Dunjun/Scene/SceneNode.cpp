
#include <Dunjun/Scene/SceneNode.hpp>

namespace Dunjun
{
	namespace Impl
	{
		//GLOBAL size_t idCount = 0;
		inline SceneNode::ID getUniqueSceneNodeId()
		{
			LOCAL_PERSIST SceneNode::ID lastId = 0;
			return lastId++;
		}
	} // end Impl namespace

	// TODO idCount-- when destrucitng
	SceneNode::SceneNode()
		: m_children()
		, id(Impl::getUniqueSceneNodeId())
		, name("")
		, transform()
		, parent(nullptr)
		, visible(true)
	{
		std::stringstream ss;
		ss << "node_" << id;
		name = ss.str();
	}

	SceneNode& SceneNode::attachChild(u_ptr child)
	{
		// get the 
		child->parent = this;
		m_children.push_back(std::move(child));

		return *this;
	}

	SceneNode::u_ptr SceneNode::detachChild(const SceneNode& node)
	{
		// check through all children to find if the one to detach exists
		auto found = std::find_if(m_children.begin(), m_children.end(), 
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
		if(found != m_children.end())
		{
			u_ptr result = std::move(*found);

			result->parent = nullptr; // remove parent pointer form result
			m_children.erase(found); // remove child pointer from m_children

			return result;
		}

		return nullptr;
	}

	// TODO: return the id of all with the same name
	SceneNode* SceneNode::findChildByName(const std::string& name) const
	{
		for(const u_ptr& child : m_children)
		{
			if(child->name == name)
				return child.get();
		}

		return nullptr;
	}

	SceneNode* SceneNode::findChildById(size_t id) const
	{
		for (const u_ptr& child : m_children)
		{
			if (child->id == id)
				return child.get();
		}

		return nullptr;
	}

	Transform SceneNode::getGlobalTransform() const
	{
		Transform result;

		for(const SceneNode* node = this; node != nullptr; node = node->parent)
			result *= node->transform;

		return result;
	}

	void SceneNode::onStart()
	{
		onStartCurrent();
		onStartChildren();

		// check node components
		for(auto& component : m_components)
		{
			component->onStart();
		}
	}

	void SceneNode::update(Time dt)
	{
		updateCurrent(dt);
		updateChildren(dt);

		for (auto& component : m_components)
		{
			component->update(dt);
		}
	}

	void SceneNode::draw(SceneRenderer& renderer, Transform t) const
	{
		if(!visible)
			return;

		t *= this->transform;

		drawCurrent(renderer, t);
		drawChildren(renderer, t);

		for (auto& component : m_components)
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

	void SceneNode::onStartCurrent()
	{
		// Do nothing by default
	}

	void SceneNode::onStartChildren()
	{
		for(u_ptr& child : m_children)
			child->onStart();
	}

	void SceneNode::updateCurrent(Time dt)
	{
		// Do nothing by default
	}

	void SceneNode::updateChildren(Time dt)
	{
		for (u_ptr& child : m_children)
			child->update(dt);
	}

	void SceneNode::drawCurrent(SceneRenderer& renderer, Transform t) const
	{
		// Do nothing by default
	}

	void SceneNode::drawChildren(SceneRenderer& renderer, Transform t) const
	{
		for (const u_ptr& child : m_children)
			child->draw(renderer, t);
	}

} // end Dunjun
