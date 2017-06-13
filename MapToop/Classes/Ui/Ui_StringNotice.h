//
//  Ui_StringNotice.hpp
//  MakePizza
//
//  Created by xcode5 on 17/3/16.
//
//

#ifndef Ui_StringNotice_hpp
#define Ui_StringNotice_hpp

#include "Head.h"
#if (__HeadFileIncludeNum == __HeadFileHavePath)
#include "Entity/Interface_Entity.h"
#else
#include "Interface_Entity.h"
#endif//判断路径的引用
class Ui_StringNotice : public Interface_Entity
{
public:
    /*
     @构造,析构
     */
    Ui_StringNotice();
    ~Ui_StringNotice();
    /*
     @场景建立
     */
    //static Ui_StringNotice * CreateWithId(const vector<int> & _id);
    static Ui_StringNotice * CreateWithContentWithTime(const string & content);
    virtual bool init();
    CREATE_FUNC(Ui_StringNotice);
    /*
        @属性
    */
    CC_SYNTHESIZE(string, m_Content, MyContent);
    ImageView  * m_ImageBackground;//背景
    Text * m_TextContent;//文本
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
        @刷新子类的链表
     */
    void RefreshView();
};
#endif /* Ui_StringNotice_hpp */
