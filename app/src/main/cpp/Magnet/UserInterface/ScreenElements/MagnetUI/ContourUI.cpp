#include "MagnetStd.h"

#include <Simulation/UserInterface/UserInterface.h>
#include <Simulation/GameLogic/BaseGameLogic.h>

#include "Events/MagnetEvents.h"

#include "GameLogic/MagnetLogic.h"

#include "Components/Objects/ContourComponent.h"
#include "Components/Objects/BatteryComponent.h"

#include "ContourUI.h"

ContourUI::ContourUI(Context* context) : BaseUI(context)
{
	m_pUI = g_pApp->GetSubsystem<UI>();
	m_bIsInitialized = false;
	m_pContourWindow = nullptr;
	m_pEditHeight = nullptr;
	m_pEditWidth = nullptr;
	m_pEditDensity = nullptr;
	m_pEditArea = nullptr;
	m_pMass = nullptr;
	m_pEditRotationalInertia = nullptr;
	m_pEditSystemFriction = nullptr;
	m_pEditGravity = nullptr;
	m_pAmperesForce = nullptr;
	m_pAngularAcceleration = nullptr;
	m_pAngularVelocity = nullptr;
	m_pTorque = nullptr;
	m_pacoeff = nullptr;

	SubscribeToEvent(EVENT_DATA_MAGNET_APP_ENVIRONMENT_CREATED, URHO3D_HANDLER(ContourUI, ContourEnvironmentLoadedDelegate));
	m_pBatteryComponent = nullptr;
	m_pContourComponent = nullptr;
    SubscribeToEvent(EVENT_DATA_SHOW_CONTOUR_UI, URHO3D_HANDLER(ContourUI, ShowContourUIDelegate));
}

bool ContourUI::VOnRestore()
{
	if (!m_bIsInitialized)
	{
		ContourUI::CreateContourWindow();
		return true;
	}
	else
	{
		if (m_pContourWindow)
		{
			m_pContourWindow->SetVisible(true);
		}
	}

	return false;
}

void ContourUI::VSetVisible(bool value)
{
	if (m_pContourWindow)
	{
		m_pContourWindow->SetVisible(value);
	}
}

bool ContourUI::VOnLostDevice()
{
	m_pContourWindow->SetVisible(false);
	return false;
}



void ContourUI::VOnUpdate(float timeStep)
{
	const ContourData& contourData = m_pContourComponent->GetCountourData();
	if (m_bIsInitialized)
	{
		m_pTorque->SetText(String(contourData.m_Torque));
		m_pAmperesForce->SetText(String(contourData.m_AmperesForce));
		m_pacoeff->SetText(String(contourData.m_PaceOff));
		m_pAngularAcceleration->SetText(String(contourData.m_AngularAcceleration.z_));
		m_pAngularVelocity->SetText(String(contourData.m_AngularVelocity.z_));
	}
}

void ContourUI::CreateContourWindow()
{
	if (!m_pContourWindow)
	{
		XMLFile* file = g_pApp->GetResCache()->GetResource<XMLFile>(String("UI/Contour_UI.xml"));
		m_pContourWindow = StaticCast<Window>(m_pUI->LoadLayout(file));

		UIElement* closeButton = m_pContourWindow->GetChild(String("Button_Contour_Close"), true);
		SubscribeToEvent(closeButton, E_RELEASED, URHO3D_HANDLER(ContourUI, HandleCloseDelegate));

		UIElement* applyButton = m_pContourWindow->GetChild(String("Button_Contour_Apply"), true);
		SubscribeToEvent(applyButton, E_RELEASED, URHO3D_HANDLER(ContourUI, HandleApplyContourDelegate));

		UIElement* resetButton = m_pContourWindow->GetChild(String("Button_Contour_Reset"), true);
		SubscribeToEvent(resetButton, E_RELEASED, URHO3D_HANDLER(ContourUI, HandleResetContourDelegate));

		m_pEditHeight = (LineEdit*)m_pContourWindow->GetChild(String("Countour_Edit_Height"), true);
		m_pEditWidth = (LineEdit*)m_pContourWindow->GetChild(String("Edit_Contour_Width"), true);
		m_pEditArea = (LineEdit*)m_pContourWindow->GetChild(String("Countour_Edit_Area"), true);
		m_pEditDensity = (LineEdit*)m_pContourWindow->GetChild(String("Countour_Edit_Density"), true);
		m_pMass = (Text*)m_pContourWindow->GetChild(String("Text_Countour_Mass"), true);
		m_pEditRotationalInertia = (LineEdit*)m_pContourWindow->GetChild(String("Edit_Contour_Intertia_Moment"), true);
		m_pAmperesForce = (Text*)m_pContourWindow->GetChild(String("Text_Contour_Amperes_Force_Value"), true);
		m_pAngularAcceleration = (Text*)m_pContourWindow->GetChild(String("Text_Contour_Acceleration_Value"), true);
		m_pAngularVelocity = (Text*)m_pContourWindow->GetChild(String("Text_Contour_Angular_Velocity_Value"), true);
		m_pTorque = (Text*)m_pContourWindow->GetChild(String("Text_Contour_Torque_Value"), true);
		m_pEditSystemFriction = (LineEdit*)m_pContourWindow->GetChild(String("Edit_Contour_System_Friction"), true);
		m_pEditGravity = (LineEdit*)m_pContourWindow->GetChild(String("Edit_Countour_Gravity"), true);
		m_pacoeff = (Text*)m_pContourWindow->GetChild(String("Text_Contour_a_koef_value"), true);

		SetContourUIFontSize();

		m_pContourWindow->SetVisible(false);

		m_pUI->GetRoot()->AddChild(m_pContourWindow);

        m_bIsInitialized = true;
	}
}

