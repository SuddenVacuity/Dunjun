
#include <Dunjun/SceneGraph.hpp>

namespace Dunjun
{

	SceneGraph::SceneGraph(Allocator& a)
		: allocator(a)
		, data()
		, map(allocator)
	{
		allocate(16);
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
		newData.global		= (Transform*)(newData.local	+ capacity);
		newData.parent		= (NodeId*)(newData.global		+ capacity);
		newData.firstChild  = (NodeId*)(newData.parent		+ capacity);
		newData.prevSibling = (NodeId*)(newData.firstChild	+ capacity);
		newData.nextSibling = (NodeId*)(newData.prevSibling + capacity);

		// copy data from old location to new location
		memcpy(newData.entityId,	data.entityId,	  data.length * sizeof(EntityId));
		memcpy(newData.local,		data.local,		  data.length * sizeof(Transform));
		memcpy(newData.global,		data.global,	  data.length * sizeof(Transform));
		memcpy(newData.parent,		data.parent,	  data.length * sizeof(NodeId));
		memcpy(newData.firstChild,	data.firstChild,  data.length * sizeof(NodeId));
		memcpy(newData.prevSibling, data.prevSibling, data.length * sizeof(NodeId));
		memcpy(newData.nextSibling, data.nextSibling, data.length * sizeof(NodeId));

		allocator.deallocate(data.buffer);

		// change pointer to point to new data
		data = newData;
	}

	SceneGraph::NodeId SceneGraph::addNode(EntityId id, const Transform& t)
	{
		if(data.capacity == data.length || data.capacity == 0)
			allocate(2 * data.length + 1);

		// get last position in data arrays
		const NodeId last = data.length;

		// assing data to next open position
		data.entityId[last] = id;
		data.local[last] = t;
		data.global[last] = t;

		data.parent[last] = EmptyNode;
		data.firstChild[last] = EmptyNode;
		data.prevSibling[last] = EmptyNode;
		data.nextSibling[last] = EmptyNode;

		// add data to hash map
		set(map, id, last);

		data.length++;

		return last;
	}

	void SceneGraph::removeNode(NodeId id)
	{
		const NodeId last = data.length - 1;
		const EntityId entity = data.entityId[id];
		const EntityId lastEntity = data.entityId[last];

		data.entityId[id]	 = data.entityId[last];
		data.local[id]		 = data.local[last];
		data.global[id]		 = data.global[last];
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

	void SceneGraph::linkNodes(NodeId parent, NodeId child)
	{
		// unlink any previous parent
		unlinkNode(child);

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

			// cycle though any existing children
			while(isValid(current))
			{
				// and linked child to the end of the line
				prev = current;
				current = data.nextSibling[current];
			}

			// add new child to last childs pointer
			data.nextSibling[prev] = child;

			// set pointers for new child
			data.firstChild[child] = EmptyNode;
			data.nextSibling[child] = EmptyNode;
			data.prevSibling[child] = prev;
		}
		//
		// start update child transform to account for parent transform
		//

		const Transform parentTransform = data.global[parent];
		const Transform childTransform = data.global[child];

		const Transform localTransform = parentTransform / childTransform;

		data.local[child] = localTransform;
		data.parent[child] = parent;

		transformChild(child, parentTransform);
	}

	void SceneGraph::unlinkNode(NodeId child)
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

	void SceneGraph::transformChild(NodeId id, const Transform& parentTransform)
	{
		data.global[id] = parentTransform * data.local[id];
		NodeId child = data.firstChild[id];

		while(isValid(child))
		{
			transformChild(child, data.global[id]);
			child = data.nextSibling[child];
		}
	}

	void SceneGraph::updateLocal(NodeId id)
	{
		NodeId parent = data.parent[id];
		Transform parentTransform = Transform::Identity;

		if (isValid(parent))
			parentTransform = data.global[parent];

		transformChild(id, parentTransform);
	}

	void SceneGraph::updateGlobal(NodeId id)
	{
		NodeId parent = data.parent[id];
		Transform parentTransform = Transform::Identity;

		if (isValid(parent))
			parentTransform = data.global[parent];

		data.local[id] = parentTransform / data.global[id];

		transformChild(id, parentTransform);
	}


	Transform SceneGraph::getLocalTransform(NodeId id) const
	{
		return data.local[id];
	}

	Transform SceneGraph::getGlobalTransform(NodeId id) const
	{
		return data.global[id];
	}


	void SceneGraph::setLocalTransform(NodeId id, const Transform& t)
	{
		// TODO: find out why this is needed
		// changing data.local anywhere else causes problems
		Transform transform = t;
		//transform.orientation = conjugate(Quaternion::Identity);

		data.local[id] = transform;
		updateLocal(id);
	}

	void SceneGraph::setGlobalTransform(NodeId id, const Transform& t)
	{
		// TODO: find out why this is needed
		// scale is ending up with negative values
		Transform transform = t;
		//transform.orientation = conjugate(Quaternion::Identity);

		data.global[id] = transform;
		updateGlobal(id);
	}

} // end Dunjun
