#ifndef  __GLTesting_H_
#define  __GLTesting_H_

#include "cocos2d.h"

USING_NS_CC;

class GLTestingLayer : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void menuCloseCallback(cocos2d::Ref* pSender);
	CREATE_FUNC(GLTestingLayer);
	void graySprite(Sprite * sprite);
	virtual void visit(Renderer* renderer, const Mat4 &parentTransform, bool parentTransformUpdated);
	void onDraw();
private:
	CustomCommand _command;
};



class ShaderNode : public cocos2d::Node
{
public:
	CREATE_FUNC(ShaderNode);
	static ShaderNode* shaderNodeWithVertex(const std::string &vert, const std::string &frag);

	virtual void update(float dt) override;
	virtual void setPosition(const cocos2d::Vec2 &newPosition) override;
	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;

protected:
	ShaderNode();
	~ShaderNode();

	bool initWithVertex(const std::string &vert, const std::string &frag);
	void loadShaderVertex(const std::string &vert, const std::string &frag);

	void onDraw(const cocos2d::Mat4& transform, uint32_t flags);

	cocos2d::Vec2 _center;
	cocos2d::Vec2 _resolution;
	float      _time;
	std::string _vertFileName;
	std::string _fragFileName;
	cocos2d::CustomCommand _customCommand;
};
#endif // ! __GLTesting_H_