#ifndef _Effect_H_
#define _Effect_H_
#include "Config/Head.h"
//
// Effect
//
class EffectSprite;
class Effect : public cocos2d::Ref
{
public:
	cocos2d::GLProgramState* getGLProgramState() const;
	virtual void setTarget(EffectSprite *sprite){}

protected:
	bool initGLProgramState(const std::string &fragmentFilename);
	Effect();
	virtual ~Effect();
	cocos2d::GLProgramState* _glprogramstate;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	std::string _fragSource;
	cocos2d::EventListenerCustom* _backgroundListener;
#endif
};
#endif //!_Effect_H_
//origin
class EffectOrigin : public Effect
{
public:
	CREATE_FUNC(EffectOrigin);

	bool init()
	{
		initGLProgramState("Shaders/example_Origin.fsh");
		return true;
	}

};

// Outline
class EffectOutline : public Effect
{
public:
	CREATE_FUNC(EffectOutline);

	bool init()
	{
		initGLProgramState("Shaders/example_Outline.fsh");

		Vec3 color(1.0f, 0.2f, 0.3f);
		GLfloat radius = 0.02f;
		GLfloat threshold = 2.0;

		_glprogramstate->setUniformVec3("u_outlineColor", color);
		_glprogramstate->setUniformFloat("u_radius", radius);
		_glprogramstate->setUniformFloat("u_threshold", threshold);
		return true;
	}
};

// water
class EffectWater : public Effect
{
public:
	CREATE_FUNC(EffectWater);

	bool init()
	{
		initGLProgramState("WaterShader/Shader_Water.fsh");

		auto TexCache = Director::getInstance()->getTextureCache();
		auto wave2 = TexCache->addImage("WaterShader/wave1.png");
		auto wave1 = TexCache->addImage("WaterShader/18.jpg");
		wave1->setTexParameters(Texture2D::TexParams{ GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT });
		wave2->setTexParameters(Texture2D::TexParams{ GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT });


		_glprogramstate->setUniformTexture("u_wave1", wave1);
		_glprogramstate->setUniformTexture("u_wave2", wave2);
		_glprogramstate->setUniformFloat("saturateValue", 1.2);
		return true;
	}
};