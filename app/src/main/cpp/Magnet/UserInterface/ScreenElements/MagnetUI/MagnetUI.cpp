#include "MagnetStd.h"

#include <Simulation/UserInterface/UserInterface.h>
#include <Simulation/GameLogic/BaseGameLogic.h>

#include "Events/MagnetEvents.h"

#include "GameLogic/MagnetLogic.h"

#include "Components/Objects/MagnetComponent.h"

#include "MagnetUI.h"

MagnetUI::MagnetUI(Context* context) : BaseUI(context)
{
	m_pUI = g_pApp->GetSubsystem<UI>();
	m_bIsInitialized = false;

	m_pMagnetWindow = nullptr;
	m_pEditMagneticField = nullptr;
	m_pTopPolarity = nullptr;
	m_pBottomPolarity = nullptr;

	m_pNorth = nullptr;
	m_pSouth = nullptr;

	m_bIsVisible = false;

	SubscribeToEvent(EVENT_DATA_MAGNET_APP_ENVIRONMENT_CREATED, URHO3D_HANDLER(MagnetUI, ContourEnvironmentLoadedDelegate));
	m_pMagnetComponent = nullptr;
    SubscribeToEvent(EVENT_DATA_SHOW_MAGNET_UI, URHO3D_HANDLER(MagnetUI, ShowMagnetUIDelegate));
}

bool MagnetUI::VOnRestore()
{
	if (!m_bIsInitialized)
	{
		MagnetUI::CreateMagnetWindow();
		return true;
	}
	else
	{
		if (m_pMagnetWindow)
		{
			m_pMagnetWindow->SetVisible(true);
		}
	}

	return false;
}

bool MagnetUI::VOnLostDevice()
{
	m_pMagnetWindow->SetVisible(false);
	return false;
}


void MagnetUI::VSetVisible(bool value)
{
	if (m_pMagnetWindow)
	{
		m_pMagnetWindow->SetVisible(value);
	}
}

void MagnetUI::CreateMagnetWindow()
{
	if (!m_pMagnetWindow)
	{
		XMLFile* file = g_pApp->GetResCache()->GetResource<XMLFile>("UI/Magnet_UI.xml");
		m_pMagnetWindow = StaticCast<Window>(m_pUI->LoadLayout(file));

		UIElement* applyButton = m_pMagnetWindow->GetChild(String("Button_Magnet_Apply"), true);
		SubscribeToEvent(applyButton, E_RELEASED, URHO3D_HANDLER(MagnetUI, HandleApplyMagnetDelegate));

		UIElement* changePolarityButton = m_pMagnetWindow->GetChild(String("Button_Magnet_Change_Polarity"), true);
		SubscribeToEvent(changePolarityButton, E_RELEASED, URHO3D_HANDLER(MagnetUI, HandleChangePolarityDelegate));

		UIElement* closeButton = m_pMagnetWindow->GetChild(String("Button_Magnet_Close"), true);
		SubscribeToEvent(closeButton, E_RELEASED, URHO3D_HANDLER(MagnetUI, HandleCloseDelegate));

		m_pEditMagneticField = (LineEdit*)m_pMagnetWindow->GetChild(String("Edit_Magnet_Field"), true);
		m_pTopPolarity = (Text*)m_pMagnetWindow->GetChild(String("Text_Magnet_Top_Polarity_Value"), true);
		m_pBottomPolarity = (Text*)m_pMagnetWindow->GetChild(String("Text_Magnet_Bottom_Polarity_Value"), true);

		SetMagnetUIFontSize();
        m_pMagnetWindow->SetVisible(false);

		m_pUI->GetRoot()->AddChild(m_pMagnetWindow);

        m_bIsInitialized = true;
	}
}


