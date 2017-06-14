#ifndef _RoleModel_H_
#define _RoleModel_H_
#include "Config/Head.h"
/*
	@dir 1.上,2.下，3.左，4.右
*/
class Block;
class EffectArmature;
class MapBase;
class Behaviour;
class CfgAnim;
class EffectSpine;
class RoleModel : public Layout
{
public:
	enum EyeAngleType {
		Eye_Sky = 1,
		Eye_2D  = 2,
	};
public:
	RoleModel();
	~RoleModel();
	static RoleModel * Create(int roleId, EyeAngleType eyeType = EyeAngleType::Eye_2D);
	static RoleModel * CreatePlayer();
	virtual bool init(int roleId, EyeAngleType eyeType = EyeAngleType::Eye_2D);
	virtual void SetId(int id);
	virtual void SetModelId(int modelId);
	virtual void SetBehaviour(vector<int> BehaviourId);
	virtual void ClearAllBehaviour();
	virtual void UpdateBehaviour();
	void UpdateView();
	void UpdataBehaviour();
	void RunAnimAction(ActionType actionType);
public://我懒了，直接访问吧
	bool m_IsTouchNpc;
protected:
	Layout * m_LayoutModel;
	Layout * m_LayoutContent;
	EffectArmature * m_Model;
	MapBase * m_MyMap;
	EffectSpine * m_SpineModel;
	int m_Id;
	int m_ModelId;
	float m_Speed;
	vector<PosInt> m_RunPath;
	vector<PosInt> m_RunPathCache;
	vector<int> m_CanNotTypes;
	Action * m_RunAction;
	PosInt m_Pos;
	DirType m_Dir;
	bool m_IsCanMove;
	bool m_IsTouchNpcCache;
	ModelType m_ModelType;
	EyeAngleType m_EysType;
	PhysicsBody * m_PhysicsBody;
	ActionType m_ActionType;
public:
	virtual void SetMapDelegate(MapBase * mapBase);
	virtual void RunWithPath(vector<PosInt>  path,bool isTouchNpc = false);
	virtual void AddBehaviour(Behaviour * haviour);
	virtual void SetIsCanMove(bool is);
	void SetPos(int posX,int posY);
	void SetPosForEditor(int posX, int posY);
	PosInt GetPos();
	PosInt GetRunEndPos();
	MapBase * GetMyMap();
	DirType GetDir();
	ModelType GetModelType();
	vector<int> GetMyCanNotMoveNodeTypes();
	int GetId();
	void SetDir(DirType dir);
	void RefreshAnimWithDir(DirType dir);
	bool IsRuning();
protected:
	virtual void InitRoleWidget();
	virtual void SetPos(PosInt pos);
private:
	void RefreshAnim(const CfgAnim * cfg);
	void RefreshSpineAnim(const CfgSpineAnim * cfg);
	void Adaptive();
	void RunPath();
	void RunEnd();
	virtual void update(float delta);
	void ActiveBehaviour(float delta);//单线程行为逻辑
	void ExecuteBehaviour(float delta);//多线程行为逻辑
public://一些行为函数
	vector<Behaviour *> m_Behaviour;
	Behaviour * m_CurBehaviour;
	bool m_BehaviourCanDo;
};





#endif // ! __RoleModel_H_