#include "DeferredRenderer.h"
#include "framework/io/ResourceManager.h"
#include <glm/gtc/matrix_transform.inl>

namespace Framework
{
	DeferredRenderer::DeferredRenderer(unsigned width, unsigned height, GLContext *context, Shader *pointLightShader, Shader *dirLightShader)
		: Renderer(context), _width(width), _height(height), _pointLightShader(pointLightShader), _dirLightShader(dirLightShader)
	{
		_gBuffer = new GBuffer(width, height);
		_pointLightBoundingSphere = ResourceManager::Load<Mesh>("assets/models/sphere.obj");
		_pointLightShader = ResourceManager::Load<Shader>("assets/shaders/pointLightPass");
		_dirLightShader = ResourceManager::Load<Shader>("assets/shaders/dirLightPass");
		_nullShader = ResourceManager::Load<Shader>("assets/shaders/null");

		_directionalLightBoundingQuad = new Mesh(
			{
				Vertex({ -1, -1, 0 },{ 0, 0, 0 },{ 0, 0 },{ 1, 1, 1, 1 }),
				Vertex({ -1, 1, 0 },{ 0, 0, 0 },{ 0, 1 },{ 1, 1, 1, 1 }),
				Vertex({ 1, 1, 0 },{ 0, 0, 0 },{ 1, 1 },{ 1, 1, 1, 1 }),
				Vertex({ 1, -1, 0 },{ 0, 0, 0 },{ 1, 0 },{ 1, 1, 1, 1 })

			},
			{
				1, 0, 2, 0, 3, 2
			}
		);
	}

	void DeferredRenderer::init()
	{
		
	}

	void DeferredRenderer::beginFrame()
	{
		_gBuffer->StartFrame();
		_gBuffer->BindForGeomPass();
		glDepthMask(GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}

	void DeferredRenderer::endFrame()
	{
		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);

		for (const auto& pointLight : _pointLights)
		{
			StencilPass(pointLight);
			PointLightPass(pointLight);
		}

		DirectionalLightPass();

		FinalPass();

		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glm::ivec2 size = this->_gBuffer->get_framebuffer()->size();
		glBlitFramebuffer(0, 0, size.x, size.y, 0, 0, size.x, size.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	}

	void DeferredRenderer::resize(glm::ivec2 size)
	{
		this->_gBuffer->get_framebuffer()->resize(size);
	}

	DeferredRenderer::~DeferredRenderer()
	{
		delete _gBuffer;
		delete _directionalLightBoundingQuad;
	}

	void DeferredRenderer::PointLightPass(const PointLight* light)
	{
		_gBuffer->BindForLightPass();
		
		_pointLightShader->bind();
		_pointLightShader->setUniform("ViewMatrix", this->_pCamera->view(), false);
		_pointLightShader->setUniform("ProjectionMatrix", this->_projection, false);
		_pointLightShader->setUniform("gScreenSize", _gBuffer->get_framebuffer()->size());
		_pointLightShader->setUniform("gPosition", 0);
		_pointLightShader->setUniform("gDiffuse", 1);
		_pointLightShader->setUniform("gNormal", 2);

		// set up the stencil test to pass when not equal to zero
		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		_pointLightShader->setUniform("light.position", light->position);
		_pointLightShader->setUniform("light.diffuse", light->diffuse);
		_pointLightShader->setUniform("light.intensity", light->intensity);
		_pointLightShader->setUniform("light.linearAttenuation", light->linearAttenuation);
		_pointLightShader->setUniform("light.expAttenuation", light->expAttenuation);
		const float boundingSphereScale = PointLightBoundingSphere(light);
		glm::mat4 model = glm::translate(glm::mat4(1), light->position);
		model = glm::scale(model, glm::vec3(boundingSphereScale));
		_pointLightShader->setUniform("ModelMatrix", model, false);
		_pointLightBoundingSphere->render();
		_pointLightShader->unbind();

		glCullFace(GL_BACK);
		glDisable(GL_BLEND);
	}

	// from: http://ogldev.atspace.co.uk/www/tutorial36/tutorial36.html
	float DeferredRenderer::PointLightBoundingSphere(const PointLight* light) const
	{
		//float MaxChannel = fmax(fmax(light.diffuse.get_r(), light.diffuse.get_g()), light.diffuse.get_b());

		/*float ret = (-light.linearAttenuation + glm::sqrt(light.linearAttenuation * light.linearAttenuation -
			4 * light.expAttenuation * (light.expAttenuation - 256 * MaxChannel * 1)))
			/
			(2 * light.expAttenuation);
		return ret;*/
		return glm::sqrt(63.f / light->linearAttenuation);
	}

	void DeferredRenderer::DirectionalLightPass()
	{
		_gBuffer->BindForLightPass();
		
		_dirLightShader->bind();
		_dirLightShader->setUniform("ViewMatrix", glm::mat4(1), false);
		_dirLightShader->setUniform("ProjectionMatrix", glm::mat4(1), false);
		_dirLightShader->setUniform("ModelMatrix", glm::mat4(1), false);
		_dirLightShader->setUniform("gScreenSize", _gBuffer->get_framebuffer()->size());
		_dirLightShader->setUniform("gPosition", 0);
		_dirLightShader->setUniform("gDiffuse", 1);
		_dirLightShader->setUniform("gNormal", 2);
		_dirLightShader->setUniform("light.direction", _dirLight.direction);
		_dirLightShader->setUniform("light.diffuse", _dirLight.diffuse);
		_dirLightShader->setUniform("light.ambient", _dirLight.ambient);
		_dirLightShader->setUniform("light.intensity", _dirLight.intensity);
		_dirLightShader->setUniform("ViewPos", _pCamera->get_position());

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		_directionalLightBoundingQuad->render();
		_dirLightShader->unbind();

		glDisable(GL_BLEND);
	}

	void DeferredRenderer::StencilPass(const PointLight* light)
	{
		_nullShader->bind();

		_gBuffer->BindForStencilPass();
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glClear(GL_STENCIL_BUFFER_BIT);

		// need stencil test enabled but we need it to
		// always succeed -- we only need depth test
		glStencilFunc(GL_ALWAYS, 0, 0);

		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

		// render light bounding sphere to stencil buffer
		_nullShader->setUniform("ViewMatrix", this->_pCamera->view(), false);
		_nullShader->setUniform("ProjectionMatrix", this->_projection, false);
		const float boundingSphereScale = PointLightBoundingSphere(light);
		glm::mat4 model = glm::translate(glm::mat4(1), light->position);
		model = glm::scale(model, glm::vec3(boundingSphereScale));
		_nullShader->setUniform("ModelMatrix", model, false);
		_pointLightBoundingSphere->render();
		_nullShader->unbind();
	}

	void DeferredRenderer::FinalPass()
	{
		_gBuffer->BindForFinalPass();
		glm::ivec2 fboSize = _gBuffer->get_framebuffer()->size();
		glBlitFramebuffer(0, 0, fboSize.x, fboSize.y,
			0, 0, fboSize.x, fboSize.y, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	}

}
