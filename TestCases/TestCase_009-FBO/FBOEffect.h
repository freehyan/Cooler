#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include "../../Cooler/Cooler/Effect.h"
#include "DeferredFBO.h"

namespace Cooler
{
	CInputTransformer;
}

class CInputEffect : public Cooler::CEffect
{
public:
	CInputEffect();
	virtual ~CInputEffect();

protected:
	virtual void _initEffectV() override;
	virtual void _renderEffectV() override;
	virtual void _destoryEffectV() override;

private:
	void __geometryPass();
	void __showTexturePass();
	void __renderScreenSizeQuad();

	void __initSceneData();

	Cooler::CInputTransformer* m_pInputTransformer;
	CDeferredFBO* m_pDeferredFBO;
	GLuint m_VBO, m_VAO;
};