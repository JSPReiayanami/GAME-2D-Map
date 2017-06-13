#include "Slots.h"
#define  MIDE_TIME 0.2
#define  START_TIME 1
#define  START_TIMEC 0.15
#define  END_TIMEC 0.15
#define  END_TIME 2//最终慢慢的速度随便调整

#define InsertDnowMap(index,num)\
{\
if (m_DownImgae.find(index) == m_DownImgae.end()){\
		std::vector<int> temp;\
		temp.clear();\
		m_DownImgae.insert(std::make_pair(index, temp)); \
	}\
	m_DownImgae[index].push_back(num); \
}
Slots::Slots()
{
	m_IsStop = false;
	m_IsOver = true;
	m_CurIndex = 4;
	m_Max = 9;
	rand_1 = 3;
	rand_2 = 2;
	rand_3 = 1;
	InitImage();
	UseImage(rand_1);
	UseImage(rand_2);
	UseImage(rand_3);
	for (int index = 0; index < 9;index++)
	{
		allImage.push_back(index + 1);
	}
	m_ImageName.clear();
	m_DownImgae.clear();
	m_ImageName.insert(std::make_pair(1, "cell/bar.png"));
	InsertDnowMap(1, 2);
	InsertDnowMap(1, 3);
	InsertDnowMap(1, 6);
	m_ImageName.insert(std::make_pair(2, "cell/hu.png"));
	InsertDnowMap(2, 1);
	InsertDnowMap(2, 3);
	m_ImageName.insert(std::make_pair(3, "cell/lbar.png"));
	InsertDnowMap(3, 1);
	InsertDnowMap(3, 4);
	InsertDnowMap(3, 8);
	m_ImageName.insert(std::make_pair(4, "cell/ldang.png"));
	InsertDnowMap(4, 5);
	m_ImageName.insert(std::make_pair(5, "cell/long.png"));
	InsertDnowMap(5, 2);
	m_ImageName.insert(std::make_pair(6, "cell/niu.png"));
	InsertDnowMap(6, 3);
	m_ImageName.insert(std::make_pair(7, "cell/she.png"));
	InsertDnowMap(7, 1);
	m_ImageName.insert(std::make_pair(8, "cell/shu.png"));
	InsertDnowMap(8, 1);
	m_ImageName.insert(std::make_pair(9, "cell/tu.png"));
	InsertDnowMap(9, 1);

}
Slots::~Slots()
{

}
/*
@固定创建方法
*/
bool Slots::init(){
	if (!Node::init())
	{
		return false;
	}
	InitSlots();
	RefreshView();
	return true;
}

void Slots::InitSlots(){
	ImageView * t_img = ImageView::create(m_ImageName.at(1));//随便生成一个获取大小
	for (int i = 1; i <= 3; i++)
	{
		ImageView * img = ImageView::create(m_ImageName.at(i));
		this->addChild(img);
		img->setPositionY(t_img->getContentSize().height*1.5 - (i - 1)*t_img->getContentSize().height);
		m_Vec2.insert(std::make_pair(i, img->getPosition()));
		m_Images.insert(std::make_pair(i, img));
	}
}

void Slots::RunToIndex(int index)
{
	if (m_IsOver == false)return;
	m_IsForceEnter = 0;
	endFlag = false;
	m_IsOver = false;
	m_TargetIndex = index;
	m_CurRunType = 1;
	m_StartCount = (int)(CCRANDOM_0_1() * 5) + 10;//5-10圈
	m_MideCount = (int)(CCRANDOM_0_1() * 5) + 10;//5-10圈
	m_EndCount = (int)(CCRANDOM_0_1() * 5) + 10;//5-10圈
	m_CurStartTime = START_TIME;//--给开始的动态时间赋值
	//--计算最终停留到想要的位置需要多转几个位置
	int all_count = m_StartCount;
	all_count += m_MideCount;
	all_count += m_EndCount;
	int yu = all_count % m_Max;
	int add_index = (yu + m_CurIndex) > m_Max ? ((yu + m_CurIndex) - m_Max) : (yu + m_CurIndex);
	if (add_index < index)
	{
		m_EndCount += (index - add_index);
	}
	else
	{
		m_EndCount += (m_Max - add_index + index);
	}
	StartAction();
}

