#include "ListView3D.h"

ListView3D_Delegete::ListView3D_Delegete()
{
}

ListView3D_Delegete::~ListView3D_Delegete()
{
}
Layout * ListView3D_Delegete::GetPageLayoutWithIndex(int index)
{
	Layout * layout = nullptr;

	return layout;
}

void ListView3D_Delegete::CurShowPage(Layout * layout,int index)
{

}
//====
ListView3D::ListView3D(){
	m_Delegete = nullptr;
	m_ShowNum = 3;
	m_OnePageSize = Size(200, 200);
	m_CurIndex = 1;
	m_AllNum = 0;
	m_AllItem.clear();
}

ListView3D::~ListView3D(){

}

bool ListView3D::init(){
	if (!Layout::init())
	{
		return false;
	}
	this->setColor(Color3B::RED);
	this->setBackGroundColor(Color3B::YELLOW);
	this->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	this->setClippingEnabled(true);
	//===
	//m_PageView = PageView::create();
	//m_PageView->setIndicatorSelectedIndexColor(Color3B::GREEN);
	//m_PageView->addEventListenerPageView(this, pagevieweventselector(ListView3D::PageViewCallFunc,this));
	//m_PageView->addEventListener(CC_CALLBACK_2(ListView3D::PageViewCallFunc,this));
	//m_PageView->setDirection(ScrollView::Direction::HORIZONTAL);
	//m_PageView->setClippingEnabled(false);
	//m_PageView->setClippingType(Layout::ClippingType::STENCIL);
	//this->addChild(m_PageView);
	//===
	m_ListView = ListView::create();
	m_ListView->setDirection(ScrollView::Direction::HORIZONTAL);
	m_ListView->addEventListener((ui::ListView::ccScrollViewCallback)CC_CALLBACK_2(ListView3D::ScrollCallFunc, this));
	m_ListView->addEventListener((ui::ListView::ccListViewCallback)CC_CALLBACK_2(ListView3D::ListViewCallFunc, this));
	this->addChild(m_ListView);
	//===
	SetOnePageSize(m_OnePageSize);
	return true;
}
/*
@定义委托者
*/
void ListView3D::SetDelegete(ListView3D_Delegete * delegete){
	m_Delegete = delegete;
}

/*
@设定一个page多大
*/
void ListView3D::SetOnePageSize(Size size){
	m_OnePageSize = size;
	Size contentSize;
	contentSize.width = m_ShowNum * size.width;
	contentSize.height = size.height;
	this->setContentSize(contentSize);
	m_ListView->setContentSize(contentSize);
	//this->setStencilClippingSize(contentSize);
}
Size ListView3D::GetOnePageSize(){
	return m_OnePageSize;
}
/*
@设定几个页面
*/
void ListView3D::SetAllPageNum(int num){
	if (num <= 0) return;
	m_AllNum = num;
	UpdateView();
}
//======private
void ListView3D::ListViewCallFunc(Ref* sender, ListView::EventType  pageType){
	ListView * list = (ListView*)sender;
	Vec2 vec =  list->getScrollBarPositionFromCornerForHorizontal();
	CCLOG("ListView3D::ListViewCallFunc[%0.1f,%0.1f]", vec.x , vec.y);
}

