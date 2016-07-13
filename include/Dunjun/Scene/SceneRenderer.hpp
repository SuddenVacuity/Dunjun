#ifndef DUNJUN_RENDERER_HPP
#define DUNJUN_RENDERER_HPP

#include <Dunjun/Context.hpp>

namespace Dunjun
{
	class World;
	class SceneNode;
	//class Drawable;
	struct MeshRenderer;

	class SceneRenderer
	{
	public:

		struct ModelInstance
		{
			const MeshRenderer* asset = nullptr;
			Transform transform = Transform();

			//ModelInstance() = default;
			//ModelInstance(const MeshRenderer* asset, const Transform& transform)
			//	: asset(asset)
			//	, transform(transform)
			//{
			//}
		};

		World* world = nullptr;

		GBuffer gBuffer{};

		const Camera* camera = nullptr;

		RenderTexture lightingBuffer{};
		RenderTexture finalTexture{};

		const Texture* currentTexture = nullptr;
		const ShaderProgram* currentShaders = nullptr;
		const Material* currentMaterial = nullptr;

		Array<ModelInstance> modelInstances;

		SceneRenderer();
		~SceneRenderer();

		// clear all pointers
		SceneRenderer& reset();

		// clear double ended queues
		SceneRenderer& clearAll();

		SceneRenderer& addSceneGraph(const SceneNode& node, const Transform& t = Transform());
		void draw(const Mesh* mesh) const;

		void addModelInstance(const MeshRenderer& meshRenderer, Transform t);

		SceneRenderer& setFrameBufferSize(Vector2 size);

		void render();

		//inline void createGBuffer(u32 w, u32 h)
		//{
		//	if (m_gBuffer == nullptr)
		//		m_gBuffer = new GBuffer();
		//
		//	m_gBuffer->create(w, h);
		//}
		//
		//inline GBuffer* getGBuffer()
		//{
		//	if(m_gBuffer == nullptr)
		//		createGBuffer(Window::width, Window::height);
		//
		//	return m_gBuffer;
		//}

		SceneRenderer& deferredGeometryPass();
		SceneRenderer& deferredLightPass();
		SceneRenderer& deferredFinalPass();


		//void setUniforms(const Transform& t);

	private:
		void renderAmbientLight();
		void renderDirectionalLights();
		void renderPointLights();
		void renderSpotLights();

		inline bool isCurrentShaders(const ShaderProgram* shaders);
		inline bool isCurrentTexture(const Texture* texture);

		void setShaders(const ShaderProgram* shaders);
		void setTexture(const Texture* texture, u32 position);

	}; // end SceneRenderer
} // end Dunjun

#endif
