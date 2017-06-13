#include "Ui_SliderNum.h"
#include "Manager/DataManager.h"
#include "Manager/GameManager.h"
#include "Manager/SoundManager.h"
#include "Controller/Controller_Square.h"
#define  _Ui_SliderNum_Src "UI/Ui_SliderNum.csb"
/*
@构造,析构
*/
Ui_SliderNum::Ui_SliderNum() :
m_TextTop(nullptr),
m_TextMiddle(nullptr),
m_TextDown(nullptr),
m_MinNum(0),
m_MaxNum(9),
m_IsNumChangeOver(true),
m_MoveTime(0.1),
m_OutFunctioner(nullptr),
m_OutFunction(nullptr),
m_ControSquare(nullptr),
m_IsTouched(false)
{
	m_LayerSrc = _Ui_SliderNum_Src;
	m_TextVec2.clear();
	m_Num.insert(make_pair(1, 9));
	m_Num.insert(make_pair(2, 0));
	m_Num.insert(make_pair(3, 1));

}
Ui_SliderNum::~Ui_SliderNum()
{

}
/*
@固定创建方法
*/
bool Ui_SliderNum::init()
{
	if (!Interface_Entity::init())
	{
		return false;
	}
	m_ControSquare = Controller_Square::create();
	m_ControSquare->RegisterCallback(CallFunc_SquareEnum_SELECTOR(Ui_SliderNum::TouchSquare), this);
	m_ControSquare->OpenTouch();
	this->addChild(m_ControSquare);
	return true;
}
/*
@设置自己的名字
*/
void Ui_SliderNum::SetInterfaceName()
{
	this->setName("Ui_SliderNum");
	CCLOG("Ui_SliderNum");
}
/*
@初始化资源
*/
void Ui_SliderNum::InitSrcLayout()
{
	m_Root = (Layout *)CSLoader::createNode(m_LayerSrc);
	this->addChild(m_Root);
}
/*
@初始化控件
*/
void Ui_SliderNum::InitWidget()
{

	__GetChildAndAssignmentWithOB(m_TextTop, m_Root, "Text_Top", Text)
		__GetChildAndAssignmentWithOB(m_TextMiddle, m_Root, "Text_Middle", Text)
		__GetChildAndAssignmentWithOB(m_TextDown, m_Root, "Text_Down", Text)
		__GetChildAndAssignmentWithOB(m_ImageBackground, m_Root, "Image_Background", ImageView)
		__GetChildAndAssignmentWithOB(m_ButtonChoice, m_Root, "Button_Choice", Button)
		m_TextVec2.insert(make_pair(1, m_TextTop->getPosition()));
	m_TextVec2.insert(make_pair(2, m_TextMiddle->getPosition()));
	m_TextVec2.insert(make_pair(3, m_TextDown->getPosition()));
	this->setContentSize(m_ImageBackground->getContentSize());
	m_TextTop->setLocalZOrder(5);
	m_TextMiddle->setLocalZOrder(5);
	m_TextDown->setLocalZOrder(5);

}
/*
@初始化按钮的回调事件
*/
void Ui_SliderNum::InitButtonEvent()
{
	m_ButtonChoice->addTouchEventListener(CC_CALLBACK_2(Ui_SliderNum::TouchedEvent,this));
}

/*
@选择事件
*/
void Ui_SliderNum::TouchedEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::BEGAN)
	{
		m_IsTouched = true;
	}
	else if (eventType == ui::Widget::TouchEventType::CANCELED)
	{
		m_IsTouched = false;
	}
	else if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		m_IsTouched = false;
	}
}

