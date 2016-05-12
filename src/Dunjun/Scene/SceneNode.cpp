
#include <Dunjun/Scene/SceneNode.hpp>

namespace Dunjun
{
	SceneNode::SceneNode()
		: m_children()
		, name("DEFAULT SCENENODE")
		, transform()
		, parent(nullptr)
	{
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

	SceneNode* SceneNode::findChildByName(const std::string& name) const
	{
		for(const u_ptr& child : m_children)
		{
			if(child->name == name)
				return child.get();
		}

		return nullptr;
	}

	Transform SceneNode::getGlobaTransform() const
	{
		Transform result;

		for(SceneNode* p = parent; p != nullptr; p->parent)
			result *=  p->transform;

		return result;
	}

	void SceneNode::onStart()
	{
		onStartCurrent();
		onStartChildren();

		// check node components
		for(auto& group : m_groupedComponents)
		{
			for(auto& component : group.second)
			{
				component->onStart();
			}
		}
	}

	void SceneNode::update(f32 dt)
	{
		updateCurrent(dt);
		updateChildren(dt);

		for (auto& group : m_groupedComponents)
		{
			for (auto& component : group.second)
			{
				component->update(dt);
			}
		}
	}

	void SceneNode::draw(Transform t)
	{
		t *= this->transform;

		drawCurrent(t);
		drawChildren(t);

		for (auto& group : m_groupedComponents)
		{
			for (auto& component : group.second)
			{
				component->draw(t);
			}
		}
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

	SceneNode* SceneNode::addComponent(NodeComponent* component)
	{
		component->parent = this;

		const std::type_index id(typeid(*component));

		m_groupedComponents[id].push_back(component);

		return this;
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

	void SceneNode::onStartCurrent()
	{
		// Do nothing by default
	}

	void SceneNode::onStartChildren()
	{
		for(u_ptr& child : m_children)
			child->onStart();
	}

	void SceneNode::updateCurrent(f32 dt)
	{
		// Do nothing by default
	}

	void SceneNode::updateChildren(f32 dt)
	{
		for (u_ptr& child : m_children)
			child->update(dt);
	}

	void SceneNode::drawCurrent(Transform t)
	{
		// Do nothing by default
	}

	void SceneNode::drawChildren(Transform t)
	{
		for (u_ptr& child : m_children)
			child->draw(t);
	}

} // end Dunjun
