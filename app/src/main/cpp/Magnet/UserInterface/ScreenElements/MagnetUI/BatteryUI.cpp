#include "MagnetStd.h"

#include <Simulation/UserInterface/UserInterface.h>
#include <Simulation/GameLogic/BaseGameLogic.h>
#include <Simulation/UserInterface/LayoutUtility.h>

#include "GameLogic/MagnetLogic.h"
#include "Events/MagnetEvents.h"

#include "Components/Objects/BatteryComponent.h"
#include "Components/Objects/ContourComponent.h"

#include "BatteryUI.h"

BatteryUI::BatteryUI(Context* context) : BaseUI(context)
{
	m_pUI = g_pApp->GetSubsystem<UI>();
	m_bIsInitialized = false;
	m_pBatteryWindow = nullptr;
	m_pEditIntensity = nullptr;
	m_pIntensityDirection = nullptr;
	m_pIntensityStatus = nullptr;
	
	m_bIsVisible = false;

	SubscribeToEvent(EVENT_DATA_MAGNET_APP_ENVIRONMENT_CREATED, URHO3D_HANDLER(BatteryUI, ContourEnvironmentLoadedDelegate));
	m_pBatteryComponent = nullptr;
	m_pContourComponent = nullptr;
	m_pLeft = nullptr;
	m_pRight = nullptr;
    SubscribeToEvent(EVENT_DATA_SHOW_BATTERY_UI, URHO3D_HANDLER(BatteryUI, ShowBatteryUIDelegate));
}

bool BatteryUI::VOnRestore()
{
	if (!m_bIsInitialized)
	{
		BatteryUI::CreateBatteryWindow();
		m_bIsInitialized = true;
		return true;
	}
	else
	{
		if (m_pBatteryWindow)
		{
			m_pBatteryWindow->SetVisible(true);
		}
	}

	return false;
}

void BatteryUI::VSetVisible(bool value)
{
	if (m_pBatteryWindow)
	{
		m_pBatteryWindow->SetVisible(value);
	}
}

bool BatteryUI::VOnLostDevice()
{
	m_pBatteryWindow->SetVisible(false);
	return false;
}


bool BatteryUI::VOnMsgProc(AppMsg message)
{

}

void BatteryUI::VOnUpdate(float timeStep)
{
	const BatteryData& batteryData = m_pBatteryComponent->GetBatteryData();
	if (m_bIsInitialized)
	{

	}
}


void BatteryUI::CreateBatteryWindow()
{
	if (!m_pBatteryWindow)
	{
		XMLFile* file = g_pApp->GetResCache()->GetResource<XMLFile>(String("UI/Battery_UI.xml"));
		m_pBatteryWindow = StaticCast<Window>(m_pUI->LoadLayout(file));

		UIElement* closeButton = m_pBatteryWindow->GetChild(String("Button_Battery_Close"), true);
		SubscribeToEvent(closeButton, E_RELEASED, URHO3D_HANDLER(BatteryUI, HandleCloseDelegate));

		UIElement* applyButton = m_pBatteryWindow->GetChild(String("Button_Battery_Apply"), true);
		SubscribeToEvent(applyButton, E_RELEASED, URHO3D_HANDLER(BatteryUI, HandleApplyBatteryDelegate));

		UIElement* enableCircuitButton = m_pBatteryWindow->GetChild(String("Button_Battery_Enable_Circuit"), true);
		SubscribeToEvent(enableCircuitButton, E_RELEASED, URHO3D_HANDLER(BatteryUI, HandleEnableCircuitDelegate));
        SubscribeToEvent(EVENT_DATA_CIRCUIT_TOGGLE, URHO3D_HANDLER(BatteryUI, HandleEnableCircuitDelegate));

		UIElement* changeDirectionButton = m_pBatteryWindow->GetChild(String("Button_Battery_Change_Intensity_Direction"), true);
		SubscribeToEvent(changeDirectionButton, E_RELEASED, URHO3D_HANDLER(BatteryUI, HandleChangeCircuitDirectionDelegate));
        SubscribeToEvent(EVENT_DATA_CIRCUIT_CHANGE_DIRECTION, URHO3D_HANDLER(BatteryUI, HandleChangeCircuitDirectionDelegate));

		m_pEditIntensity = (LineEdit*)m_pBatteryWindow->GetChild(String("Edit_Battery_Intensity"), true);
		m_pIntensityStatus = (Text*)m_pBatteryWindow->GetChild(String("Text_Battery_Intensity_Status_Value"), true);
		m_pIntensityDirection = (Text*)m_pBatteryWindow->GetChild(String("Text_Battery_Intensity_Direction_Value"), true);

		SetBatteryUIFontSize();

        m_pBatteryWindow->SetVisible(false);

		m_pUI->GetRoot()->AddChild(m_pBatteryWindow);
	}
}


