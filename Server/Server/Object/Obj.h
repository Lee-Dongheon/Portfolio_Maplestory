#pragma once

#include "../Core/Ref.h"
#include "../Collider/Collider.h"

class CCollider;
class CScene;

class CObj :
	public CRef
{
protected:
	friend class CScene;

protected:
	CObj();
	CObj(const CObj& obj);
	virtual ~CObj();

protected:
	CScene* m_pScene; // ���� object�� ������ �ִ� scene

	bool m_bIsPhysics; // ����ȿ�� ���� ������ ����
	float m_fGravityTime; // �߷� ���뿡 ����ϴ� float ����

	OBJECT_TYPE m_eObjType; // ������Ʈ Ÿ��

	POSITION m_tPos; // object ��ǥ
	_SIZE m_tSize; // object ũ��
	POSITION m_tPivot; // object pivot
	MOVE_DIR m_eDir; // object�� ����

	list<CCollider*> m_ColliderList; // Object Collider List

	OBJECT_STATE m_eObjState; // object ����

	class CCriticalSectionObj* m_pCritic;

public:
	void SetScene(CScene* pScene);
	void SetScene(int iSceneNum);
	void SetPhysics(bool bPhysics) { m_bIsPhysics = bPhysics; }
	void SetObjType(OBJECT_TYPE eObjType) { m_eObjType = eObjType; }
	void SetDir(MOVE_DIR eDir) { m_eDir = eDir; }

	void SetPos(const POSITION& tPos) { m_tPos = tPos; }
	void SetPos(float x, float y) { m_tPos.x = x; m_tPos.y = y; }
	void SetSize(const _SIZE& tSize) { m_tSize = tSize; }
	void SetSize(float x, float y) { m_tSize.x = x; m_tSize.y = y; }
	void SetPivot(const POSITION& tPivot) { m_tPivot = tPivot; }
	void SetPivot(float x, float y) { m_tPivot.x = x; m_tPivot.y = y; }

	void SetObjState(OBJECT_STATE eObjState) { m_eObjState = eObjState; }

	void SetCriticEnter();
	void SetCriticLeave();

public:
	CScene* GetScene() const { return m_pScene; }
	bool GetPhysics() const { return m_bIsPhysics; }
	const list<CCollider*>* GetColliderList() const { return &m_ColliderList; }
	OBJECT_TYPE GetObjType() const { return m_eObjType; }
	MOVE_DIR GetDir() const { return m_eDir; }

	POSITION GetPos() const { return m_tPos; }
	_SIZE GetSize() const { return m_tSize; }
	POSITION GetPivot() const { return m_tPivot; }

	OBJECT_STATE GetObjState() const { return m_eObjState; }

public:
	void ClearGravity() { m_fGravityTime = 0.f; }
	bool CheckCollider() { return !m_ColliderList.empty(); }

public:
	virtual bool Init() = 0;
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual CObj* Clone() = 0;

	template<typename T>
	void AddCollisionFunction(const string& strTag, COLLISION_STATE eState, T* pObj, void(T::*pFunc)(CCollider*, CCollider*, float))
	{
		list<CCollider*>::iterator iter;
		list<CCollider*>::iterator iterEnd = m_ColliderList.end();

		for (iter = m_ColliderList.begin(); iter != iterEnd; iter++)
		{
			if ((*iter)->GetTag() == strTag)
			{
				(*iter)->AddCollisionFunction(eState, pObj, pFunc);
				break;
			}
		}
	}

	template<typename T>
	T* AddCollider(const string& strTag)
	{
		T* pCollider = new T;

		pCollider->SetObj(this);
		pCollider->SetTag(strTag);

		if (!pCollider->Init())
		{
			SAFE_RELEASE(pCollider);
			return NULL;
		}

		pCollider->AddRef();
		m_ColliderList.push_back(pCollider);

		return pCollider;
	}

	template<typename T>
	static T* CreateObj(const string& strTag, CScene* pScene = NULL)
	{
		T* pObj = new T;

		pObj->SetTag(strTag);

		if (!pObj->Init())
		{
			SAFE_DELETE(pObj);
			return NULL;
		}
		if (pScene)
		{
			pScene->AddObject(pObj);
		}

		return pObj;
	}

	//static CObj* CreateCloneObj(const string& strPrototypeKey, const string& strTag, CLayer* pLayer = NULL);
};