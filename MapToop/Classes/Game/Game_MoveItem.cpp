#include "Game_MoveItem.h"

//判断路径的引用
#if (__HeadFileIncludeNum == __HeadFileHavePath)
#include "Manager/DataManager.h"
#include "Controller/Controller_Point.h"
#else
#include "DataManager.h"
#include "Controller_Point.h"
#endif//判断路径的引用


Game_MoveItem::Game_MoveItem():
m_MyId(0),
m_Data(nullptr),
m_IsChoice(false),
m_IsRunEnd(false),
m_OutFunctioner(nullptr),
m_OutFunction(nullptr),
m_EndView(nullptr),
m_EndRect(0,0,0,0)
{

}
Game_MoveItem::~Game_MoveItem()
{
    
}
/*
@固定创建方法
*/
Game_MoveItem * Game_MoveItem::CreateWithId(const int &_id)
{
	auto *game_move_item = new(std::nothrow) Game_MoveItem();
	if (game_move_item)
	{
		game_move_item->setMyId(_id);

	}
	if (game_move_item->init())
	{
		game_move_item->autorelease();
		//game_move_item->RunActionWithType();
		return game_move_item;
	}
	else
	{
		delete game_move_item;
		game_move_item = nullptr;
		return nullptr;
	}
}
bool Game_MoveItem::init()
{
	if (!Layout::init())
	{
		return false;
	}
    m_ControllerP = Controller_Point::create();
    m_ControllerP->RegisterCallback(CallFunc_ButtonTypeAndPoint_selector(Game_MoveItem::MovePointEvent), this);
    addChild(m_ControllerP);
    //---
    this->setAnchorPoint(Vec2(0.5,0.5));
	//---获取完毕数据
	auto g_move_data = DataManager::getInstance()->GetMoveItemDataWithId(m_MyId);
	if (nullptr != g_move_data)
	{
		setMyData(g_move_data);
	}
	InitView();
	return true;
}

/*
@初始化显示数据
*/
void Game_MoveItem::InitView()
{
	if (nullptr != m_Data)
	{
        m_ViewItem = ImageView::create(m_Data->getMySrc());
        this->setContentSize(m_ViewItem->getContentSize());
         m_ViewItem->setPosition(m_ViewItem->getContentSize() / 2);
        m_ViewItem->setTouchEnabled(false);
        this->addChild(m_ViewItem);
        //--
        this->setPosition(m_Data->getMySPos());
	}
}
/*
@移动
*/
void Game_MoveItem::MoveToPos(const Vec2 & pos)
{
	this->setPosition(pos);
}

/*
 @坐标移动事件
 */
void Game_MoveItem::MovePointEvent(ui::Widget::TouchEventType t_type,Vec2 t_vec)
{
    if( ui::Widget::TouchEventType::BEGAN == t_type)
    {
       
        CCLOG("[%s] 移动开始 [x:%0.2f y:%0.2f]",m_Data->getMyName().c_str(),t_vec.x,t_vec.y);
        //--判断是否点击到自己
        auto this_pos = this->getPosition();
        auto this_content = this->getContentSize();
        CCLOG("[posx:%0.2f posy:%0.2f]",this_pos.x,this_pos.y);
        CCLOG("[content:%0.2f posy:%0.2f]",this_content.width,this_content.height);
        if(
           t_vec.x >= (this_pos.x - this_content.width/2.0) &&
           t_vec.x <= (this_pos.x + this_content.width/2.0) &&
           (t_vec.y - __ResolutionPolicy_FIXED_WIDTH ) >= (this_pos.y - this_content.height/2.0)&&
           (t_vec.y + __ResolutionPolicy_FIXED_WIDTH )<= (this_pos.y + this_content.height/2.0)
           )
        {
             m_IsChoice = true;
        }
    }else if (ui::Widget::TouchEventType::MOVED == t_type)
    {
        if (true == m_IsChoice) {
            this->MoveToPos(Vec2(t_vec.x,t_vec.y - __ResolutionPolicy_FIXED_WIDTH));
            //CCLOG("[%s] 移动中 [x:%0.2f y:%0.2f]",m_Data->getMyName().c_str(),t_vec.x,t_vec.y);
        }
        
    }else if (ui::Widget::TouchEventType::ENDED == t_type)
    {
        
        m_IsChoice = false;
        IsMoveToEndRect();
        //CCLOG("[%s] 移动结束 [x:%0.2f y:%0.2f]",m_Data->getMyName().c_str(),t_vec.x,t_vec.y);
        if ( false == m_IsRunEnd)
        {
            //--回到原位
            this->setPosition(m_Data->getMySPos());
        }
    }
}

