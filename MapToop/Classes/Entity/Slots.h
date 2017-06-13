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
	@�̶���������
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
	int m_CurIndex;//��ǰͼƬ��λ��
	int m_TargetIndex;
	int m_Max;//�ܹ���ͼƬ���ٸ�
	std::map<int, string> m_ImageName;//ͼƬ��Դ����,��index��Ӧ
	int m_StartCount;//��ʼת��Ȧ
	int m_MideCount;//�м�ת��Ȧ
	int m_EndCount;//���ת��Ȧ--����Ǽ���ģ����ݴ����indexȥ����
	int m_CurRunType;//��ǰ�Ķ����׶�,1����ʼ��2�м䣬3��β
	float m_CurStartTime;
	float m_CurEndTime;
	
	
	void RefreshView();
	void StartAction();
	void MidelAction();
	void EndAction();
	void DoEnd();
	void DoGun(float time, int timeType = 1, float timeC = 0);
	int m_IsForceEnter;// 0 ,û�У�1��׼��ǿ�ƽ��룬2�Ѿ�����
	void ForceEnterEnd();
	std::map<int, Vector<Node*>> m_ActionNodes;
	std::map<int, vector<int>> m_DownImgae;
	vector<int> allImage;
	void InitImage();
	int PopImage();
	void UseImage(int index);
	void UnUseImage(int index);
	bool endFlag;//
	int rand_1;//���һ��ֵ
	int rand_2;//�������ֵ
	int rand_3;//�������ֵ
};
#endif // !__Slots_H__