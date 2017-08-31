#include "MagnetStd.h"

#include <Simulation/UserInterface/UserInterface.h>
#include "Events/MagnetEvents.h"
#include "MainMenuUI.h"

MainMenuUI::MainMenuUI(Context* context) : BaseUI(context)
{
	m_pUI = g_pApp->GetSubsystem<UI>();
	m_bIsInitialized = false;
	m_pMainMenu = nullptr;
	m_bIsVisible = false;

}

bool MainMenuUI::VOnRestore()
{
	if (!m_bIsInitialized)
	{
		MainMenuUI::CreateMainMenuWindow();
		return true;
	}
	else
	{
		if (m_pMainMenu)
		{
			m_pMainMenu->SetVisible(true);
		}
	}

	return false;
}

void MainMenuUI::VSetVisible(bool value)
{
	if (m_pMainMenu)
	{
		m_pMainMenu->SetVisible(value);
	}
}

bool MainMenuUI::VOnLostDevice()
{
	m_pMainMenu->SetVisible(false);
	return false;
}

void MainMenuUI::CreateMainMenuWindow()
{
	if (!m_pMainMenu)
	{

		XMLFile* file = g_pApp->GetResCache()->GetResource<XMLFile>(String("UI/MainMenuUI.xml"));
		m_pMainMenu = StaticCast<Window>(m_pUI->LoadLayout(file));

		UIElement* contourButton = m_pMainMenu->GetChild(String("Contour_Button"), true);
		SubscribeToEvent(contourButton, E_RELEASED, URHO3D_HANDLER(MainMenuUI, ContourUIHandle));

		UIElement* batteryButton = m_pMainMenu->GetChild(String("Battery_Button"), true);
		SubscribeToEvent(batteryButton, E_RELEASED, URHO3D_HANDLER(MainMenuUI, BatteryUIHandle));

		UIElement* magnetButton = m_pMainMenu->GetChild(String("Magnet_Button"), true);
		SubscribeToEvent(magnetButton, E_RELEASED, URHO3D_HANDLER(MainMenuUI, MagnetUIHandle));

        float textFontSize = LayoutUtility::GetTextFontSize();
        IntVector2 windowSize = LayoutUtility::GetMainMenuWindowSize();
		m_pMainMenu->SetSize(windowSize);

		int buttonHeight = windowSize.y_ / 3.0f;
		contourButton->SetSize(windowSize.x_, buttonHeight);
		batteryButton->SetSize(windowSize.x_, buttonHeight);
		magnetButton->SetSize(windowSize.x_, buttonHeight);

        Text* contourButtonText = (Text*)m_pMainMenu->GetChild(String("Contour_Text"), true);
        contourButtonText->SetFontSize(textFontSize);

        Text* batteryButtonText = (Text*)m_pMainMenu->GetChild(String("Battery_Text"), true);
        batteryButtonText->SetFontSize(textFontSize);

        Text* magnetButtonText = (Text*)m_pMainMenu->GetChild(String("Magnet_Text"), true);
        magnetButtonText->SetFontSize(textFontSize);

		m_pMainMenu->SetVisible(true);

		m_pUI->GetRoot()->AddChild(m_pMainMenu);

        m_bIsInitialized = true;
	}
}

void MainMenuUI::ContourUIHandle(StringHash eventType, VariantMap& eventData)
{
	SendEvent(EVENT_DATA_SHOW_CONTOUR_UI);
}
void MainMenuUI::BatteryUIHandle(StringHash eventType, VariantMap& eventData)
{
	SendEvent(EVENT_DATA_SHOW_BATTERY_UI);
}

void MainMenuUI::MagnetUIHandle(StringHash eventType, VariantMap& eventData)
{
	SendEvent(EVENT_DATA_SHOW_MAGNET_UI);
}