void ListView3D::ScrollCallFunc(Ref* sender, ScrollView::EventType  pageType){
	ListView * list = (ListView*)sender;
	if (ScrollView::EventType::SCROLLING == pageType){
		//CCLOG("ScrollCallFunc=>>[SCROLLING]");
		//Vec2 inerVec = list->getInnerContainerPosition();
		//CCLOG("ListView3D::ListViewCallFunc[%0.1f,%0.1f]", inerVec.x, inerVec.y);
	}
	else if (ScrollView::EventType::CONTAINER_MOVED == pageType)
	{
		//CCLOG("ScrollCallFunc=>>[CONTAINER_MOVED]");
	}
	
}
void ListView3D::UpdateView(){
	if (m_Delegete == nullptr) return;
	m_ListView->removeAllItems();
	m_AllItem.clear();
	//--先生成一个默认的空的Lay
	Layout * layHead = Layout::create();
	layHead->setContentSize(m_OnePageSize);
	m_ListView->pushBackCustomItem(layHead);
	m_AllItem.insert(make_pair(0, layHead));
	//--暂时一次性生成完
	for (int index = 0; index < m_AllNum;index++)
	{
		Layout * lay = m_Delegete->GetPageLayoutWithIndex(index + 1);
		Layout * scaleLay = Layout::create();
		scaleLay->setContentSize(m_OnePageSize);
		scaleLay->addChild(lay);
		scaleLay->setName("DelegeteLay");
		Layout * faLay = Layout::create();
		faLay->setContentSize(m_OnePageSize);
		faLay->addChild(scaleLay);
		scaleLay->setAnchorPoint(Vec2(0.5,0.5));
		scaleLay->setPosition(m_OnePageSize / 2);
		m_ListView->pushBackCustomItem(faLay);
		m_AllItem.insert(make_pair(index + 1, faLay));
	}
	Layout * layEnd = Layout::create();
	layEnd->setContentSize(m_OnePageSize);
	m_ListView->pushBackCustomItem(layEnd);
	m_AllItem.insert(make_pair(m_AllNum+1, layHead));
}
void ListView3D::ScaleWithInnerPosition(Vec2 vec){
	if (m_LastInnerVec == vec) return;
	m_LastInnerVec = vec;
	float ScaleNum = 1;
	float movePos = fabs(vec.x);
	int headNum = floor(movePos / m_OnePageSize.width);
	float disPos_0 = movePos - headNum * m_OnePageSize.width;
	float scaleFloat1 = (fabs(m_OnePageSize.width - disPos_0) / m_OnePageSize.width) * ScaleNum + 1;
	float scaleFloat2 = (fabs(disPos_0) / m_OnePageSize.width) * ScaleNum + 1;
	for (auto & layout : m_AllItem)
	{
		Layout * lay = (Layout *)layout.second->getChildByName("DelegeteLay");
		if (lay == nullptr){
			lay = (Layout*)(Helper::seekWidgetByName(layout.second,"DelegeteLay"));
		}
		if (lay == nullptr) continue;
		if (layout.first == headNum){

		}
		else if (layout.first == (headNum + 1))
		{
			lay->setScale(scaleFloat1);
		}
		else if (layout.first == (headNum + 2))
		{
			lay->setScale(scaleFloat2);
		}
		else
		{
			lay->setScale(1);
		}
	}
}

void ListView3D::CamberWithInnerPosition(Vec2 vec)
{
	if (m_LastInnerVec == vec) return;
	m_LastInnerVec = vec;
	float ScaleNum = 1;
	float movePos = fabs(vec.x);
	int headNum = floor(movePos / m_OnePageSize.width);
	float disPos_0 = movePos - headNum * m_OnePageSize.width;
	float scaleFloat1 = (fabs(m_OnePageSize.width - disPos_0) / m_OnePageSize.width) * ScaleNum + 1;
	float scaleFloat2 = (fabs(disPos_0) / m_OnePageSize.width) * ScaleNum + 1;
	for (auto & layout : m_AllItem)
	{
		Layout * lay = (Layout *)layout.second->getChildByName("DelegeteLay");
		if (lay == nullptr) {
			lay = (Layout*)(Helper::seekWidgetByName(layout.second, "DelegeteLay"));
		}
		if (lay == nullptr) continue;
		if (layout.first == headNum) {

		}
		else if (layout.first == (headNum + 1))
		{
			lay->setScale(scaleFloat1);
		}
		else if (layout.first == (headNum + 2))
		{
			lay->setScale(scaleFloat2);
		}
		else
		{
			lay->setScale(1);
		}
	}
}
void ListView3D::onEnter(){
	Layout::onEnter();
	UpdateView();
	this->scheduleUpdate();
}
void ListView3D::onExit(){
	Layout::onExit();
	this->unscheduleUpdate();
}

void ListView3D::update(float delta){
	Layout::update(delta);
	Vec2 inerVec = m_ListView->getInnerContainerPosition();
	ScaleWithInnerPosition(inerVec);
}

