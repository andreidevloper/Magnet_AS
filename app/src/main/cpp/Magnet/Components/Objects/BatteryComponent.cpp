#include "MagnetStd.h"
#include "BatteryComponent.h"

BatteryComponent::BatteryComponent(Context* context) : LogicComponent(context)
{
	m_BatteryData.m_IntensityDirection = false;
	m_BatteryData.m_bIsCircuitEnabled = true;
	m_BatteryData.m_Intensity = Vector3(-1, -1, -1);
}

void BatteryComponent::RegisterObject(Context* context)
{
	context->RegisterFactory<BatteryComponent>();
}

void BatteryComponent::Start()
{
	m_BatteryData.m_IntensityDirection = true;
	m_BatteryData.m_bIsCircuitEnabled = false;

	if (m_BatteryData.m_bIsCircuitEnabled)
	{
		m_BatteryData.m_Intensity = Vector3(0, 0, -0.5f);
	}
	else
	{
		m_BatteryData.m_Intensity = Vector3::ZERO;
	}
}

void BatteryComponent::ChangeIntensityDirection()
{
	m_BatteryData.m_IntensityDirection = !m_BatteryData.m_IntensityDirection;
	if (m_BatteryData.m_Intensity.z_ <= 0)
	{
		m_BatteryData.m_Intensity.z_ = Abs(m_BatteryData.m_Intensity.z_);
	}
	else if(m_BatteryData.m_Intensity.z_ > 0)
	{
		m_BatteryData.m_Intensity.z_ = -m_BatteryData.m_Intensity.z_;
	}
}

void BatteryComponent::ResetBattery()
{
	m_BatteryData.m_bIsCircuitEnabled = false;
	m_BatteryData.m_Intensity = Vector3::ZERO;
}