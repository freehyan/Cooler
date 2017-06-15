#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include "../../Cooler/Cooler/Effect.h"

class CShaderEffect : public Cooler::CEffect
{
public:
	CShaderEffect();
	virtual ~CShaderEffect();

protected:
	virtual void _initEffectV() override;
	virtual void _renderEffectV() override;

private:
	void __triagnlePass();

	GLuint g_VBO, g_VAO;
};