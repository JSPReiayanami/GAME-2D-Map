#include "Game_KeyLock.h"
#include "Manager/DataManager.h"
#include "Manager/GameManager.h"
#include "Tool/StringHelper.h"
#include "Ui/Ui_SliderNum.h"
Game_KeyLock::Game_KeyLock() :
m_IsOpen(false),
m_SliderSrc(""),
m_TextSize(10),
m_BackSrc("ItemIco/Public/button.png"),
m_OutFunction(nullptr),
m_OutFunctioner(nullptr),
m_SliderGap(0)
{
	m_KeyNum.clear();
	m_CurKey.clear();
}
Game_KeyLock::~Game_KeyLock()
{
	CC_SAFE_DELETE(m_Data);
}

/*
@固定创建方法
*/
Game_KeyLock * Game_KeyLock::CreateWithNumData(const vector<int> & key_num, const int & text_size, const string & slider_src, const string & back_src, const float & slider_gap)
{
	auto *game_key_lock = new(std::nothrow) Game_KeyLock();
	if (game_key_lock)
	{
		game_key_lock->setMyTextSize(text_size);
		game_key_lock->setMySliderSrc(slider_src);
		game_key_lock->setMyBackSrc(back_src);
		game_key_lock->setMySliderGap(slider_gap);
		game_key_lock->InitKeyNum(key_num);

	}
	if (game_key_lock->init())
	{
		game_key_lock->autorelease();
		return game_key_lock;
	}
	else
	{
		delete game_key_lock;
		game_key_lock = nullptr;
		return nullptr;
	}
}

Game_KeyLock * Game_KeyLock::CreateWithId(const int & _id)
{
	auto *game_key_lock = new(std::nothrow) Game_KeyLock();
	if (game_key_lock)
	{
		game_key_lock->SetKeyLockId(_id);
	}
	if (game_key_lock->init())
	{
		game_key_lock->autorelease();
		return game_key_lock;
	}
	else
	{
		delete game_key_lock;
		game_key_lock = nullptr;
		return nullptr;
	}
}

/*
@固定创建方法
*/
bool Game_KeyLock::init(){
	if (!Layout::init())
	{
		return false;
	}
	this->setContentSize(__VisibleSize);
	//--
	RefreshView();
	return true;
}
void Game_KeyLock::InitKeyNum(const vector<int> & key_num)
{
	for (int i = 0; i < key_num.size(); i++)
	{
		m_KeyNum.push_back(key_num.at(i));
	}
}
/*
@刷新显示
*/
void Game_KeyLock::RefreshView()
{
	this->removeAllChildrenWithCleanup(true);
	//---背景设置
	auto img_background = ImageView::create(m_Data->getMySrc());
	img_background->setAnchorPoint(Vec2(0.5, 0.5));
	img_background->setPosition(__VisibleSize / 2);
	this->addChild(img_background);
	auto btn = Button::create(m_BackSrc);
	btn->setScale9Enabled(true);
	btn->setContentSize(__VisibleSize);
	btn->setPosition(__VisibleSize / 2);
	btn->addTouchEventListener(CC_CALLBACK_2(Game_KeyLock::TouchCloseKeyLockEvent, this));
	//btn->setOpacity(0);
	this->addChild(btn);
	for (int i = 0; i < m_KeyNum.size(); i++)
	{
		//--
		auto s_num = Ui_SliderNum::create();
		if (i == 0)
		{
			this->setContentSize(Size(s_num->getContentSize().width * m_KeyNum.size(), s_num->getContentSize().height));
		}
		auto con_size = s_num->getContentSize();
		s_num->ChanageBackground(m_Data->getMyBagIco(), m_TextSize);
		s_num->setTag(i + 1);
		s_num->RegisterNumChangeCallback(CallFunc_IntAndNode_selector(Game_KeyLock::GetSliderNumChange), this);
		s_num->setAnchorPoint(Vec2::ZERO);
		s_num->setPositionY(m_StartPos.y);
		s_num->setPositionX(m_StartPos.x + i * (s_num->getContentSize().width + m_SliderGap));
		m_CurKey.push_back(s_num->GetCurViewNum());
		this->addChild(s_num);
	}
	
}
/*
@接收滑动数字的改变
*/
void Game_KeyLock::GetSliderNumChange(int num, Node * slider_num)
{
	CCLOG("[SlideNum %d] Key : %d",slider_num->getTag(),num);
	m_CurKey.at(slider_num->getTag() - 1) = num;
	JudgeIsOpen();
	if (m_IsOpen)
	{
		CallOutFunction();
		CloseKeyLock();
		CCLOG("[Game_KeyLock][Is Open]");
	}
}
/*
@判断当前的数字和密码是否一致
*/
void Game_KeyLock::JudgeIsOpen()
{
	m_IsOpen = true;
	for (int i = 0; i < m_KeyNum.size();i++)
	{
		if (m_KeyNum.at(i) != m_CurKey.at(i))
		{
			m_IsOpen = false;
			break;
		}
	}
}
/*
@注册解密成功或者失败的回调事件
*/
void Game_KeyLock::RegisterUnlockCallback(SEL_CallFunc_Bool callback, Ref * register_user)
{
	m_OutFunction = callback;
	m_OutFunctioner = register_user;
}
/*
@调用外部函数
*/
void Game_KeyLock::CallOutFunction()
{
	if (m_OutFunctioner && m_OutFunction)
	{
		(m_OutFunctioner->*m_OutFunction)(m_IsOpen);
	}
}

/*
设置Id
*/
void Game_KeyLock::SetKeyLockId(const int & _id)
{
	m_Id = _id;
	m_Data = DataManager::getInstance()->GetGameItemDataWithId(m_Id);
	//--背景加载
	if (m_Data)
	{
		//---解析数据
		auto lock_data = StringHelper::splitString(m_Data->getMyValue(), ";");
		if (lock_data.size() == 4)
		{
			//---初始坐标
			auto pos_data = StringHelper::splitString(lock_data.at(0), ",");
			Vec2 start_pos(atoi(pos_data.at(0).c_str()), atoi(pos_data.at(1).c_str()));
			m_StartPos = start_pos;
			//---基础显示数据
			auto view_data = StringHelper::splitString(lock_data.at(2), ",");
			m_TextSize = atoi(view_data.at(0).c_str());
			//--密纹保存
			auto key_data = StringHelper::splitString(lock_data.at(1), ",");
			for (int i = 0; i < key_data.size(); i++)
			{
				m_KeyNum.push_back(atoi(key_data.at(i).c_str()));
			}
			//--间隙
			setMySliderGap(atof(lock_data.at(3).c_str()));
			
		}
	}
}

/*
@关闭密码
*/
void Game_KeyLock::CloseKeyLock()
{
	this->removeFromParentAndCleanup(true);
}

/*
@点击后关闭密码
*/
void Game_KeyLock::TouchCloseKeyLockEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (eventType == ui::Widget::TouchEventType::ENDED)
	{
		CloseKeyLock();
	}
}