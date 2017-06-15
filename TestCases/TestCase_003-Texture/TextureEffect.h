#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include "../../Cooler/Cooler/Effect.h"

class CTextureEffect : public Cooler::CEffect
{
public:
	CTextureEffect();
	virtual ~CTextureEffect();

protected:
	virtual void _initEffectV() override;
	virtual void _renderEffectV() override;
	virtual void _destoryEffectV() override;

private:
	void __triagnlePass();
	void __initSceneData();

	GLuint m_VBO, m_VAO, m_EBO;
	GLuint m_TextureId;
};