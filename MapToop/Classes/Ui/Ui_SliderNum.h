#ifndef __Ui_SliderNum_H__
#define __Ui_SliderNum_H__
#include "Config/Head.h"
#include "Entity/Interface_Entity.h"
class Controller_Square;
class Ui_SliderNum : public Interface_Entity
{
public:
	Ui_SliderNum();
	~Ui_SliderNum();
	CREATE_FUNC(Ui_SliderNum);
	/*
		@�̶���������
	*/
	virtual bool init();

	/*
		@���ڵ�
	*/
	Layout * m_Root;//���ڵ�
	string   m_LayerSrc;//������Դ
	ImageView * m_ImageBackground;//����
	Controller_Square * m_ControSquare;//������
	Button * m_ButtonChoice;//ѡ��
	Text   * m_TextTop;
	Text   * m_TextMiddle;
	Text   * m_TextDown;
	int      m_MinNum;//��С��
	int      m_MaxNum;//����
	bool     m_IsNumChangeOver;
	bool     m_IsTouched;//�Ƿ񱻵����
	map<int , Vec2> m_TextVec2;
	map<int, int>  m_Num;
	float    m_MoveTime;
	
	SEL_CallFunc_IntAndNode m_OutFunction;
	Ref * m_OutFunctioner;
	/*
	@�����Լ�������
	*/
	void SetInterfaceName();
	/*
	@��ʼ����Դ
	*/
	void InitSrcLayout();
	/*
	@��ʼ���ؼ�
	*/
	void InitWidget();
	/*
	@��ʼ����ť�Ļص��¼�
	*/
	void InitButtonEvent();
	/*
		@�����ķ���
	*/
	void TouchSquare(Square_DirType s_type);
	/*
		@ѡ���¼�
	*/
	void TouchedEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
	/*
		@���µ��ƶ�
	*/
	void RunUp();
	void RunDown();
	/*
		@��������
	*/
	string CalculateNumIsAdd(string  str_num,bool _is);
	/*
		@��ȡ��ǰ������
	*/
	int GetCurViewNum();
	/*
		@ע�����ָı�ص�
	*/
	void RegisterNumChangeCallback(SEL_CallFunc_IntAndNode callback, Ref * register_user);
	/*
		@�����ⲿ����
	*/
	void CallOutFunction();
	/*
		@�ı����
	*/
	void ChanageBackground(const string & src_back, const int & text_size = -1);
	/*
		@��������
		@�رջ���
	*/
	void OpenSlider();
	void CloseSlider();
};
#endif // !__Ui_BagItem_H__