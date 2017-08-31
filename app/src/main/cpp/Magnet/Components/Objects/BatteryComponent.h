#pragma once

struct BatteryData
{
	Vector3 m_Intensity;
	bool m_bIsCircuitEnabled;
	bool m_IntensityDirection;
};

class BatteryComponent : public LogicComponent
{
	URHO3D_OBJECT(BatteryComponent, LogicComponent)
public:
	BatteryComponent(Context* context);

	// Register object factory and URHO3D_ATTRIBUTEs.
	static void RegisterObject(Context* context);
	virtual void Start();

	void SetIntensity(float intensity) { m_BatteryData.m_Intensity.z_ = intensity; }
	void SetCircuitEnabled(bool value) { m_BatteryData.m_bIsCircuitEnabled = value; }
	void ChangeIntensityDirection(); 

	const BatteryData& GetBatteryData() const { return m_BatteryData; }

	void ResetBattery();

protected:
	BatteryData m_BatteryData;

};