void BatteryUI::SetBatteryUIFontSize()
{
	float textFontSize = LayoutUtility::GetTextFontSize();
	float lineEditFontSize = LayoutUtility::GetLineEditFontSize();

    Text* batteryOption = (Text*)m_pBatteryWindow->GetChild(String("Text_Battery_Option"), true);
    batteryOption->SetFontSize(lineEditFontSize);

    Text* batteryIntensity = (Text*)m_pBatteryWindow->GetChild(String("Text_Battery_Intensity"), true);
	batteryIntensity->SetFontSize(textFontSize);

	Text* batteryIntensityDir = (Text*)m_pBatteryWindow->GetChild(String("Text_Battery_Intensity_Direction"), true);
	batteryIntensityDir->SetFontSize(textFontSize);

	Text* batteryIntensityStatus = (Text*)m_pBatteryWindow->GetChild(String("Text_Battery_Intensity_Status"), true);
	batteryIntensityStatus->SetFontSize(textFontSize);

	Text* offOnCircuitText = (Text*)m_pBatteryWindow->GetChild(String("Button_Battery_Enable_Circuit_Text"), true);
	offOnCircuitText->SetFontSize(textFontSize);

	Text* closeButtonText = (Text*)m_pBatteryWindow->GetChild(String("Button_Battery_Close_Text"), true);
	closeButtonText->SetFontSize(textFontSize);

	Text* applyButtonText = (Text*)m_pBatteryWindow->GetChild(String("Button_Battery_Apply_Text"), true);
	applyButtonText->SetFontSize(textFontSize);

	m_pEditIntensity->GetTextElement()->SetFontSize(lineEditFontSize);
	m_pIntensityStatus->SetFontSize(lineEditFontSize);
	m_pIntensityDirection->SetFontSize(lineEditFontSize);
}

void BatteryUI::HandleEnableCircuitDelegate(StringHash eventType, VariantMap& eventData)
{
	const BatteryData& batteryData = m_pBatteryComponent->GetBatteryData();
	m_pBatteryComponent->SetCircuitEnabled(!batteryData.m_bIsCircuitEnabled);

	if (batteryData.m_bIsCircuitEnabled)
	{
		m_pIntensityStatus->SetText("ON");
		m_pBatteryComponent->SetIntensity(ToFloat(m_pEditIntensity->GetText()));
	}
	else
	{
		m_pIntensityStatus->SetText("OFF");
		m_pBatteryComponent->SetIntensity(0.0f);
	}
}


void BatteryUI::HandleChangeCircuitDirectionDelegate(StringHash eventType, VariantMap& eventData)
{
	const BatteryData& batteryData = m_pBatteryComponent->GetBatteryData();
	m_pBatteryComponent->ChangeIntensityDirection();

	if (batteryData.m_IntensityDirection)
	{
		m_pIntensityDirection->SetText(String("CCW"));
		m_pLeft->GetComponent<Text3D>()->SetText("+");
		m_pRight->GetComponent<Text3D>()->SetText("-");
	}
	else
	{
		m_pIntensityDirection->SetText(String("CW"));
		m_pLeft->GetComponent<Text3D>()->SetText("-");
		m_pRight->GetComponent<Text3D>()->SetText("+");
	}

	m_pEditIntensity->SetText(String(batteryData.m_Intensity.z_));
}

void BatteryUI::HandleApplyBatteryDelegate(StringHash eventType, VariantMap& eventData)
{
	m_pBatteryComponent->SetIntensity(ToFloat(m_pEditIntensity->GetText()));
}

void BatteryUI::HandleCloseDelegate(StringHash eventType, VariantMap& eventData)
{
	m_pBatteryWindow->SetVisible(false);
}

void BatteryUI::ShowBatteryUIDelegate(StringHash eventType, VariantMap & eventData)
{
    if(m_pBatteryWindow)
    {
        m_pBatteryWindow->SetVisible(true);
    }
}

void BatteryUI::ContourEnvironmentLoadedDelegate(StringHash eventType, VariantMap & eventData)
{
	Scene* pScene = static_cast<Scene*>(eventData[CONTOUR_ENVIRONMENT_CREATED::P_SCENE].GetPtr());
	
	if (pScene)
	{
		m_pLeft = pScene->GetChild("Text_Battery_Left_Polus", true);
		m_pRight = pScene->GetChild("Text_Battery_Right_Polus", true);

		if (m_pEditIntensity && m_pIntensityStatus)
		{
			MagnetLogic* pMagnetLogic = static_cast<MagnetLogic*>(eventData[CONTOUR_ENVIRONMENT_CREATED::P_CONTOUR_LOGIC].GetPtr());
			m_pBatteryComponent = pMagnetLogic->GetBatteryComponent();
			m_pContourComponent = pMagnetLogic->GetContourComponent();
			const BatteryData& batteryData = m_pBatteryComponent->GetBatteryData();

			if (batteryData.m_IntensityDirection)
			{
				m_pIntensityDirection->SetText(String("CW"));
			}
			else
			{
				m_pIntensityDirection->SetText(String("CCW"));
			}

			m_pEditIntensity->SetText(String(Abs(batteryData.m_Intensity.z_)));
			if (batteryData.m_bIsCircuitEnabled)
			{
				m_pIntensityStatus->SetText("ON");
			}
			else
			{
				m_pIntensityStatus->SetText("OFF");
			}
		}
	}
	else
	{
		URHO3D_LOGERROR("Failed to get scene in BatteryUI::ContourEnvironmentLoadedDelegate");
	}
}
