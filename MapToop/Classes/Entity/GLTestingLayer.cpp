#include "GLTestingLayer.h"

cocos2d::Scene* GLTestingLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = GLTestingLayer::create();
	scene->addChild(layer);
	return scene;
}

bool GLTestingLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->setPosition(0, 0);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto sprite = Sprite::create("HelloWorld.png");
	sprite->setAnchorPoint(Point(0.5, 0.5));
	sprite->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(sprite);
	graySprite(sprite);

	this->setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_COLOR));

	return true;
}

void GLTestingLayer::menuCloseCallback(cocos2d::Ref* pSender)
{

}

void GLTestingLayer::graySprite(Sprite * sprite)
{
	if (sprite)
	{
		GLProgram * p = new GLProgram();
		p->initWithFilenames("tr.vsh", "tr.fsh");
		p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
		p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
		p->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
		p->link();
		p->updateUniforms();
		sprite->setShaderProgram(p);
	}

}

void GLTestingLayer::visit(Renderer* renderer, const Mat4 &parentTransform, bool parentTransformUpdated)
{
	Layer::visit(renderer, parentTransform, parentTransformUpdated);
	_command.init(_globalZOrder);
	_command.func = CC_CALLBACK_0(GLTestingLayer::onDraw, this);
	Director::getInstance()->getRenderer()->addCommand(&_command);
}

void GLTestingLayer::onDraw()
{
	auto glProgram = this->getShaderProgram();
	glProgram->use();
	glProgram->setUniformsForBuiltins();
	glPointSize(10.0f);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	auto size = Director::getInstance()->getWinSize();

	float vertercies[] = { 100, 100,
		200, 200,
		300, 100 };
	float color[] = { 1, 0, 0, 1,
		0, 1, 0, 1,
		0, 0, 1, 1 };
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertercies);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_TRUE, 0, color);
	// 绘制三角形  
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//通知cocos2d-x 的renderer，让它在合适的时候调用这些OpenGL命令  
	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 3);
	CHECK_GL_ERROR_DEBUG();

}



enum
{
	SIZE_X = 256,
	SIZE_Y = 256,
};

ShaderNode::ShaderNode()
:_center(Vec2(0.0f, 0.0f))
, _resolution(Vec2(0.0f, 0.0f))
, _time(0.0f)
{
}

ShaderNode::~ShaderNode()
{
}

ShaderNode* ShaderNode::shaderNodeWithVertex(const std::string &vert, const std::string& frag)
{
	auto node = new (std::nothrow) ShaderNode();
	node->initWithVertex(vert, frag);
	node->autorelease();

	return node;
}

bool ShaderNode::initWithVertex(const std::string &vert, const std::string &frag)
{
	_vertFileName = vert;
	_fragFileName = frag;
#if CC_ENABLE_CACHE_TEXTURE_DATA
	auto listener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, [this](EventCustom* event){
		this->setGLProgramState(nullptr);
		loadShaderVertex(_vertFileName, _fragFileName);
	});

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif

	loadShaderVertex(vert, frag);

	_time = 0;
	_resolution = Vec2(SIZE_X, SIZE_Y);

	scheduleUpdate();

	setContentSize(Size(SIZE_X, SIZE_Y));
	setAnchorPoint(Vec2(0.5f, 0.5f));


	return true;
}

void ShaderNode::loadShaderVertex(const std::string &vert, const std::string &frag)
{
	auto fileUtiles = FileUtils::getInstance();

	// frag
	auto fragmentFilePath = fileUtiles->fullPathForFilename(frag);
	auto fragSource = fileUtiles->getStringFromFile(fragmentFilePath);

	// vert
	std::string vertSource;
	if (vert.empty()) {
		vertSource = ccPositionTextureColor_vert;
	}
	else {
		std::string vertexFilePath = fileUtiles->fullPathForFilename(vert);
		vertSource = fileUtiles->getStringFromFile(vertexFilePath);
	}

	auto glprogram = GLProgram::createWithByteArrays(vertSource.c_str(), fragSource.c_str());
	auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
	setGLProgramState(glprogramstate);
}

void ShaderNode::update(float dt)
{
	_time += dt;
}

void ShaderNode::setPosition(const Vec2 &newPosition)
{
	Node::setPosition(newPosition);
	auto position = getPosition();
	auto frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto retinaFactor = Director::getInstance()->getOpenGLView()->getRetinaFactor();
	_center = Vec2(position.x * frameSize.width / visibleSize.width * retinaFactor, position.y * frameSize.height / visibleSize.height * retinaFactor);
}

void ShaderNode::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	_customCommand.init(_globalZOrder, transform, flags);
	_customCommand.func = CC_CALLBACK_0(ShaderNode::onDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
}

void ShaderNode::onDraw(const Mat4 &transform, uint32_t flags)
{
	float w = SIZE_X, h = SIZE_Y;
	GLfloat vertices[12] = { 0, 0, w, 0, w, h, 0, 0, 0, h, w, h };

	auto glProgramState = getGLProgramState();
	glProgramState->setUniformVec2("resolution", _resolution);
	glProgramState->setUniformVec2("center", _center);
	glProgramState->setVertexAttribPointer("a_position", 2, GL_FLOAT, GL_FALSE, 0, vertices);

	glProgramState->apply(transform);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 6);
}

