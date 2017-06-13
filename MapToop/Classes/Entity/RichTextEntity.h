#ifndef __RichTextEntity_H__
#define __RichTextEntity_H__
#include "Config/Head.h"

enum RichType
{
	RichType_Text = 1,
	RichType_Image,
	RichType_Null = 9999,
};
class RichNodeData
{
public:
	RichNodeData();
	~RichNodeData();
	string m_Content;
	Color3B m_Color;
	GLubyte m_Opacity;
	int m_FontSize;
	string m_FontName;
	RichType m_Type;
	string m_ImagePath;
private:

};

/*
	@{# 开始  #}结束
	@必须要成对出现,中间的为内容
	@';'为分隔符，如果要文本为';'则可以';;'2个该分隔符即可
	@实例{#color=255,0,255;FontSize=35;a;;bc#}
	@关键字有：color,type,FontSize,FontName,Opacity,ImagePath，
*/

class RichTextEntity : public Layout
{
public:
	RichTextEntity();
	~RichTextEntity();
	/*
	@固定创建方法
	*/
	static RichTextEntity * CreateWithContent(string content = "",Size size = Size(0,0));
	virtual bool init();
	bool initWithData(string content = "", Size size = Size(0, 0));
	CREATE_FUNC(RichTextEntity);
	void RefrshView();
	void SetContent(string content);
protected:
	Color3B StrConverC3b(string str);
private:
	void AnalysisContent();
	RichNodeData SplitContent(const string & content, const string & splitTag);
	void AnalysisDecorate(RichNodeData & richData, const string & decoradte);
	void PushRichNodeData(RichNodeData & richData);
	void SetContentSize(const Size& contentSize);
	virtual void onEnter();
	virtual void onExit();
	string m_Content;
	Size m_Size;
	bool m_IsCanRefresh;
	std::map<int, RichNodeData> m_AllNodeData;

};
#endif // !__RichTextEntity_H__