/*
@触摸的方向
*/
void Ui_SliderNum::TouchSquare(Square_DirType s_type)
{
	if (m_IsTouched)
	{
		switch (s_type)
		{
		case Square_DirType_Up:
			if (m_IsNumChangeOver)
			{
				CCLOG("[Square_DirType_Up]");
				RunUp();
			}
			break;
		case Square_DirType_Donw:
			if (m_IsNumChangeOver)
			{
				CCLOG("[Square_DirType_Donw]");
				RunDown();
			}
			break;
		default:
			break;
		}
	}

}
void Ui_SliderNum::RunUp()
{
	SoundManager::getInstance()->PlayEffectMusic(10002);
	m_IsNumChangeOver = false;
	//--写动画
	Spawn *mid_spa = nullptr;
	Spawn *down_spa = nullptr;
	Sequence * this_seq = nullptr;
	float time_ = m_MoveTime;
	//---上面的
	{
		m_TextTop->setPosition(m_TextVec2.at(3));
		m_TextTop->setOpacity(0);
	}
	//---中间的
	{
		auto move_to = MoveTo::create(time_, m_TextVec2.at(1));
		auto fide_in = FadeOut::create(time_);//
		mid_spa = Spawn::create(move_to, fide_in, NULL);
	}
	//---下面的
	{
		auto move_to = MoveTo::create(time_, m_TextVec2.at(2));
		auto fide_in = FadeIn::create(time_);
		down_spa = Spawn::create(move_to, fide_in, NULL);
	}
	//---改变数字
	{
		this_seq = Sequence::create(
			DelayTime::create(time_ / 2), 
			CallFunc::create([=]{
			m_TextTop->setString(StringUtils::format("%d", m_Num.at(1)));
			m_TextMiddle->setString(StringUtils::format("%d", m_Num.at(2)));
			m_TextDown->setString(StringUtils::format("%d", m_Num.at(3)));
		}), 
			DelayTime::create(time_*1.5),
			CallFunc::create([=]{
			//---结束
			m_IsNumChangeOver = true;
			CallOutFunction();
		}),
			NULL);
		
	}
	m_TextMiddle->runAction(mid_spa);
	m_TextDown->runAction(down_spa);
	this->runAction(this_seq);
	//--交换指针
	Text * temp_t;
	temp_t = m_TextTop;
	m_TextTop = m_TextMiddle;
	m_TextMiddle = m_TextDown;
	m_TextDown = temp_t;
	m_Num.at(1) = atoi(CalculateNumIsAdd(StringUtils::format("%d", m_Num.at(1)), true).c_str());
	m_Num.at(2) = atoi(CalculateNumIsAdd(StringUtils::format("%d", m_Num.at(2)), true).c_str());
	m_Num.at(3) = atoi(CalculateNumIsAdd(StringUtils::format("%d", m_Num.at(3)), true).c_str());

	
}
void Ui_SliderNum::RunDown()
{
	SoundManager::getInstance()->PlayEffectMusic(10002);
	m_IsNumChangeOver = false;
	//--写动画
	Spawn *top_spa = nullptr;
	Spawn *mid_spa = nullptr;
	Sequence * this_seq = nullptr;
	float time_ = m_MoveTime;
	//---上面的
	{
		auto move_to = MoveTo::create(time_, m_TextVec2.at(2));
		auto fide_in = FadeIn::create(time_);//
		top_spa = Spawn::create(move_to, fide_in, NULL);
	}
	//---中间的
	{
		auto move_to = MoveTo::create(time_, m_TextVec2.at(3));
		auto fide_in = FadeOut::create(time_);
		mid_spa = Spawn::create(move_to, fide_in, NULL);
	}
	//---下面的
	{
		
		m_TextDown->setPosition(m_TextVec2.at(1));
		m_TextDown->setOpacity(0);
	}
	//---改变数字
	{
		this_seq = Sequence::create(
			DelayTime::create(time_ / 2),
			CallFunc::create([=]{
			m_TextTop->setString(StringUtils::format("%d", m_Num.at(1)));
			m_TextMiddle->setString(StringUtils::format("%d", m_Num.at(2)));
			m_TextDown->setString(StringUtils::format("%d", m_Num.at(3)));
		}),
			DelayTime::create(time_*1.5),
			CallFunc::create([=]{
			//---结束
			m_IsNumChangeOver = true;
			CallOutFunction();
		}),
			NULL);

	}
	m_TextTop->runAction(top_spa);
	m_TextMiddle->runAction(mid_spa);
	this->runAction(this_seq);
	//--交换指针
	Text * temp_t;
	temp_t = m_TextTop;
	m_TextTop = m_TextDown;
	m_TextDown = m_TextMiddle;
	m_TextMiddle = temp_t;
	m_Num.at(1) = atoi(CalculateNumIsAdd(StringUtils::format("%d", m_Num.at(1)), false).c_str());
	m_Num.at(2) = atoi(CalculateNumIsAdd(StringUtils::format("%d", m_Num.at(2)), false).c_str());
	m_Num.at(3) = atoi(CalculateNumIsAdd(StringUtils::format("%d", m_Num.at(3)), false).c_str());
}

