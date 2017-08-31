#include "MagnetStd.h"
#include "ContourComponent.h"

#define RADIANS_TO_DEGREES(x) ((x) * 180.0 / M_PI)
#define DEGREES_TO_RADIANS(x) ((x) * M_PI / 180.0)


ContourComponent::ContourComponent(Context* context) : LogicComponent(context)
{
	m_pNode = nullptr;
	m_ContourData.m_Width = -1.0f;
	m_ContourData.m_Height = -1.0f;
	m_ContourData.m_Density = -1.0f;
	m_ContourData.m_CrossArea = -1.0f;
	m_ContourData.m_Mass = -1.0f;
	m_ContourData.m_AngularVelocity = Vector3(-1, -1, -1);
	m_ContourData.m_AngularAcceleration = Vector3(-1, -1, -1);
	m_ContourData.m_Torque = -1;
	m_ContourData.m_RotationalInertia = -1.0f;
	m_ContourData.m_AmperesForce = -1;
	m_ContourData.m_PaceOff = -1;
	m_FixedStep = FIXED_STEP_FOR_PHYSICS;
}

void ContourComponent::RegisterObject(Context* context)
{
	context->RegisterFactory<ContourComponent>();
}

void ContourComponent::Start()
{
	m_pNode = GetNode();
	m_ContourData.m_Width = 0.5f;
	m_ContourData.m_Height = 0.5f;
	m_ContourData.m_Density = 7800.0f;
	m_ContourData.m_CrossArea = 0.0001f;
	CalculateMass();
	m_ContourData.m_AngularVelocity = Vector3::ZERO;
	m_ContourData.m_AngularAcceleration = Vector3::ZERO;
	m_ContourData.m_Torque = 0;
	CalculateRotationalInertia();
	m_ContourData.m_AmperesForce = 0;
	m_ContourData.m_PaceOff = 0;
}

void ContourComponent::Update(float timeStep)
{
	if (m_ContourData.m_Mass != 0)
	{
		Vector3 eulerOrientation = node_->GetRotation().EulerAngles();
		eulerOrientation.x_ += (float)RADIANS_TO_DEGREES(m_ContourData.m_AngularVelocity.x_ * m_FixedStep);
		eulerOrientation.y_ += (float)RADIANS_TO_DEGREES(m_ContourData.m_AngularVelocity.y_ * m_FixedStep);
		eulerOrientation.z_ += (float)RADIANS_TO_DEGREES(m_ContourData.m_AngularVelocity.z_ * m_FixedStep);

		Quaternion currentOrientation = Quaternion(eulerOrientation.x_, eulerOrientation.y_, eulerOrientation.z_);

		m_pNode->SetRotation(currentOrientation);
	}
}

float ContourComponent::CalculateRotationalInertia()
{
	m_ContourData.m_RotationalInertia = m_ContourData.m_Mass / 3.0f * pow(m_ContourData.m_Height, 2) + (2 * (m_ContourData.m_Mass / 3.0f) * pow(m_ContourData.m_Width, 2)) / 3;
	return m_ContourData.m_RotationalInertia;
}

float ContourComponent::CalculateMass()
{
	m_ContourData.m_Mass = 2 * m_ContourData.m_Height * m_ContourData.m_CrossArea * m_ContourData.m_Density +
			   m_ContourData.m_Width * m_ContourData.m_CrossArea * m_ContourData.m_Density;
	return m_ContourData.m_Mass;
}

void ContourComponent::ResetContour()
{
	m_ContourData.m_AngularAcceleration = Vector3::ZERO;
	m_ContourData.m_AngularVelocity = Vector3::ZERO;
	m_ContourData.m_AmperesForce = 0;
	m_ContourData.m_PaceOff = 0;
	m_pNode->SetRotation(Quaternion::IDENTITY);
}