void Slots::RefreshView()
{
	
	int top = m_CurIndex + 1;
	int dowm = m_CurIndex - 1;
	if (top > m_Max)
	{
		top = 1;
	}
	if (dowm <= 0)
	{
		dowm = 9;
	}
	int tempImg = rand_3;
	rand_3 = rand_2;
	rand_2 = rand_1;
	
	if (endFlag == false)
		rand_1 = PopImage();
	else
		rand_1 = top;
	//--
	if (m_EndCount == 2){
		int randD = (int)(CCRANDOM_0_1() * (m_DownImgae.at(m_TargetIndex).size()));
		rand_1 = m_DownImgae.at(m_TargetIndex).at(randD);
	}
	if (m_EndCount == 1)
	{
		rand_1 = m_TargetIndex;
	}
	if (m_EndCount == 0){
		rand_1 = top;
	}
	m_Images.at(1)->loadTexture(m_ImageName.at(rand_1));//
	m_Images.at(2)->loadTexture(m_ImageName.at(rand_2));//--中间图片肯定是指定的上下自己计算是啥
	m_Images.at(3)->loadTexture(m_ImageName.at(rand_3));//
	UseImage(rand_1);
	UnUseImage(tempImg);
}
void Slots::StartAction()
{
	m_StartCount--;
	if (m_StartCount == -1)
	{
		m_CurRunType = 2;
		MidelAction();
	}
	else
	{
		DoGun(START_TIME,1,START_TIMEC);
	}
}
void Slots::MidelAction()
{
	m_MideCount--;
	if (m_MideCount == -1)
	{
		m_CurRunType = 3;
		m_CurEndTime = MIDE_TIME;//给动态终止时间赋值
		EndAction();
	}
	else{
		DoGun(MIDE_TIME);
	}
	
}
void Slots::EndAction()
{
	m_EndCount--;
	if (m_EndCount == -1)
	{
		DoEnd();
	}
	else{
		if (m_EndCount <= 3) endFlag = true;
		DoGun(MIDE_TIME,3,END_TIMEC);
	}
}
void Slots::DoEnd()
{
	//---这个是运动完后的会调用的
	CCLOG("Over");
	m_IsOver = true;
}
void Slots::DoGun(float time, int timeType, float timeC)
{
	m_CurIndex++;
	if (m_CurIndex > m_Max)
	{
		m_CurIndex = 1;
	}
	float r_timm = time;
	if (timeType == 1)
	{
		m_CurStartTime -= timeC;
		r_timm = (m_CurStartTime < MIDE_TIME) ? MIDE_TIME : m_CurStartTime;
	}else if (timeType == 3)
	{
		m_CurEndTime += timeC;
		r_timm = (m_CurEndTime < END_TIME) ? m_CurEndTime : END_TIME;
	}
	
	ImageView * donw_img = m_Images.at(3);
	std::map<int, ImageView*> temp_map;
	for (int i = 1; i <= 3;i++)
	{
		m_Images.at(i)->stopAllActions();
		int move_index = i + 1;
		move_index = move_index >= 4 ? 1 : move_index;
		if (i != 3)
		{
			MoveTo * move_to = MoveTo::create(r_timm, m_Vec2.at(move_index));
			m_Images.at(i)->runAction(move_to);
			temp_map.insert(std::make_pair(move_index, m_Images.at(i)));
		}
		else
		{
			MoveTo * move_to_1 = MoveTo::create(r_timm / 2, Vec2(donw_img->getPositionX(),donw_img->getPositionY() - donw_img->getContentSize().height/2));
			MoveTo * move_to_2 = MoveTo::create(r_timm / 2,m_Vec2.at(1));
			Sequence * seq = Sequence::create(move_to_1, CallFunc::create([=](){
				RefreshView();//刷新
				donw_img->setPosition(Vec2(m_Vec2.at(move_index).x, m_Vec2.at(move_index).y + donw_img->getContentSize().height / 2));
			}),move_to_2,NULL);
			donw_img->runAction(seq);
			temp_map.insert(std::make_pair(1, donw_img));
		}
	}
	for (int index = 1; index <= 3;index ++)
	{
		m_Images.at(index) = temp_map.at(index);
	}
	Sequence * seq = Sequence::create(DelayTime::create(r_timm), CallFunc::create([=](){
		if (m_IsForceEnter == 1 && m_CurRunType != 3){
			m_CurRunType = 3;
			m_CurEndTime = MIDE_TIME;//给动态终止时间赋值
			EndAction();
			m_IsForceEnter = 2;
		}
		else
		{
			if (m_CurRunType == 1)
			{
				StartAction();
			}
			else if (m_CurRunType == 2)
			{
				MidelAction();
			}
			else if (m_CurRunType == 3)
			{
				EndAction();
			}
		}
		
	}), NULL);
	this->runAction(seq);

}
void Slots::ForceEnterEnd(){
	if (m_IsForceEnter = 0){
		m_IsForceEnter = 1;
		//--也许要你自己计算一下最终记下移动到目标位置
	}
}
void Slots::Stop()
{
	if (m_IsStop == true)return;
	m_IsStop = true;
	if (m_ActionNodes.find(0) == m_ActionNodes.end())
	{
		m_ActionNodes.insert(std::make_pair(0, this->getActionManager()->pauseAllRunningActions()));
	}
	else
	{
		m_ActionNodes.at(0) = this->getActionManager()->pauseAllRunningActions();
	}

// 	for (int i = 1; i <= 3; i++)
// 	{
// 		if (m_ActionNodes.find(i) == m_ActionNodes.end())
// 		{
// 			m_ActionNodes.insert(std::make_pair(i, m_Images.at(i)->getActionManager()->pauseAllRunningActions()));
// 		}
// 		else
// 		{
// 			m_ActionNodes.at(i) = this->getActionManager()->pauseAllRunningActions();
// 		}
// 	}
}
void Slots::Contine()
{
	if (m_IsStop == false)return;
	m_IsStop = false;
	this->getActionManager()->resumeTargets(m_ActionNodes.at(0));
// 	for (int i = 1; i <= 3; i++)
// 	{
// 		m_Images.at(i)->getActionManager()->resumeTargets(m_ActionNodes.at(i));
// 	}
}
void Slots::InitImage(){
	for (int i = 0; i < 9;i++)
	{
		allImage.push_back(0);
	}
}
int Slots::PopImage()
{
	int randD = (CCRANDOM_0_1() * 6) + 1;
	int count = 0;
	for (int i = 0; i < 9; i++)
	{
		if (allImage[i] == 0) count++;
		if (count == randD) return (i + 1);
	}
	return 1;
}
void Slots::UseImage(int index)
{
	allImage[index - 1] = 1;
}
void Slots::UnUseImage(int index)
{
	allImage[index - 1] = 0;
}