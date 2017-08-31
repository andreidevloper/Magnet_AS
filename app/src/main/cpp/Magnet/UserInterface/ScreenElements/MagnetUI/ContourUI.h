#pragma once

#include "Simulation/UserInterface/UserInterface.h"

class BatteryComponent;
class ContourComponent;
class MagnetLogic;

class ContourUI : public BaseUI
{
	URHO3D_OBJECT(ContourUI, BaseUI);

public:
	ContourUI(Context* context);
	virtual bool VOnRestore();
	virtual void VSetVisible(bool visible);
	virtual void VOnUpdate(float timeStep);
	virtual bool VOnLostDevice();

protected:
	void CreateContourWindow();
	void HandleApplyContourDelegate(StringHash eventType, VariantMap& eventData);
	void HandleResetContourDelegate(StringHash eventType, VariantMap& eventData);
	void HandleCloseDelegate(StringHash eventType, VariantMap& eventData);
	void ContourEnvironmentLoadedDelegate(StringHash eventType, VariantMap& eventData);
	void ShowContourUIDelegate(StringHash eventType, VariantMap& eventData);
	void SetContourUIFontSize();

protected:
	UI* m_pUI;

    MagnetLogic* m_pMagnetLogic;
	BatteryComponent* m_pBatteryComponent;
	ContourComponent* m_pContourComponent;

	bool m_bIsInitialized;

	SharedPtr<Window> m_pContourWindow;
	LineEdit* m_pEditHeight;
	LineEdit* m_pEditWidth;
	LineEdit* m_pEditDensity;
	LineEdit* m_pEditArea;
	Text* m_pMass;
	LineEdit* m_pEditRotationalInertia;
	LineEdit* m_pEditSystemFriction;
	LineEdit* m_pEditGravity;
	Text* m_pAmperesForce;
	Text* m_pAngularAcceleration;
	Text* m_pAngularVelocity;
	Text* m_pTorque;
	Text* m_pacoeff;
};
