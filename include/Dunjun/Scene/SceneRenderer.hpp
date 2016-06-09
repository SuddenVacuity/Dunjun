#ifndef DUNJUN_RENDERER_HPP
#define DUNJUN_RENDERER_HPP

#include <Dunjun/ResourceHolder_objects.hpp>

namespace Dunjun
{
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

		SceneRenderer();

		virtual ~SceneRenderer();

		// clear all pointers
		void reset();

		// clear double ended queues
		void clearAll();

		void addSceneGraph(const SceneNode& node, const Transform& t = Transform());
		void draw(const Mesh* mesh) const;

		void addModelInstance(const MeshRenderer& meshRenderer, Transform t);
		void addDirectionalLight(const DirectionalLight* light);
		void addPointLight(const PointLight* light);
		void addSpotLight(const SpotLight* light);

		//void renderAll();

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

		void deferredGeometryPass();
		void deferredLightPass();
		void deferredFinalPass();

		//void setMaterial(const Material* material);
		void setCamera(const Camera& camera);

		//void setUniforms(const Transform& t);

		GBuffer gBuffer;

		const Camera* camera = nullptr;

		RenderTexture lightingTexture;
		RenderTexture finalTexture;

	private:
		const Material* m_currentMaterial = nullptr;
		const ShaderProgram* m_currentShaders = nullptr;

		const Texture* m_currentTexture = nullptr;

		void renderAmbientLight();
		void renderDirectionalLights();
		void renderPointLights();
		void renderSpotLights();

		// cache of instances
		std::deque<ModelInstance> m_modelInstances;

		std::deque<const DirectionalLight*> m_directionalLights;
		std::deque<const PointLight*> m_pointLights;
		std::deque<const SpotLight*> m_spotLights;

		bool isCurrentShaders(const ShaderProgram* shaders);
		bool isCurrentTexture(const Texture* texture);

		void setShaders(const ShaderProgram* shaders);
		void setTexture(const Texture* texture, u32 position);

	};
} // end Dunjun

#endif
