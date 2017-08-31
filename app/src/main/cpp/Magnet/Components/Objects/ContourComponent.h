#pragma once

#define FIXED_STEP_FOR_PHYSICS 0.0166666675f;

struct ContourData
{
	Vector3 m_AngularVelocity;
	Vector3 m_AngularAcceleration;
	float m_Width;
	float m_Height;
	float m_Density;
	float m_CrossArea;
	float m_Torque;
	float m_RotationalInertia;
	float m_AmperesForce;
	float m_Mass;
	float m_PaceOff;
};

class ContourComponent : public LogicComponent
{
	URHO3D_OBJECT(ContourComponent, LogicComponent)

public:
	ContourComponent(Context* context);

	// Register object factory and URHO3D_ATTRIBUTEs.
	static void RegisterObject(Context* context);
	virtual void Start();

	// Handle physics world update. Called by LogicComponent base class.
	virtual void Update(float timeStep);

	void ResetContour();

	void SetWidth(float width) { m_ContourData.m_Width = width; }
	void SetHeight(float height) { m_ContourData.m_Height = height; }
	void SetDensity(float density) { m_ContourData.m_Density = density; }
	void SetCrossArea(float crossArea) { m_ContourData.m_CrossArea = crossArea; }
	void SetRotationalInertia(float rotationalInertia) { m_ContourData.m_RotationalInertia = rotationalInertia; }
	float CalculateMass();
	float CalculateRotationalInertia();

	void SetAngularVelocity(Vector3 vel) { m_ContourData.m_AngularVelocity = vel; }
	void SetAngularAcceleration(Vector3 acc) { m_ContourData.m_AngularAcceleration = acc; }
	void SetAmpereForce(float force) { m_ContourData.m_AmperesForce = force; }
	void SetPaceOff(float a) { m_ContourData.m_PaceOff = a; }
	void SetTorque(float torque) { m_ContourData.m_Torque = torque; }
	const ContourData& GetCountourData() const { return m_ContourData; }

protected:
	
	
protected:
	ContourData m_ContourData;
	Node* m_pNode;
	float m_FixedStep;
};