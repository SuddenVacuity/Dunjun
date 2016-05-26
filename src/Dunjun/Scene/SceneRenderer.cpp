
#include <Dunjun/Scene/SceneRenderer.hpp>

#include <Dunjun/Scene/MeshRenderer.hpp>

namespace Dunjun
{
	SceneRenderer::SceneRenderer()
	{
	}

	SceneRenderer::~SceneRenderer()
	{
	}

	void SceneRenderer::reset()
	{
		if (m_currentShaders)
			m_currentShaders->stopUsing();

		m_currentShaders = nullptr;
		m_currentTexture = nullptr;
		Texture::bind(nullptr, 0);
		m_currentMaterial = nullptr; // commenting this out makes the top right corner of the window white
		currentCamera = nullptr;
		m_modelInstances.clear();
		m_pointLights.clear();
	}

	void SceneRenderer::draw(const SceneNode& node, const Transform& t)
	{
		node.draw(*this, t);
	}

	void SceneRenderer::draw(const Mesh* mesh) const
	{
		if(mesh)
			mesh->draw();
	}
	
	void SceneRenderer::addModelInstance(const MeshRenderer& meshRenderer, Transform t)
	{
		if(meshRenderer.parent->visible == true)
			m_modelInstances.push_back({&meshRenderer, t});
	}

	void SceneRenderer::addPointLight(const PointLight* light)
	{
		m_pointLights.push_back(light);
	}


	void SceneRenderer::renderAll()
	{
		std::sort(std::begin(m_modelInstances), std::end(m_modelInstances),
			[](const ModelInstance& a, const ModelInstance& b) -> bool
		{
			const auto& A = a.asset->material;
			const auto& B = b.asset->material;

			// if same shaders sort by texture else sort by shader
			if(A->shaders == B->shaders)
				return A->texture < B->texture;
			else
				return A->shaders < B->shaders;
		});


		// FIXME: out of range happens somewhere in here when level::grid is large and initialized as true
		// somehow related to levelSizeY in Level::placeRooms() being greater than 2
		for(const auto& inst : m_modelInstances)
		{
			if(!m_currentShaders)
				setShaders(inst.asset->material->shaders);

			if (isCurrentShaders(inst.asset->material->shaders))
			{
				m_currentShaders->setUniform("u_camera", currentCamera->getMatrix()); // shaderprogram.cpp
				m_currentShaders->setUniform("u_tex", (Dunjun::u32)0); // shaderprogram.cpp
				m_currentShaders->setUniform("u_light.position", m_pointLights[0]->position); // shaderprogram.cpp
				m_currentShaders->setUniform("u_light.intensities", m_pointLights[0]->intensities); // shaderprogram.cpp
			}

			setTexture(inst.asset->material->texture);

			m_currentShaders->setUniform("u_transform", inst.transform); // shaderprogram.cpp

			draw(inst.asset->mesh);
		}
	}

	//void SceneRenderer::setMaterial(const Material* material)
	//{
	//	if(material != m_currentMaterial)
	//	{
	//		m_currentMaterial = material;
	//
	//		setShaders(m_currentMaterial->shaders);
	//		setTexture(m_currentMaterial->texture);
	//	}
	//}

	bool SceneRenderer::isCurrentShaders(const ShaderProgram* shaders)
	{
		if(shaders == m_currentShaders)
		{
			return true;
		}

		return false;
	}

	bool SceneRenderer::isCurrentTexture(const Texture* texture)
	{
		if (texture == m_currentTexture)
		{
			return true;
		}
		return false;
	}

	void SceneRenderer::setShaders(const ShaderProgram* shaders)
	{
		//if(shaders != m_currentShaders && m_currentShaders)
		//	m_currentShaders->stopUsing();
		if (shaders != m_currentShaders)
		{
			if (m_currentShaders)
				m_currentShaders->stopUsing();

			m_currentShaders = shaders;
			m_currentShaders->use();

		}
	}

	void SceneRenderer::setTexture(const Texture* texture)
	{
		if (texture != m_currentTexture)
		{
			m_currentTexture = texture;
			Texture::bind(m_currentTexture, 0);
		}
	}

	//void SceneRenderer::setCamera(const Camera& camera)
	//{
	//	m_currentCamera = &camera;
	//}
	//
	//void SceneRenderer::setUniforms(const Transform& t)
	//{
	//	if(!m_currentShaders)
	//		return;
	//	else if(!m_currentShaders->isInUse())
	//		m_currentShaders->use();
	//
	//	m_currentShaders->setUniform("u_camera", m_currentCamera->getMatrix()); // shaderprogram.cpp
	//	m_currentShaders->setUniform("u_transform", t); // shaderprogram.cpp
	//	m_currentShaders->setUniform("u_tex", (Dunjun::u32)0); // shaderprogram.cpp
	//}
} // end Dunjun
