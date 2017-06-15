#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include "../../Cooler/Cooler/Effect.h"

class CModelEffect : public Cooler::CEffect
{
public:
	CModelEffect();
	virtual ~CModelEffect();

protected:
	virtual void _initEffectV() override;
	virtual void _renderEffectV() override;
	virtual void _destoryEffectV() override;

private:
	void __ModelPass();

	GLuint m_VBO, m_VAO;
	GLuint m_TextureId;
};