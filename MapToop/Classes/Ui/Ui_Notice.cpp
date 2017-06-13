#include "Ui_Notice.h"
#include "Manager/DataManager.h"
#define  _Ui_Noticeg_Src "UI/Ui_Notice.csb"
/*
@����,����
*/
Ui_Notice::Ui_Notice()
{
	m_LayerSrc = _Ui_Noticeg_Src;

}
Ui_Notice::~Ui_Notice()
{

}
Ui_Notice * Ui_Notice::CreateWithId(const int & _id)
{
	auto *ui_notice = new(std::nothrow) Ui_Notice();
	if (ui_notice)
	{
		ui_notice->setMyId(_id);

	}
	if (ui_notice->init())
	{
		ui_notice->autorelease();
		return ui_notice;
	}
	else
	{
		delete ui_notice;
		ui_notice = nullptr;
		return nullptr;
	}
}

/*
@�̶���������
*/
bool Ui_Notice::init()
{
	if (!Interface_Entity::init())
	{
		return false;
	}

	return true;
}
/*
@�����Լ�������
*/
void Ui_Notice::SetInterfaceName()
{
	this->setName("Ui_Bag");
	CCLOG("Ui_Bag");
}
/*
@��ʼ����Դ
*/
void Ui_Notice::InitSrcLayout()
{
	m_Root = (Layout *)CSLoader::createNode(m_LayerSrc);
	this->addChild(m_Root);
}
/*
@��ʼ���ؼ�
*/
void Ui_Notice::InitWidget()
{
	__GetChildAndAssignmentWithOB(m_ListText, m_Root, "ListView_Content",ListView)
}
/*
@��ʼ����ť�Ļص��¼�
*/
void Ui_Notice::InitButtonEvent()
{
	__AddTouchEventWithNameAndPNode("Button_Close", m_Root, Ui_Notice::TouchCloseEvent)
}

/*
@ˢ����ʾ
*/
void Ui_Notice::RefreshView()
{
	//---
	m_ListText->removeAllItems();
	auto n_data = DataManager::getInstance()->GetNoticeData(m_Id);
	if (nullptr != n_data)
	{
		for (auto & str : n_data->getMyContent())
		{
			//--�����ı�
			Text * text_notice = Text::create();
			text_notice->setFontSize(36);
			text_notice->setString(str);
			float bs_width = text_notice->getContentSize().width / m_ListText->getContentSize().width;
			float t_hight = text_notice->getContentSize().height;
			if (bs_width >= 1)
			{
				auto ccc = ceil(bs_width);
				text_notice->ignoreContentAdaptWithSize(false);
				text_notice->setContentSize(Size(m_ListText->getContentSize().width, t_hight * ceil(bs_width)));
				text_notice->setString(str);
				//text_notice->ignoreAnchorPointForPosition(true);
			}
			m_ListText->pushBackCustomItem(text_notice);
		}
	}
}
/*
@��ť�Ļص��¼�
*/
void Ui_Notice::TouchCloseEvent(Ref * pSender, ui::Widget::TouchEventType eventType)
{
	if (ui::Widget::TouchEventType::ENDED == eventType)
	{
		this->removeFromParentAndCleanup(true);
	}
}

/*
	@
*/
void Ui_Notice::onEnter()
{
	Layout::onEnter();
	RefreshView();
}


