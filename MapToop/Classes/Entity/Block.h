#ifndef __Block_H__
#define __Block_H__
#include "Config/Head.h"
enum BlockType
{
	Ordinary = 0,
	Sky,
	Obstacle,
	TYPEMax
};
class Block : public Layout
{
public:
	Block();
	~Block();
	/*
	@固定创建方法
	*/
	static Block * CreateWithInfo(const Size & y_size, const int & y_type);
	virtual bool init();
	void InitWithInfo(const Size & y_size, const int & y_type);
	CREATE_FUNC(Block);
	Size m_Size;
	int m_Type;
	PosInt m_Pos;
	ImageView * m_ImageBlock;//块
	Text * m_Text;
	Text * m_TextObject;
	bool m_IsComVisi;
	bool m_IsPoint;
	void SetBlockType(const int  & block_type);

	void SetPos(const PosInt & pos);

	void SetPosVisi(bool is_visi);

	void SetComVisi(bool _is);

	void IsPoint(bool _is);
protected:

private:
	void TestShader();

};
#endif // !__MAINSCENE__