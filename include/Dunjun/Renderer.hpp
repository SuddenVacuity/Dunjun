#ifndef DUNJUN_RENDERER_HPP
#define DUNJUN_RENDERER_HPP

#include <Dunjun/ModelAsset.hpp>

namespace Dunjun
{
	class SceneNode;
	class Drawable;

	class Renderer : private NonCopyable
	{
	public:
		Renderer();

		virtual ~Renderer();

		void reset();

		void draw(const Drawable& node, const Transform& t = Transform());
		void draw(const Mesh* mesh) const;

		void setMaterial(const Material* material);
		void setCamera(const Camera& camera);

		void setUniforms(const Transform& t);

	private:
		const Material* m_currentMaterial = nullptr;
		const ShaderProgram* m_currentShaders = nullptr;
		const Texture* m_currentTexture = nullptr;
		const Camera* m_currentCamera = nullptr;

		void setShaders(const ShaderProgram* shaders);
		void setTexture(const Texture* texture);
	};
} // end Dunjun

#endif
