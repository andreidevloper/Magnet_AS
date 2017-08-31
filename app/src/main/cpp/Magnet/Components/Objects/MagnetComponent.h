#pragma once

struct MagnetData
{
	Vector3 m_MagneticField;
	bool m_Polarity;
};

class MagnetComponent : public LogicComponent
{
	URHO3D_OBJECT(MagnetComponent, LogicComponent)

public:
	MagnetComponent(Context* context);

	// Register object factory and URHO3D_ATTRIBUTEs.
	static void RegisterObject(Context* context);

	virtual void Start();

	void SetMagneticField(float magneticField) { m_MagnetData.m_MagneticField.y_ = magneticField; }
	void ChangePolarity();

	
	const MagnetData& GetMagnetData() const { return m_MagnetData; }

protected:
	MagnetData m_MagnetData;
};