void ContourUI::SetContourUIFontSize()
{
	float textFontSize = LayoutUtility::GetTextFontSize();
	float lineEditFontSize = LayoutUtility::GetLineEditFontSize();

	// -------------- Set font size for text elements

    Text* contourOption = (Text*)m_pContourWindow->GetChild(String("Text_Contour_Option"), true);
    contourOption->SetFontSize(lineEditFontSize);

    Text* contourWidth = (Text*)m_pContourWindow->GetChild(String("Text_Contour_Width"), true);
	contourWidth->SetFontSize(textFontSize);

	Text* contourHeight = (Text*)m_pContourWindow->GetChild(String("Text_Contour_Height"), true);
	contourHeight->SetFontSize(textFontSize);

	Text* crossSectionalArea = (Text*)m_pContourWindow->GetChild(String("Text_Contour_Area"), true);
	crossSectionalArea->SetFontSize(textFontSize);

	Text* contourMass = (Text*)m_pContourWindow->GetChild(String("Text_Contour_Mass"), true);
	contourMass->SetFontSize(textFontSize);

	Text* contourRotInertia = (Text*)m_pContourWindow->GetChild(String("Text_Contour_Intertia_Moment"), true);
	contourRotInertia->SetFontSize(textFontSize);

	Text* rotAcc = (Text*)m_pContourWindow->GetChild(String("Text_Contour_Acceleration"), true);
	rotAcc->SetFontSize(textFontSize);

	Text* contourDensity = (Text*)m_pContourWindow->GetChild(String("Text_Contour_Density"), true);
	contourDensity->SetFontSize(textFontSize);

	Text* rotVel = (Text*)m_pContourWindow->GetChild(String("Text_Contour_Angular_Velocity"), true);
	rotVel->SetFontSize(textFontSize);

	Text* contourTorque = (Text*)m_pContourWindow->GetChild(String("Text_Contour_Torque"), true);
	contourTorque->SetFontSize(textFontSize);

	Text* systemFriction = (Text*)m_pContourWindow->GetChild(String("Text_Contour_System_Friction"), true);
	systemFriction->SetFontSize(textFontSize);

	Text* aKoef = (Text*)m_pContourWindow->GetChild(String("Text_Contour_a_koef"), true);
	aKoef->SetFontSize(textFontSize);

	Text* ampForce = (Text*)m_pContourWindow->GetChild(String("Text_Contour_Amperes_Force"), true);
	ampForce->SetFontSize(textFontSize);

	Text* gravForce = (Text*)m_pContourWindow->GetChild(String("Text_Contour_Gravity"), true);
	gravForce->SetFontSize(textFontSize);

	Text* closeButtonText = (Text*)m_pContourWindow->GetChild(String("Button_Contour_Close_Text"), true);
	closeButtonText->SetFontSize(textFontSize);

	Text* applyButtonText = (Text*)m_pContourWindow->GetChild(String("Button_Contour_Apply_Text"), true);
	applyButtonText->SetFontSize(textFontSize);

	Text* resetButtonText = (Text*)m_pContourWindow->GetChild(String("Button_Contour_Reset_Text"), true);
	resetButtonText->SetFontSize(textFontSize);

	// ----------------------------- Set value's text font size the same as line edit font's size

	m_pMass->SetFontSize(lineEditFontSize);
	m_pAmperesForce->SetFontSize(lineEditFontSize);
	m_pAngularAcceleration->SetFontSize(lineEditFontSize);
	m_pAngularVelocity->SetFontSize(lineEditFontSize);
	m_pTorque->SetFontSize(lineEditFontSize);
	m_pacoeff->SetFontSize(lineEditFontSize);

	// ----------------------------- Set line edit font size

	m_pEditHeight->GetTextElement()->SetFontSize(lineEditFontSize);
	m_pEditWidth->GetTextElement()->SetFontSize(lineEditFontSize);
	m_pEditArea->GetTextElement()->SetFontSize(lineEditFontSize);
	m_pEditDensity->GetTextElement()->SetFontSize(lineEditFontSize);
	m_pEditRotationalInertia->GetTextElement()->SetFontSize(lineEditFontSize);
	m_pEditSystemFriction->GetTextElement()->SetFontSize(lineEditFontSize);
	m_pEditGravity->GetTextElement()->SetFontSize(lineEditFontSize);
}


