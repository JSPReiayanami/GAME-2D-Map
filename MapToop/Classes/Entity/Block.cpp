#include "Block.h"
#define __BlockSrc "B_1.png"
Block::Block()
{
	m_ImageBlock = nullptr;
	m_IsComVisi = true;
	m_IsPoint = false;
}

Block::~Block()
{

}
bool Block::init(){
	if (!Layout::init())
	{
		return false;
	}
	this->setAnchorPoint(Vec2(0.5,0.5));
	return true;
}
Block *  Block::CreateWithInfo(const Size & y_size, const int & y_type)
{
	Block * block = new(std::nothrow) Block();
	if (block && block->init()) 
	{ 
		block->autorelease();
		block->InitWithInfo(y_size, y_type);
		return block;
	} 
	else 
	{ 
		delete block;
		block = nullptr;
		return nullptr; 
	} 
}

void Block::InitWithInfo(const Size & y_size, const int & y_type)
{
	m_Size = y_size;
	m_ImageBlock = ImageView::create(__BlockSrc);
	auto image_size = m_ImageBlock->getContentSize();
	m_ImageBlock->setScaleX(1.0 / ((image_size.width - 0) / (y_size.width - 8)));
	m_ImageBlock->setScaleY(1.0 / ((image_size.height - 0) / (y_size.height - 8)));
	m_ImageBlock->setContentSize(m_Size);
	m_ImageBlock->setAnchorPoint(Vec2(0.5,0.5));
	m_ImageBlock->setPosition(y_size / 2);
	m_ImageBlock->setOpacity(80);
	//--
	m_Text = Text::create();
	m_Text->setFontSize(20);
	m_Text->setColor(Color3B::BLACK);
	
	m_TextObject = Text::create();
	m_TextObject->setFontSize(20);
	m_TextObject->setColor(Color3B::ORANGE);

	this->setContentSize(y_size);
	this->addChild(m_ImageBlock);
	SetBlockType(y_type);


	this->addChild(m_Text);
	m_Text->setPosition(y_size / 2);
	m_Text->setAnchorPoint(Vec2(0.5, 0.5));
	m_Text->setVisible(false);

	m_TextObject->setPosition(y_size / 2);
	m_TextObject->setAnchorPoint(Vec2(0.5, 0.5));
	m_TextObject->setVisible(false);
}

void Block::SetBlockType(const int  & block_type)
{
	m_Type = block_type;
	switch (block_type)
	{
	case  0://正常块
		m_ImageBlock->setColor(Color3B::WHITE);
		break;
	case  1://悬浮层块
		m_ImageBlock->setColor(Color3B::RED);
		break;
	case  2://障碍块
		m_ImageBlock->setColor(Color3B::BLUE);
		break;
	case  3://对象块
		m_ImageBlock->setColor(Color3B::GREEN);
		break;
	default://都不是就默认是正常块
		m_Type = 0;
		break;
	}

}


void Block::SetPos(const PosInt & pos)
{
	m_Pos.setX(pos.getX());
	m_Pos.setY(pos.getY());
	this->setPosition(Vec2((m_Pos.getX() - 0)* m_Size.width + m_Size.width / 2, (m_Pos.getY() - 0)*m_Size.height + m_Size.height / 2));
	m_Text->setString(StringUtils::format("(%d,%d)", pos.getX(), pos.getY()));
}


void Block::SetPosVisi(bool is_visi)
{
	m_Text->setVisible(is_visi);
}

void Block::SetComVisi(bool _is)
{
	if (m_IsComVisi != _is && m_Type == 0)
	{
		m_IsComVisi = _is;
		this->setVisible(m_IsComVisi);
		if (m_IsPoint)
		{
			this->setVisible(m_IsPoint);
		}
	}
	if (m_Type != 0 && m_IsComVisi != _is)
	{
		m_IsComVisi = _is;
	}
}

void Block::IsPoint(bool _is)
{
	m_IsPoint = _is;
	this->stopAllActions();
	if (_is)
	{
		float s_time = 0.4;
		this->setScale(0.8);
		auto seq = Sequence::create(ScaleTo::create(s_time, 0.8), ScaleTo::create(s_time, 0.5), NULL);
		this->runAction(RepeatForever::create(seq));
		this->setVisible(true);
	}
	else{
		this->setScale(1);
		if (m_Type == 0)
		{
			this->setVisible(m_IsComVisi);
		}
	}
}

void Block::TestShader()
{
	auto shader_program = GLProgram::createWithFilenames("shadow.vsh", "shadow.fsh");
	shader_program->use();
	shader_program->setUniformsForBuiltins();
	m_ImageBlock->getVirtualRenderer()->setGLProgram(shader_program);
}
//==============

