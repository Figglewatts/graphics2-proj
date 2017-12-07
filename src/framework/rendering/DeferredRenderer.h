#ifndef DEFERREDRENDERER_H
#define DEFERREDRENDERER_H
#pragma once

#include "Renderer.h"
#include "GBuffer.h"
#include "framework/graphics/Shader.h"
#include "Lights.h"
#include "framework/graphics/Mesh.h"
#include "Renderable.h"

namespace Framework
{
	class DeferredRenderer : public Renderer
	{
	private:
		GBuffer *_gBuffer;
		unsigned _width, _height;
		Shader *_pointLightShader;
		Shader *_dirLightShader;
		Shader *_nullShader;
		DirectionalLight _dirLight;
		std::vector<PointLight> _pointLights;
		Mesh *_pointLightBoundingSphere;
		Mesh *_directionalLightBoundingQuad;

		void PointLightPass(const PointLight& light);
		float PointLightBoundingSphere(const PointLight& light) const;
		void DirectionalLightPass();
		void StencilPass(const PointLight& light);
		void FinalPass();
	public:
		DeferredRenderer(unsigned width, unsigned height, GLContext *context, Shader *pointLightShader, Shader *dirLightShader);
		void init() override;
		void resize(glm::ivec2 size) override;
		void beginFrame() override;
		void endFrame() override;
		~DeferredRenderer();

		void setDirLight(DirectionalLight light) { this->_dirLight = light; }
		std::vector<PointLight>& pointLights() { return _pointLights; }
	};
}

#endif