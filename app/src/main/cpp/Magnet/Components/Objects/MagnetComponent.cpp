#include "MagnetStd.h"
#include "MagnetComponent.h"

MagnetComponent::MagnetComponent(Context* context) : LogicComponent(context)
{
	m_MagnetData.m_MagneticField = Vector3(-1, -1, -1);
	m_MagnetData.m_Polarity = false;
}

// Register object factory and URHO3D_ATTRIBUTEs.
void MagnetComponent::RegisterObject(Context* context)
{
	context->RegisterFactory<MagnetComponent>();
}

void MagnetComponent::Start()
{
	m_MagnetData.m_MagneticField = Vector3(0, 1.0f, 0);
	m_MagnetData.m_Polarity = true;
}

void MagnetComponent::ChangePolarity()
{
	m_MagnetData.m_Polarity = !m_MagnetData.m_Polarity;
	m_MagnetData.m_MagneticField = -m_MagnetData.m_MagneticField;
}
