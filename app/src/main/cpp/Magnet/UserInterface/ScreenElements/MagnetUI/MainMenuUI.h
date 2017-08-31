#pragma once

#include "GameEngine/UserInterface/UserInterface.h"

class BatteryComponent;
class ContourComponent;

class MainMenuUI : public BaseUI
{
	URHO3D_OBJECT(MainMenuUI, BaseUI);

public:
	MainMenuUI(Context* context);
	virtual bool VOnRestore();
	virtual void VSetVisible(bool visible);
	virtual bool VOnLostDevice();
protected:
	void CreateMainMenuWindow();

	void ContourUIHandle(StringHash eventType, VariantMap& eventData);
	void BatteryUIHandle(StringHash eventType, VariantMap& eventData);
	void MagnetUIHandle(StringHash eventType, VariantMap& eventData);

protected:
	UI* m_pUI;

	bool m_bIsInitialized;

	SharedPtr<Window> m_pMainMenu;
};
