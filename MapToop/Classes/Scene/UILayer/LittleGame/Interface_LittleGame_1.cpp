#include "Interface_LittleGame_1.h"
Interface_LittleGame_1::Interface_LittleGame_1()
{
	this->m_Root = nullptr;
	this->m_LayerSrc = "";
	m_GameLevel = 0;
	m_VecNum = Vec2(5, 5);
	m_ViewRes_1 = "GameRes/LittleGame/1.jpg";
	m_ViewRes_2 = "GameRes/LittleGame/2.jpg";
	m_ViewColor_1 = Color3B::WHITE;
	m_ViewColor_2 = Color3B::BLUE;
	m_ChangeGB = nullptr;
	m_ChangePreGB = nullptr;
	m_ChangeTime = 1;
	m_CanTouch = true;
	m_GameState = 0;
	m_Time = 0;
	srand((unsigned)time(NULL));

}
Interface_LittleGame_1::~Interface_LittleGame_1()
{

}

bool Interface_LittleGame_1::init()
{
	if (!Interface_Entity::init())
	{
		return false;
	}
	this->setContentSize(__VisibleSize);
	m_LayoutView = Layout::create();
	m_LayoutView->setContentSize(__VisibleSize);
	this->addChild(m_LayoutView);
	m_GameState = 0;
	InitGame(1);
	scheduleUpdate();
	return true;
}
/*
@设置自己的名字
*/
void Interface_LittleGame_1::SetInterfaceName(const string & name)
{
	this->setName(name);
}
/*
@初始化资源
*/
void Interface_LittleGame_1::InitSrcLayout()
{
	m_Root = (Layout *)CSLoader::createNode(this->m_LayerSrc);
	if (m_Root != nullptr)
	{
		this->addChild(m_Root);
		this->setContentSize(m_Root->getContentSize());
	}
}
/*
@初始化控件
*/
void Interface_LittleGame_1::InitWidget()
{

}
/*
@初始化按钮回调
*/
void Interface_LittleGame_1::InitButtonEvent()
{

}

void Interface_LittleGame_1::OnIntoScene()
{
	Interface_Entity::OnIntoScene();
	//..
}

void Interface_LittleGame_1::OnExitScene()
{
	//..
	Interface_Entity::OnExitScene();
}
void Interface_LittleGame_1::OnClose()
{
	//..
	Interface_Entity::OnClose();
}

void Interface_LittleGame_1::InitGame(int level)
{
	m_AllGameButtons.clear();
	m_LayoutView->removeAllChildrenWithCleanup(true);
	int num_x = m_VecNum.x;
	int num_y = m_VecNum.y;
	m_GameLevel = level;
	Vec2 winSize = __VisibleSize;
	GameButton * gb = GameButton::create();
	gb->SetViewImageView(m_ViewRes_1);
	gb->setPosition(winSize / 2);
	//m_LayoutView->addChild(gb);
	Vec2 oneGbSize = gb->getContentSize();
	Vec2 startPos = Vec2::ZERO;
	startPos.x = winSize.x/2 - oneGbSize.x * floor(num_x / 2.0);
	startPos.y = winSize.y/2 - oneGbSize.y * floor(num_y / 2.0);
	for (int index_y = 0; index_y < num_y;index_y++)
	{
		for (int index_x = 0;index_x < num_x;index_x++)
		{
			GameButton * gb_item = GameButton::create();
			gb_item->SetViewImageView(m_ViewRes_1);
			gb_item->setPosition(startPos + Vec2(index_x * oneGbSize.x , index_y * oneGbSize.y));
			m_LayoutView->addChild(gb_item);
			m_AllGameButtons.push_back(gb_item);
		}
	}

	int randNUM = CCRANDOM_0_1()*m_AllGameButtons.size();
	m_ChangeGB = m_AllGameButtons.at(randNUM);
	m_ChangeGB->SetViewImageView(m_ViewRes_2);
	m_GameState = 1;
}


void Interface_LittleGame_1::update(float delta)
{
	if (m_GameState == 1 && m_CanTouch == true)
	{
		m_Time += delta;
		if (m_Time >= m_ChangeTime)
		{
			m_CanTouch = false;
			while (true)
			{
				int randNUM = CCRANDOM_0_1()*m_AllGameButtons.size();
				m_ChangePreGB = m_AllGameButtons.at(randNUM);
				if (m_ChangeGB != m_ChangePreGB)
				{
					Sequence * seq_pre = Sequence::create(
						ScaleTo::create(1,0.1),
						CallFunc::create([=]() {
							if (m_ChangePreGB)
							{
								m_ChangePreGB->SetViewImageView(m_ViewRes_2);
							}
						}),
						ScaleTo::create(1,1),
						NULL
						);
					
					Sequence * seq = Sequence::create(
						ScaleTo::create(1, 0.1),
						CallFunc::create([=]() {
							if (m_ChangeGB)
							{
								m_ChangeGB->SetViewImageView(m_ViewRes_1);
							}
						}),
						ScaleTo::create(1, 1),
						CallFunc::create([=]() {
							m_ChangeGB = m_ChangePreGB;
							m_CanTouch = true;
						}),
						NULL
						);
					m_ChangePreGB->runAction(seq_pre);
					m_ChangeGB->runAction(seq);
					break;
				}
			}
			m_Time = 0;
		}
	}
}