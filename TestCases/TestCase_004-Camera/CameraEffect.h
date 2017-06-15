#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include "../../Cooler/Cooler/Effect.h"

class CCameraEffect : public Cooler::CEffect
{
public:
	CCameraEffect();
	virtual ~CCameraEffect();

protected:
	virtual void _initEffectV() override;
	virtual void _renderEffectV() override;
	virtual void _destoryEffectV() override;

private:
	void __triagnlePass();
	void __initSceneData();

	GLuint m_VBO, m_VAO;
	GLuint m_TextureId;
};