void ContourUI::HandleApplyContourDelegate(StringHash eventType, VariantMap& eventData)
{
	const ContourData& contourData = m_pContourComponent->GetCountourData();

    // Abs - prevent from wrong input with MINUS sign
	m_pContourComponent->SetHeight(Abs(ToFloat(m_pEditHeight->GetText())));
	m_pContourComponent->SetWidth(Abs(ToFloat(m_pEditWidth->GetText())));
	m_pContourComponent->SetDensity(Abs(ToFloat(m_pEditDensity->GetText())));
	m_pContourComponent->SetCrossArea(Abs(ToFloat(m_pEditArea->GetText())));
	
	m_pMass->SetText(String(m_pContourComponent->CalculateMass()));

	m_pMagnetLogic->SetSystemFriction(Vector3(0.0f, 0.0f, Abs(ToFloat(m_pEditSystemFriction->GetText()))));
	m_pMagnetLogic->SetSystemGravity(Vector3(0.0f, 0.0f, ToFloat(m_pEditGravity->GetText())));

	float temp = Abs(ToFloat(m_pEditRotationalInertia->GetText()));
	if (contourData.m_RotationalInertia != temp)
	{
		m_pContourComponent->SetRotationalInertia(temp);
		m_pEditRotationalInertia->SetText(String(temp));
	}
	else
	{
		m_pEditRotationalInertia->SetText(String(m_pContourComponent->CalculateRotationalInertia()));
	}
}


void ContourUI::HandleCloseDelegate(StringHash eventType, VariantMap& eventData)
{
	UI* ui = GetSubsystem<UI>();
	UIElement* element = (UIElement*)eventData[Urho3D::Released::P_ELEMENT].GetPtr();

	m_pContourWindow->SetVisible(false);
}

void ContourUI::ShowContourUIDelegate(StringHash eventType, VariantMap & eventData)
{
    if(m_pContourWindow)
    {
        m_pContourWindow->SetVisible(true);
    }
}

void ContourUI::ContourEnvironmentLoadedDelegate(StringHash eventType, VariantMap & eventData)
{
	Scene* pScene = static_cast<Scene*>(eventData[CONTOUR_ENVIRONMENT_CREATED::P_SCENE].GetPtr());

	if (pScene)
	{
		m_pMagnetLogic = static_cast<MagnetLogic*>(eventData[CONTOUR_ENVIRONMENT_CREATED::P_CONTOUR_LOGIC].GetPtr());
		m_pContourComponent = m_pMagnetLogic->GetContourComponent();
		m_pBatteryComponent = m_pMagnetLogic->GetBatteryComponent();

		const ContourData& contourData = m_pContourComponent->GetCountourData();

		if (m_pEditHeight && m_pEditWidth && m_pEditDensity && m_pEditArea && m_pMass && m_pEditRotationalInertia && m_pAmperesForce)
		{
			m_pEditHeight->SetText(String(contourData.m_Height));
			m_pEditWidth->SetText(String(contourData.m_Width));
			m_pEditArea->SetText(String(contourData.m_CrossArea));
			m_pEditDensity->SetText(String(contourData.m_Density));
			m_pMass->SetText(String(contourData.m_Mass));
			m_pEditRotationalInertia->SetText(String(contourData.m_RotationalInertia));
			m_pAmperesForce->SetText(String(contourData.m_AmperesForce));
			m_pAngularAcceleration->SetText(String(contourData.m_AngularAcceleration.Length()));
			m_pAngularVelocity->SetText(String(contourData.m_AngularVelocity.Length()));
			m_pTorque->SetText(String(contourData.m_Torque));
			m_pEditSystemFriction->SetText(String(m_pMagnetLogic->GetSystemFriction().z_));
			m_pEditGravity->SetText(String(m_pMagnetLogic->GetSystemGravity().y_));
			m_pacoeff->SetText(String(Vector3::ZERO.Length()));
		}
	}
	else
	{
		URHO3D_LOGERROR("Failed to get scene in ContourUI::ContourEnvironmentLoadedDelegate");
	}
}

void ContourUI::HandleResetContourDelegate(StringHash eventType, VariantMap& eventData)
{
	m_pContourComponent->ResetContour();
	m_pBatteryComponent->ResetBattery();
}
