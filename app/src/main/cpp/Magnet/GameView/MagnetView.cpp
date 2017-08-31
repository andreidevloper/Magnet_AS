#include "MagnetStd.h"

#include "Events/MagnetEvents.h"
#include "UserInterface/ScreenElements/MagnetUI/MagnetUI.h"
#include "UserInterface/ScreenElements/MagnetUI/BatteryUI.h"
#include "UserInterface/ScreenElements/MagnetUI/ContourUI.h"
#include "UserInterface/ScreenElements/MagnetUI/MainMenuUI.h"
#include "Simulation/Controllers/MovementController.h"

#include "MagnetView.h"

MagnetView::MagnetView(Context *context, Renderer* renderer) : HumanView(context, renderer)
{
	m_pDebugRenderer = nullptr;
	m_bIsMouseVisible = true;
	SubscribeToEvent(EVENT_DATA_MAGNET_APP_ENVIRONMENT_CREATED, URHO3D_HANDLER(MagnetView, ContourEnvironmentCreated));
}


bool MagnetView::VOnRestore()
{
	if (!HumanView::VOnRestore())
	{
		return false;
	}

	return true;
}

void MagnetView::VOnUpdate(float timeStep)
{
	HumanView::VOnUpdate(timeStep);

	if (m_pFreeCameraController)
	{
		m_pFreeCameraController->OnUpdate(timeStep);
	}

}

void MagnetView::VShutdown()
{
	HumanView::VShutdown();
	m_pMagnetUI->VOnShutdown();
	m_pBatteryUI->VOnShutdown();
	m_pContourUI->VOnShutdown();
	m_pMagnetUI->VOnShutdown();
}

bool MagnetView::VOnMsgProc(AppMsg message)
{
	HumanView::VOnMsgProc(message);

    if(message.uEvent == APP_EVENT::KEY_DOWN)
    {
        if(message.lParam.m_keycode == KEY_O)
        {
            SendEvent(EVENT_DATA_CIRCUIT_TOGGLE);
        }
        else if(message.lParam.m_keycode == KEY_P)
        {
            SendEvent(EVENT_DATA_CIRCUIT_CHANGE_DIRECTION);
        }
    }

	return true;
}

bool MagnetView::VLoadGameDelegate(SharedPtr<File> level)
{
	HumanView::VLoadGameDelegate(level);

	m_pDebugRenderer = m_pScene->GetComponent<DebugRenderer>();

	m_pMagnetUI = SharedPtr<IScreenElement>(new MagnetUI(context_));
	m_pBatteryUI = SharedPtr<IScreenElement>(new BatteryUI(context_));
	m_pContourUI = SharedPtr<IScreenElement>(new ContourUI(context_));
	m_pMainMenuUI = SharedPtr<IScreenElement>(new MainMenuUI(context_));

	m_pMainMenuUI->VOnRestore();

	VPushElement(m_pMagnetUI);
	VPushElement(m_pBatteryUI);
	VPushElement(m_pContourUI);
	VPushElement(m_pMainMenuUI);

	// A movement controller is going to control the camera, 
	// but it could be constructed with any of the objects you see in this
	// function. You can have your very own remote controlled sphere. What fun...
	m_pFreeCameraController.Reset();
	Quaternion quat = m_pControlledNode->GetRotation();

	m_pFreeCameraController = SharedPtr<MovementController>(new MovementController(context_, SharedPtr<Node>(m_pControlledNode), quat.YawAngle(), quat.PitchAngle()));

	m_pKeyboardHandler = DynamicCast<IKeyboardHandler>(m_pFreeCameraController);
	m_pPointerHandler = DynamicCast<IPointerHandler>(m_pFreeCameraController);
	return true;
}

void MagnetView::ContourEnvironmentCreated(StringHash eventType, VariantMap & eventData)
{
	m_pMagnetUI->VOnRestore();
	m_pBatteryUI->VOnRestore();
	m_pContourUI->VOnRestore();
}

