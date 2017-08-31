#pragma once

#include "Simulation/UserInterface/UserInterface.h"

class BatteryComponent;
class ContourComponent;

class BatteryUI : public BaseUI
{
	URHO3D_OBJECT(BatteryUI, BaseUI);

public:
	BatteryUI(Context* context);
    virtual bool VOnMsgProc(AppMsg message);
	virtual bool VOnRestore();
	virtual void VSetVisible(bool visible);
	virtual bool VOnLostDevice();
	virtual void VOnUpdate(float timeStep);
protected:
	void CreateBatteryWindow();

	void HandleEnableCircuitDelegate(StringHash eventType, VariantMap& eventData);
	void HandleChangeCircuitDirectionDelegate(StringHash eventType, VariantMap& eventData);
	void HandleApplyBatteryDelegate(StringHash eventType, VariantMap& eventData);
	void HandleCloseDelegate(StringHash eventType, VariantMap& eventData);
	void ContourEnvironmentLoadedDelegate(StringHash eventType, VariantMap& eventData);
	void ShowBatteryUIDelegate(StringHash eventType, VariantMap& eventData);

	void SetBatteryUIFontSize();

protected:
	UI* m_pUI;

	bool m_bIsInitialized;

	// + and - 
	Node* m_pLeft;
	Node* m_pRight;

	SharedPtr<Window> m_pBatteryWindow;
	LineEdit* m_pEditIntensity;
	Text* m_pIntensityDirection;
	Text* m_pIntensityStatus;

	BatteryComponent* m_pBatteryComponent;
	ContourComponent* m_pContourComponent;
};
