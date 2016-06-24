#ifndef DUNJUN_RENDERER_HPP
#define DUNJUN_RENDERER_HPP

#include <Dunjun/Context.hpp>

namespace Dunjun
{
	class World;
	class SceneNode;
	//class Drawable;
	class MeshRenderer;

	class SceneRenderer : private NonCopyable
	{
	public:

		struct ModelInstance
		{
			ModelInstance() = default;
			ModelInstance(const MeshRenderer* asset, const Transform& transform)
				: asset(asset)
				, transform(transform)
			{
			}

			const MeshRenderer* asset;
			Transform transform;
		};

		SceneRenderer(World& world);

		virtual ~SceneRenderer();

		// clear all pointers
		SceneRenderer& reset();

		// clear double ended queues
		SceneRenderer& clearAll();

		SceneRenderer& addSceneGraph(const SceneNode& node, const Transform& t = Transform());
		void draw(const Mesh* mesh) const;

		void addModelInstance(const MeshRenderer& meshRenderer, Transform t);
		//void addDirectionalLight(const DirectionalLight* light);
		//void addPointLight(const PointLight* light);
		//void addSpotLight(const SpotLight* light);

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

		//void setMaterial(const Material* material);
		SceneRenderer& setCamera(const Camera& camera);
		const Camera& getCamera() const 
		{
			return *m_camera;
		}

		//void setUniforms(const Transform& t);

		const GBuffer& getGBuffer() const
		{
			return m_gBuffer;
		}

		//const Texture& getFinalTexture() const
		//{
		//}

		const Texture& getFinalTexture() const
		{
			return m_finalTexture.colorTexture;
		}

	private:
		const Texture* m_currentTexture = nullptr;
		const ShaderProgram* m_currentShaders = nullptr;
		const Material* m_currentMaterial = nullptr;

		const Camera* m_camera = nullptr;
		//Vector2 m_fbSize = Vector2(512, 512);

		void renderAmbientLight();
		void renderDirectionalLights();
		void renderPointLights();
		void renderSpotLights();

		// cache of instances
		std::deque<ModelInstance> m_modelInstances;

		//std::deque<const DirectionalLight*> m_directionalLights;
		//std::deque<const PointLight*> m_pointLights;
		//std::deque<const SpotLight*> m_spotLights;

		World& m_world;
		GBuffer m_gBuffer;

		RenderTexture m_lightingTexture;
		RenderTexture m_finalTexture;

		inline bool isCurrentShaders(const ShaderProgram* shaders);
		inline bool isCurrentTexture(const Texture* texture);

		void setShaders(const ShaderProgram* shaders);
		void setTexture(const Texture* texture, u32 position);

	}; // end SceneRenderer
} // end Dunjun

#endif
