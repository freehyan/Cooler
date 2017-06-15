#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include "../../Cooler/Cooler/Effect.h"

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
	void __inputPass();
	void __initSceneData();

	Cooler::CInputTransformer* m_pInputTransformer;
	GLuint m_VBO, m_VAO;
};