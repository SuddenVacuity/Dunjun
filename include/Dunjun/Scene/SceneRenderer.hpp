#ifndef DUNJUN_RENDERER_HPP
#define DUNJUN_RENDERER_HPP

#include <Dunjun/Scene/Lighting.hpp>

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
		void addPointLight(const PointLight* light);


		void renderAll();

		//void setMaterial(const Material* material);
		void setCamera(const Camera& camera);

		//void setUniforms(const Transform& t);

		const Camera* currentCamera = nullptr;

	private:
		const Material* m_currentMaterial = nullptr;
		const ShaderProgram* m_currentShaders = nullptr;
		const Texture* m_currentTexture = nullptr;

		// cache of instances
		mutable std::deque<ModelInstance> m_modelInstances;
		std::deque<const PointLight*> m_pointLights;

		bool isCurrentShaders(const ShaderProgram* shaders);
		bool isCurrentTexture(const Texture* texture);

		void setShaders(const ShaderProgram* shaders);
		void setTexture(const Texture* texture, GLuint position);

	};
} // end Dunjun

#endif
