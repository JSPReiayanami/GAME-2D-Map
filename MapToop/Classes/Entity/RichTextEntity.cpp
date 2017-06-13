#include "RichTextEntity.h"
#include "Tool/StringHelper.h"
#define  __TTAG_B "{#"

#define  __TTAG_E "#}"
#define  __TTAG_LENTH 2
#define  __2V32NUM 4294967295
#define  __SPLIT_TAG ";"
RichNodeData::RichNodeData()
{
	 m_Content = "";
	 m_Color = Color3B::BLACK;
	 m_FontSize = 18;
	 m_Type = RichType_Text;
	 m_FontName = "";
	 m_Opacity = 255;
}

RichNodeData::~RichNodeData()
{
}

//=========

RichTextEntity::RichTextEntity()
{
	m_IsCanRefresh = false;
}
RichTextEntity::~RichTextEntity()
{

}
RichTextEntity * RichTextEntity::CreateWithContent(string content, Size size)
{
	RichTextEntity *pRet = new(std::nothrow) RichTextEntity();
	if (pRet && pRet->initWithData(content, size))
	{ 
		pRet->autorelease();
		return pRet;
	} 
	else 
	{ 
		delete pRet; 
		pRet = nullptr; 
		return nullptr; 
	} 
}
bool RichTextEntity::init(){
	if (!Layout::init()){
		return false;
	}

	return true;
}
bool RichTextEntity::initWithData(string content, Size size)
{
	if (!Layout::init()){
		return false;
	}
	m_Content = content;
	m_Size = size;
	this->setContentSize(size);

	this->setColor(Color3B::WHITE);
	this->setBackGroundColor(Color3B::YELLOW);
	this->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	this->setClippingEnabled(true);

	AnalysisContent();
	return true;
}
void RichTextEntity::RefrshView()
{
	if (m_IsCanRefresh == false)
	{
		return;
	}
	this->removeAllChildren();
	this->setContentSize(m_Size);
	RichText * richText = RichText::create();
	richText->ignoreContentAdaptWithSize(false);
	richText->setContentSize(m_Size);
	for (auto & nodeData : m_AllNodeData){
		RichNodeData richDta = nodeData.second;
		switch (richDta.m_Type)
		{
		case RichType_Text:
		{
							  RichElementText * element = RichElementText::create(
								  nodeData.first,
								  richDta.m_Color,
								  richDta.m_Opacity,
								  richDta.m_Content,
								  richDta.m_FontName,
								  richDta.m_FontSize
								  );
							  richText->pushBackElement(element);
							  break;
		}
		case RichType_Image:
		{
							   RichElementImage * element = RichElementImage::create(
								   nodeData.first,
								   richDta.m_Color,
								   richDta.m_Opacity,
								   richDta.m_ImagePath
								   );
								richText->pushBackElement(element);
							    break;
		}
		default:
			break;
		}
	}
	richText->setPosition(m_Size / 2);
	this->addChild(richText);
}
Color3B RichTextEntity::StrConverC3b(string str)
{
	auto r1 = str.substr(1, 2);
	auto g1 = str.substr(3, 4);
	auto b1 = str.substr(5, 6);
	if (str == "" || str.length() != 6){
		return Color3B(0xff, 0xff, 0xff);
	}
	return Color3B(atoi(r1.c_str()), atoi(g1.c_str()), atoi(b1.c_str()));
}

void RichTextEntity::SetContent(string content)
{
	m_Content = content;
	m_AllNodeData.clear();
	AnalysisContent();
	RefrshView();
}

void RichTextEntity::AnalysisContent()
{
	string str_C = m_Content;
	vector<string> allStr = StringHelper::splitString(str_C, __TTAG_B, __TTAG_E);
	for (auto & str : allStr)
	{
		//--切割根据关键字
		RichNodeData richData = SplitContent(str, ";");
		PushRichNodeData(richData);
		//str_C = str_C.substr(index_e + __TTAG_LENTH, str_C.length());
		//CCLOG("%d", m_AllNodeData.size());
		CCLOG(str.c_str());
	}
	//while (true)
	//{
	//	size_t index_b = 0;
	//	size_t index_e = 0;
	//	index_b = str_C.find(__TTAG_B);
	//	index_e = str_C.find(__TTAG_E);
	//	if (index_e <= index_b) break;
	//	//--获取一段
	//	string one_c = str_C.substr(index_b + __TTAG_LENTH, index_e - index_b - __TTAG_LENTH);
	//	
	//}
}

RichNodeData RichTextEntity::SplitContent(const string & content, const string & splitTag)
{
	string str_c = content;
	std::vector<string> allStr;
	allStr.clear();
	string save_str = "";
	while (str_c.length() > 0)
	{
		size_t index_e = 0;
		index_e = str_c.find(splitTag);
		if (index_e == __2V32NUM){
			save_str.append(str_c);
			allStr.push_back(save_str);
			break;
		}
		string one_c = str_c.substr(0, index_e);
		save_str.append(one_c);
		if (str_c.at(index_e+1) != splitTag.at(0)){
			allStr.push_back(save_str);
			save_str = "";
		}
		else
		{
			save_str.append(splitTag);
			index_e += 1;
		}
		str_c = str_c.substr(index_e + 1, str_c.length());
	}
	RichNodeData richData;
	if (allStr.size() >= 1){
		for (int index = 0; index < allStr.size() - 1;index++)
		{
			AnalysisDecorate(richData, allStr.at(index));
		}
		richData.m_Content = allStr.back();
	}
	return richData;
}

void RichTextEntity::AnalysisDecorate(RichNodeData & richData, const string & decoradte)
{
	vector<string> allStr = StringHelper::splitString(decoradte, "=");
	if (allStr.size() >= 2){
		if (allStr.at(0) == "color"){
			vector<string> allColor = StringHelper::splitString(allStr.at(1), ",");
			if (allColor.size() >= 3){
				richData.m_Color = Color3B(atoi(allColor.at(0).c_str()), atoi(allColor.at(1).c_str()), atoi(allColor.at(2).c_str()));
			}
		}
		else if (allStr.at(0) == "FontSize"){
			richData.m_FontSize = atoi(allStr.at(1).c_str());
		}
		else if (allStr.at(0) == "FontName"){
			richData.m_FontName = atoi(allStr.at(1).c_str());
		}
		else if (allStr.at(0) == "Opacity"){
			richData.m_Opacity = atoi(allStr.at(1).c_str());
		}
		else if (allStr.at(0) == "Type"){
			richData.m_Type = (RichType)(atoi(allStr.at(1).c_str()));
		}
		else if (allStr.at(0) == "ImagePath"){
			richData.m_ImagePath = allStr.at(1);
			if (richData.m_ImagePath.length() > 0){
				richData.m_Type = RichType::RichType_Image;
			}
		}
	}	
}

void RichTextEntity::PushRichNodeData(RichNodeData & richData)
{
	m_AllNodeData.insert(make_pair(m_AllNodeData.size() + 1, richData));
}

void RichTextEntity::SetContentSize(const Size& contentSize)
{
	m_Size = contentSize;
	RefrshView();
}

void RichTextEntity::onEnter(){
	Node::onEnter();
	m_IsCanRefresh = true;
	RefrshView();
}
void RichTextEntity::onExit(){
	m_IsCanRefresh = false;
	Node::onExit();
}