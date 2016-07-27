#ifndef DUNJUN_RENDERSYSTEM_HPP
#define DUNJUN_RENDERSYSTEM_HPP

#include <Dunjun/SceneGraph.hpp>

namespace Dunjun
{
	class RenderSystem
	{
	public:
		using ComponentId = u32;

		GLOBAL const ComponentId EmptyId = (ComponentId)(-1);

		struct InstanceData
		{
			u32 length = 0;
			u32 capacity = 0;
			void* buffer = nullptr;

			EntityId* entityId = nullptr;
			RenderComponent* component = nullptr;
		};

		Allocator& allocator;
		InstanceData data;
		HashMap<ComponentId> map;
		SceneGraph& sceneGraph;

		Vector2 fbSize;
		GBuffer gBuffer;
		RenderTexture lightingBuffer;
		RenderTexture finalTexture;

		Color ambientLightColor;
		f32 ambientLightIntensity;
		Array<DirectionalLight> directionalLights;
		Array<PointLight> pointLights;
		Array<SpotLight> spotLights;

		const Camera* camera;

		RenderSystem(Allocator& a, SceneGraph& sg);
		~RenderSystem();

		void allocate(u32 capacity);

		ComponentId addComponent(EntityId id, const RenderComponent& component);
		void removeComponent(ComponentId id);

		ComponentId getComponentId(EntityId id);
		bool isValid(ComponentId id) const;

		void resetAllPointers();

		void render();

		void deferredGeometryPass();
		void deferredLightPass();
		void deferredFinalPass();

	private:
		const Texture* currentTexture[32];

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;

		inline void renderAmbientLight();
		inline void renderDirectionalLights();
		inline void renderPointLights();
		inline void renderSpotLights();

		//inline bool isCurrentShaders(const ShaderProgram* shaders);
		//inline bool isCurrentTexture(const Texture* texture);

		//void setShaders(const ShaderProgram* shaders);
		bool setTexture(const Texture* texture, u32 position);

	};
} // end Dunjun

#endif
