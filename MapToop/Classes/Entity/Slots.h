#ifndef __Slots_H__
#define __Slots_H__
#include "Config/Head.h"
class MBugBody;
class Slots : public Node
{
public:
	Slots();
	~Slots();
	/*
	@固定创建方法
	*/
	virtual bool init();
	CREATE_FUNC(Slots);
	void InitSlots();
	void RunToIndex(int index);
	bool m_IsStop;
	bool m_IsOver;
	void Stop();
	void Contine();
protected:
	std::map<int, ImageView *> m_Images;
	std::map<int, Vec2> m_Vec2;
	int m_CurIndex;//当前图片的位置
	int m_TargetIndex;
	int m_Max;//总共的图片多少个
	std::map<int, string> m_ImageName;//图片资源名字,和index对应
	int m_StartCount;//开始转几圈
	int m_MideCount;//中间转几圈
	int m_EndCount;//最后转几圈--这个是计算的，根据传入的index去计算
	int m_CurRunType;//当前的动画阶段,1，开始，2中间，3结尾
	float m_CurStartTime;
	float m_CurEndTime;
	
	
	void RefreshView();
	void StartAction();
	void MidelAction();
	void EndAction();
	void DoEnd();
	void DoGun(float time, int timeType = 1, float timeC = 0);
	int m_IsForceEnter;// 0 ,没有，1，准备强制进入，2已经进入
	void ForceEnterEnd();
	std::map<int, Vector<Node*>> m_ActionNodes;
	std::map<int, vector<int>> m_DownImgae;
	vector<int> allImage;
	void InitImage();
	int PopImage();
	void UseImage(int index);
	void UnUseImage(int index);
	bool endFlag;//
	int rand_1;//随机一个值
	int rand_2;//随机二个值
	int rand_3;//随机三个值
};
#endif // !__Slots_H__