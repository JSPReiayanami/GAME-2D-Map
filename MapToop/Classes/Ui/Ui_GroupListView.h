#ifndef __Ui_GroupListView_H__
#define __Ui_GroupListView_H__
#include "Head.h"
#if (__HeadFileIncludeNum == __HeadFileHavePath)
#include "Entity/Interface_Entity.h"
#else
#include "Interface_Entity.h"
#endif//判断路径的引用
class Ui_GroupListView : public Interface_Entity
{
public:
    /*
     @构造,析构
     */
    Ui_GroupListView();
    ~Ui_GroupListView();
    /*
     @场景建立
     */
    static Ui_GroupListView * CreateWithId(const vector<int> & _id);
    virtual bool init();
    CREATE_FUNC(Ui_GroupListView);
    vector<int> m_KindId;
    map<int,int> m_KindType;
    ListView * m_ListKind;
    ListView * m_ListChild;
    Button * m_KindDown;
    Button * m_KindUp;
    Button * m_ChildDown;
    Button * m_ChildUp;
    int m_CurChoiceType;
    /*
     @设置自己的名字
     */
    void SetInterfaceName();
    /*
     @初始化资源
     */
    void InitSrcLayout();
    /*
     @初始化按钮的回调事件
     */
    void InitButtonEvent();
    /*
     @按钮的回调事件
     */
    void ButtonAboutBaseEvent(Ref * pSender, ui::Widget::TouchEventType eventType);
    /*
        @初始化数据
    */
    void InitData();
    /*
        @初始化显示
    */
    void InitView();
    /*
     @显示对应种类的物品
     */
    void ShowKindTypeChild(const int & type);
    /*
        @设置种类ID组合
     */
    void SetKindId(const vector<int> & _id);
    /*
        @父类点击事件获取
     */
    void ItemKindSelectEvent(Ref *pSender, ListView::EventType type);
    /*
        @父类链表的滑动事件
     */
    void ItemKindEventScrollView(Ref* pSender, ui::ScrollView::EventType type);
    /*
        @子类点击事件获取
     */
    void ItemChildSelectEvent(Ref *pSender, ListView::EventType type);
    /*
        @子类链表的滑动事件
     */
    void ItemChildEventScrollView(Ref* pSender, ui::ScrollView::EventType type);
    /*
        @刷新子类的链表
     */
    void RefreshChildListView();
};
#endif // !__MAINSCENE__