/*
@处理数字
*/
string  Ui_SliderNum::CalculateNumIsAdd(string  str_num, bool _is)
{
	int num = atoi(str_num.c_str());
	if (_is)
	{
		//--加
		num++;
		if (num>m_MaxNum)
		{
			num = m_MinNum;
		}
	}
	else{
		//--减
		num--;
		if (num<m_MinNum)
		{
			num = m_MaxNum;
		}
	}
	return StringUtils::format("%d",num);
}
/*
@获取当前的数字
*/
int Ui_SliderNum::GetCurViewNum()
{
	return atoi(m_TextMiddle->getString().c_str());
}
/*
@注册数字改变回调
*/
void Ui_SliderNum::RegisterNumChangeCallback(SEL_CallFunc_IntAndNode callback, Ref * register_user)
{
	m_OutFunction = callback;
	m_OutFunctioner = register_user;
}
/*
@调用外部函数
*/
void Ui_SliderNum::CallOutFunction()
{
	if (m_OutFunction != nullptr)
	{
		(m_OutFunctioner->*m_OutFunction)(m_Num.at(2),this);
	}
}
void Ui_SliderNum::ChanageBackground(const string & src_back, const int & text_size)
{
	m_ImageBackground->removeFromParentAndCleanup(true);
	m_ImageBackground = ImageView::create(src_back);
	m_Root->addChild(m_ImageBackground, 1);
	m_ButtonChoice->loadTextures(src_back, src_back, src_back);
	if (text_size > 0)
	{
		m_TextTop->setFontSize(text_size);
		m_TextMiddle->setFontSize(text_size);
		m_TextDown->setFontSize(text_size);
	}
	//--
	this->setContentSize(m_ImageBackground->getContentSize());
	m_ButtonChoice->setContentSize(m_ImageBackground->getContentSize());
	m_ImageBackground->setAnchorPoint(Vec2(0.5,0.5));
	m_ImageBackground->setPosition(m_ImageBackground->getContentSize() / 2);
	m_TextVec2.at(2) = m_ImageBackground->getContentSize() / 2;
	m_TextVec2.at(1) = Vec2(m_TextVec2.at(2).x, m_ImageBackground->getContentSize().height + m_TextTop->getContentSize().height/2);
	m_TextVec2.at(3) = Vec2(m_TextVec2.at(2).x,-m_TextTop->getContentSize().height / 2);
	//---
	m_TextTop->setString(StringUtils::format("%d", m_Num.at(1)));
	m_TextMiddle->setString(StringUtils::format("%d", m_Num.at(2)));
	m_TextDown->setString(StringUtils::format("%d", m_Num.at(3)));
	//---
	m_TextTop->setPosition(m_TextVec2.at(1));
	m_TextMiddle->setPosition(m_TextVec2.at(2));
	m_TextDown->setPosition(m_TextVec2.at(3));
}

/*
@开启滑动
@关闭滑动
*/
void Ui_SliderNum::OpenSlider()
{
	m_ControSquare->OpenTouch();
}
void Ui_SliderNum::CloseSlider()
{
	m_ControSquare->CloseTouch();
}


