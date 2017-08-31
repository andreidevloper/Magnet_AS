#pragma once

#include "GameEngine/UserInterface/UserInterface.h"

class MagnetComponent;

class MagnetUI : public BaseUI
{
	URHO3D_OBJECT(MagnetUI, BaseUI);

public:
	MagnetUI(Context* context);
	virtual bool VOnRestore();
	virtual void VSetVisible(bool visible);
	virtual bool VOnLostDevice();

protected:
	void HandleApplyMagnetDelegate(StringHash eventType, VariantMap& eventData);
	void HandleChangePolarityDelegate(StringHash eventType, VariantMap& eventData);
	void HandleCloseDelegate(StringHash eventType, VariantMap& eventData);
	void ContourEnvironmentLoadedDelegate(StringHash eventType, VariantMap& eventData);
	void ShowMagnetUIDelegate(StringHash eventType, VariantMap& eventData);
	void SetMagnetUIFontSize();
	\
	void CreateMagnetWindow();

protected:
	UI* m_pUI;
	bool m_bIsInitialized;

	MagnetComponent* m_pMagnetComponent;

	// Magnet
	SharedPtr<Window> m_pMagnetWindow;
	LineEdit* m_pEditMagneticField;
	Text* m_pTopPolarity;
	Text* m_pBottomPolarity;

	// Poluses
	Node* m_pNorth;
	Node* m_pSouth;
};