void MagnetUI::SetMagnetUIFontSize()
{
	float textFontSize = LayoutUtility::GetTextFontSize();
	float lineEditFontSize = LayoutUtility::GetLineEditFontSize();

    Text* magnetOption = (Text*)m_pMagnetWindow->GetChild(String("Text_Magnet_Option"), true);
    magnetOption->SetFontSize(lineEditFontSize);

    Text* magnetField = (Text*)m_pMagnetWindow->GetChild(String("Text_Magnet_Field"), true);
	magnetField->SetFontSize(textFontSize);

	Text* topPolarity = (Text*)m_pMagnetWindow->GetChild(String("Text_Magnet_Top_Polarity"), true);
	topPolarity->SetFontSize(textFontSize);

	Text* bottomPolarity = (Text*)m_pMagnetWindow->GetChild(String("Text_Magnet_Bottom_Polarity"), true);
	bottomPolarity->SetFontSize(textFontSize);

	Text* closeButtonText = (Text*)m_pMagnetWindow->GetChild(String("Button_Magnet_Close_Text"), true);
	closeButtonText->SetFontSize(textFontSize);

	Text* applyButtonText = (Text*)m_pMagnetWindow->GetChild(String("Button_Magnet_Apply_Text"), true);
	applyButtonText->SetFontSize(textFontSize);

	Text* changePolarity = (Text*)m_pMagnetWindow->GetChild(String("Button_Magnet_Change_Polarity_Text"), true);
	changePolarity->SetFontSize(textFontSize);

	m_pEditMagneticField->GetTextElement()->SetFontSize(lineEditFontSize);
	m_pTopPolarity->SetFontSize(lineEditFontSize);
	m_pBottomPolarity->SetFontSize(lineEditFontSize);
}


void MagnetUI::HandleApplyMagnetDelegate(StringHash eventType, VariantMap& eventData)
{
	float value = ToFloat(m_pEditMagneticField->GetText());
	m_pMagnetComponent->SetMagneticField(value);
}

void MagnetUI::HandleChangePolarityDelegate(StringHash eventType, VariantMap& eventData)
{
	const MagnetData& magnetData = m_pMagnetComponent->GetMagnetData();
	m_pMagnetComponent->ChangePolarity();
	
	if (magnetData.m_Polarity)
	{
		m_pTopPolarity->SetText(String("North"));
		m_pBottomPolarity->SetText(String("South"));
		m_pNorth->GetComponent<Text3D>()->SetText(String("North"));
		m_pSouth->GetComponent<Text3D>()->SetText(String("South"));
	}
	else
	{
		m_pTopPolarity->SetText(String("South"));
		m_pBottomPolarity->SetText(String("North"));
		m_pNorth->GetComponent<Text3D>()->SetText(String("South"));
		m_pSouth->GetComponent<Text3D>()->SetText(String("North"));
	}

	m_pEditMagneticField->SetText(String(magnetData.m_MagneticField.y_));
}

void MagnetUI::HandleCloseDelegate(StringHash eventType, VariantMap& eventData)
{
	UI* ui = GetSubsystem<UI>();
	UIElement* element = (UIElement*)eventData[Urho3D::Released::P_ELEMENT].GetPtr();

	m_pMagnetWindow->SetVisible(false);
}

void MagnetUI::ShowMagnetUIDelegate(StringHash eventType, VariantMap & eventData)
{
    if(m_pMagnetWindow)
    {
        m_pMagnetWindow->SetVisible(true);
    }
}

void MagnetUI::ContourEnvironmentLoadedDelegate(StringHash eventType, VariantMap & eventData)
{
	Scene* pScene = static_cast<Scene*>(eventData[CONTOUR_ENVIRONMENT_CREATED::P_SCENE].GetPtr());

	if (pScene)
	{
		MagnetLogic* pMagnetLogic = static_cast<MagnetLogic*>(eventData[CONTOUR_ENVIRONMENT_CREATED::P_CONTOUR_LOGIC].GetPtr());
		m_pMagnetComponent = pMagnetLogic->GetMagnetComponent();
		const MagnetData& magnetData = m_pMagnetComponent->GetMagnetData();

		m_pNorth = pScene->GetChild("Text_Magnet_Top_Polus");
		m_pSouth = pScene->GetChild("Text_Magnet_Bottom_Polus");

		if (m_pTopPolarity && m_pBottomPolarity && m_pEditMagneticField)
		{
			if (magnetData.m_Polarity)
			{
				m_pTopPolarity->SetText(String("North"));
				m_pBottomPolarity->SetText(String("South"));
			
				if (m_pNorth && m_pSouth)
				{
					m_pNorth->GetComponent<Text3D>()->SetText(String("North"));
					m_pSouth->GetComponent<Text3D>()->SetText(String("South"));
				}
			}
			else
			{
				m_pTopPolarity->SetText(String("South"));
				m_pBottomPolarity->SetText(String("North"));
				
				m_pNorth->GetComponent<Text3D>()->SetText(String("South"));
				m_pSouth->GetComponent<Text3D>()->SetText(String("North"));
			}

			m_pEditMagneticField->SetText(String(magnetData.m_MagneticField.y_));
		}
	}
	else
	{
		URHO3D_LOGERROR("Failed to get scene in MagnetUI::ContourEnvironmentLoadedDelegate");
	}
}


