#ifndef DUNJUN_SCENEGRAPH_HPP
#define DUNJUN_SCENEGRAPH_HPP

#include <Dunjun/Scene/Lighting.hpp>


namespace Dunjun
{
	struct SceneGraph
	{
		using NodeId = u32;
		GLOBAL const NodeId EmptyNode = (NodeId)(-1);

		struct InstanceData
		{
			u32 length = 0;
			u32 capacity = 0;

			// points to beginning of data below
			void* buffer = nullptr;

			// data contained in void* buffer
			EntityId* entityId  = nullptr;
			Transform* local	= nullptr;
			Transform* global	= nullptr;
			NodeId* parent		= nullptr;
			NodeId* firstChild  = nullptr;
			NodeId* prevSibling = nullptr;
			NodeId* nextSibling = nullptr;

		}; // end InstanceData

		Allocator& allocator;
		InstanceData data;
		HashMap<NodeId> map;
		
		SceneGraph(Allocator& a);
		~SceneGraph();

		SceneGraph(const SceneGraph&) = delete;
		SceneGraph& operator=(const SceneGraph&) = delete;

		void allocate(size_t capacity);

		NodeId addNode(EntityId id, const Transform& t);
		void removeNode(NodeId id);

		NodeId getNodeId(EntityId id) const;

		bool isValid(NodeId id) const;

		u32 nodeCount() const;

		void linkNodes(NodeId parent, NodeId child);
		void unlinkNode(NodeId child);

		void transformChild(NodeId child, const Transform& parentTransform);

		void updateLocal(NodeId id);
		void updateGlobal(NodeId id);

		Transform getLocalTransform(NodeId id) const;
		Transform getGlobalTransform(NodeId id) const;

		// relative to current transform
		void setLocalTransform(NodeId id, const Transform& t);
		// absolute transform
		void setGlobalTransform(NodeId id, const Transform& t);


	}; // end SceneGraph
} // end Dunjun

#endif
