#pragma once

#include "GameEngine/GameLogic/BaseGameLogic.h"

class BatteryComponent;
class ContourComponent;
class MagnetComponent;

class MagnetLogic : public BaseGameLogic
{
	URHO3D_OBJECT(MagnetLogic, BaseGameLogic)
public:
	MagnetLogic(Context *context);
	virtual ~MagnetLogic();

	virtual bool VInitialize();
	virtual void VChangeState(BaseGameState newState);
	virtual void VShutdown();

	virtual void VAddView(SharedPtr<IGameView> pView, NodeId actorId = INVALID_NODE_ID);
	virtual bool VLoadGameDelegate(String pLevelData);

	inline BatteryComponent* GetBatteryComponent() { return m_pBatteryComponent; }
	inline ContourComponent* GetContourComponent() { return m_pContourComponent; }
	inline MagnetComponent* GetMagnetComponent() { return m_pMagnetComponent; }
	inline const Vector3& GetSystemFriction() { return m_SystemFriction; }
	inline const Vector3& GetSystemGravity() { return m_Gravity; }

	inline void SetSystemGravity(Vector3 gravity) { m_Gravity = gravity; }
	inline void SetSystemFriction(Vector3 systemFriction) { m_SystemFriction = systemFriction; }

protected:
	virtual void VInitializeComponents();

protected: 
	Node* m_pContour;

	BatteryComponent* m_pBatteryComponent;
	ContourComponent* m_pContourComponent;
	MagnetComponent* m_pMagnetComponent;

	Vector3 m_SystemFriction;
	Vector3 m_Gravity;

	// For physics
	float m_FixedStep;

protected:
	void HandlePhysicsStepDelegate(StringHash eventType, VariantMap& eventData);

	float GetAmperesForce(Vector3 intensity, Vector3 magneticField, Vector3 armOfForceL1, float angle);
	float GetGravityForce(float mass, Vector3 armOfForceL2, float angle);
	Vector3 GetFriction(Vector3 friction, Vector3 angularVelocity);
};