
#include <Dunjun/SceneGraph.hpp>
#include <Dunjun/System/Containers/HashMap.hpp>

namespace Dunjun
{

	SceneGraph::SceneGraph(Allocator& a)
		: allocator(a)
		, data()
		, map(allocator)
	{

	}

	SceneGraph::~SceneGraph()
	{
		allocator.deallocate(data.buffer);
	}

	void SceneGraph::allocate(size_t capacity)
	{
		if(capacity <= data.length)
			return;

		// add up all the data in void* buffer
		const size_t bytes = capacity * (sizeof(EntityId) +
										 sizeof(Transform) * 2 +
										 sizeof(NodeId) * 4);

		// make a copy of data that's the new capacity
		InstanceData newData;
		newData.length = data.length;
		newData.capacity = capacity;
		newData.buffer = allocator.allocate(bytes);

		// assign pointers to buffer data accunting for extra capacity
		newData.entityId	= (EntityId*)(newData.buffer);
		newData.local		= (Transform*)(newData.entityId	+ capacity);
		newData.world		= (Transform*)(newData.local	+ capacity);
		newData.parent		= (NodeId*)(newData.world		+ capacity);
		newData.firstChild  = (NodeId*)(newData.parent		+ capacity);
		newData.prevSibling = (NodeId*)(newData.firstChild	+ capacity);
		newData.nextSibling = (NodeId*)(newData.prevSibling + capacity);

		// copy data from old location to new location
		memcpy(newData.entityId,	data.entityId,	  data.length * sizeof(EntityId));
		memcpy(newData.local,		data.local,		  data.length * sizeof(Transform));
		memcpy(newData.world,		data.world,		  data.length * sizeof(Transform));
		memcpy(newData.parent,		data.parent,	  data.length * sizeof(NodeId));
		memcpy(newData.firstChild,	data.firstChild,  data.length * sizeof(NodeId));
		memcpy(newData.prevSibling, data.prevSibling, data.length * sizeof(NodeId));
		memcpy(newData.nextSibling, data.nextSibling, data.length * sizeof(NodeId));

		allocator.deallocate(data.buffer);

		// change pointer to point to new data
		data = newData;
	}

	SceneGraph::NodeId SceneGraph::create(EntityId id, const Transform& t)
	{
		if(data.capacity == data.length)
			allocate(2 * data.length + 1);

		// get last position in data arrays
		const NodeId last = data.length;

		// assing data to next open position
		data.entityId[last] = id;
		data.local[last] = t;
		data.world[last] = t;

		data.parent[last] = EmptyNode;
		data.firstChild[last] = EmptyNode;
		data.prevSibling[last] = EmptyNode;
		data.nextSibling[last] = EmptyNode;

		// add data to hash map
		set(map, id, last);

		data.length++;

		return last;
	}

	void SceneGraph::destroy(NodeId id)
	{
		const NodeId last = data.length - 1;
		const EntityId entity = data.entityId[id];
		const EntityId lastEntity = data.entityId[last];

		data.entityId[id]	 = data.entityId[last];
		data.local[id]		 = data.local[last];
		data.world[id]		 = data.world[last];
		data.parent[id]		 = data.parent[last];
		data.firstChild[id]	 = data.firstChild[last];
		data.prevSibling[id] = data.prevSibling[last];
		data.nextSibling[id] = data.nextSibling[last];


		set(map, lastEntity, id);
		set(map, entity, EmptyNode);

		data.length--;
	}

	SceneGraph::NodeId SceneGraph::getNodeId(EntityId id) const
	{
		return get(map, id, EmptyNode);
	}

	bool SceneGraph::isValid(NodeId id) const
	{
		return id != EmptyNode;
	}

	u32 SceneGraph::nodeCount() const
	{
		return data.length;
	}

	void SceneGraph::link(NodeId parent, NodeId child)
	{
		unlink(child);

		// check if node is empty
		if(!isValid(data.firstChild[parent]))
		{
			// if node is empty assign parent and child
			data.firstChild[parent] = child;
			data.parent[child] = parent;
		}
		else
		{
			// if node is not empty
			NodeId prev = EmptyNode;
			NodeId current = data.firstChild[parent];

			while(isValid(current))
			{
				prev = current;
				current = data.nextSibling[current];
			}

			data.nextSibling[prev] = child;

			data.firstChild[child] = EmptyNode;
			data.nextSibling[child] = EmptyNode;
			data.prevSibling[child] = prev;
		}

		const Transform parentTransform = data.world[parent];
		const Transform childTransform = data.world[child];

		const Transform localTransform = parentTransform / childTransform;

		data.local[child] = localTransform;
		data.parent[child] = parent;

		transformChild(child, parentTransform);


	}

	void SceneGraph::unlink(NodeId child)
	{
		if(!isValid(data.parent[child]))
			return;

		if(!isValid(data.prevSibling[child]))
			data.firstChild[data.parent[child]] = data.nextSibling[child];
		else
			data.nextSibling[data.prevSibling[child]] = data.nextSibling[child];

		if(isValid(data.nextSibling[child]))
			data.prevSibling[data.nextSibling[child]] = data.prevSibling[child];

		data.parent[child] = EmptyNode;
		data.nextSibling[child] = EmptyNode;
		data.prevSibling[child] = EmptyNode;
	}

	void SceneGraph::transformChild(NodeId id, const Transform& t)
	{
		data.world[id] = data.local[id] * t;
		NodeId child = data.firstChild[id];

		while(isValid(child))
		{
			transformChild(child, data.world[id]);
			child = data.nextSibling[child];
		}
	}

	void SceneGraph::updateLocal(NodeId id)
	{
		NodeId parent = data.parent[id];
		Transform parentTransform = Transform::Identity;

		if (isValid(parent))
			parentTransform = data.world[parent];

		transformChild(id, parentTransform);
	}

	void SceneGraph::updateWorld(NodeId id)
	{
		NodeId parent = data.parent[id];
		Transform parentTransform = Transform::Identity;

		if (isValid(parent))
			parentTransform = data.world[parent];

		data.local[id] = data.world[id] / parentTransform;

		transformChild(id, parentTransform);
	}


	Transform SceneGraph::getLocalTransform(NodeId id) const
	{
		return data.local[id];
	}

	Transform SceneGraph::getWorldTransform(NodeId id) const
	{
		return data.world[id];
	}


	void SceneGraph::setLocalTransform(NodeId id, const Transform& t)
	{
		data.local[id] = t;
		updateLocal(id);
	}

	void SceneGraph::setWorldTransform(NodeId id, const Transform& t)
	{
		data.world[id] = t;
		updateWorld(id);
	}

} // end Dunjun
