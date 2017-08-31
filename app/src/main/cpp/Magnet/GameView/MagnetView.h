#pragma once

#include "Simulation/UserInterface/HumanView/HumanView.h"

class MagnetUI;
class BatteryUI;
class ContourUI;
class MovementController;

class MagnetView : public HumanView
{
	URHO3D_OBJECT(MagnetView, HumanView)
public:
	MagnetView(Context *context, Renderer* renderer);

	virtual bool VOnRestore();
	virtual void VOnUpdate(float timeStep);
	virtual void VShutdown();

	// Handle input message from devices
	virtual bool VOnMsgProc(AppMsg message);

	// Loading game environment.
	virtual bool VLoadGameDelegate(SharedPtr<File> level);

	// Get type from game view. Can be different, depend on realization.
	virtual GameViewType VGetType() { return GameView_Human; }

protected:
	void ContourEnvironmentCreated(StringHash eventType, VariantMap& eventData);

protected:
	SharedPtr<MovementController> m_pFreeCameraController;
	DebugRenderer* m_pDebugRenderer;
	bool m_bIsMouseVisible;

	SharedPtr<IScreenElement> m_pMagnetUI;
	SharedPtr<IScreenElement> m_pBatteryUI;
	SharedPtr<IScreenElement> m_pContourUI;
	SharedPtr<IScreenElement> m_pMainMenuUI;
};