/*
 @判断是否移动到终点区域
 */
void Game_MoveItem::IsMoveToEndRect()
{
    
    auto end_rect = m_EndRect;
    auto pos = this->getPosition();
    if(
       pos.x >= (end_rect.origin.x - end_rect.size.width) &&
       pos.x <= (end_rect.origin.x + end_rect.size.width) &&
       pos.y >= (end_rect.origin.y - end_rect.size.height) &&
       pos.y <= (end_rect.origin.y + end_rect.size.height)
       )
    {
        m_IsRunEnd = true;
        CCLOG("[Game_MoveItem::IsMoveToEndRect]");
    }
    //--判断
    
    if (true ==  m_IsRunEnd) {
        //--RunEndAction
        RunEndAction();
    }
    
}
/*
 @执行最终动画
 */
void Game_MoveItem::RunEndAction()
{
    CCLOG("[Game_MoveItem::RunEndAction]");
    m_ControllerP->CloseTouch();
    //---
    m_ViewItem->setRotation(270);
    this->setPosition(m_Data->getMyEndRect().origin);
    float j_time = 2;
    if (__IsDebugModel == 1)
    {
        j_time = 1;
    }
    //auto jump = JumpBy::create(j_time, Vec2(0,0), 100, 3);
    auto move = MoveBy::create(j_time, Vec2(0,200));
    auto seq = Sequence::create(move,CallFunc::create([=](){
        //--生成一个最终状态的图片在这个道具的父节点上
        auto p_node = this->getParent();
        m_EndView= ImageView::create(m_Data->getMyEndSrc());
        p_node->addChild(m_EndView,1);
        m_EndView->setPosition(m_EndRect.origin);
        m_ViewItem->setVisible(false);
        this->removeAllChildren();
        //--通知外部，已经进入指定区域
        if(nullptr != m_OutFunctioner && nullptr != m_OutFunction)
        {
            (m_OutFunctioner->*m_OutFunction)(m_Data->getMyId(),this);
        }
    }),NULL);
    
    //--
    auto p_rain = ParticleGalaxy::create();
    p_rain->setTexture(Director::getInstance()->getTextureCache()->addImage(m_Data->getMySrc()));
    p_rain->setPosition(Vec2::ZERO);
    this->addChild(p_rain);
    this->runAction(seq);
}
/*
 @开启或关闭移动
 */
void Game_MoveItem::OpenOrCloseMove(bool _bool)
{
    if(true == _bool)
    {
        m_ControllerP->OpenTouch();
    }else if (false == _bool)
    {
        m_ControllerP->CloseTouch();
    }
}

/*
 @注册控制器的回调
 @SEL_CallFunc_IntAndNode Id和自身的函数
 @
 */
void Game_MoveItem::RegisterCallback(SEL_CallFunc_IntAndNode callback, Ref * register_user)
{
    m_OutFunction = callback;
    m_OutFunctioner = register_user;
}

void Game_MoveItem::ClearSomeView()
{
    if(nullptr != m_EndView)
    {
        m_EndView->removeFromParent();
    }
}

/*
 @绑定终点位置的坐标属性
 */
void Game_MoveItem::BandEndRect(const Vec2 & end_point,const Size & end_size)
{
    m_EndRect.origin = end_point;
    m_EndRect.size = end_size;
}

