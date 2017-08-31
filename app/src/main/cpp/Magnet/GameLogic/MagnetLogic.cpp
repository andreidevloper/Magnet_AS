#include "MagnetStd.h"

#include "GameView/MagnetView.h"

#include "Components/Objects/BatteryComponent.h"
#include "Components/Objects/ContourComponent.h"
#include "Components/Objects/MagnetComponent.h"

#include "Events/MagnetEvents.h"

#include "MagnetLogic.h"

#define TORQUE_EPSILON 0.0001f

MagnetLogic::MagnetLogic(Context *context) : BaseGameLogic(context)
{
	m_HumanPlayersAttached = 0;
	m_pContour = nullptr;
	m_pBatteryComponent = nullptr;
	m_pContourComponent = nullptr;
	m_pMagnetComponent = nullptr;

	m_Gravity = Vector3(0, -9.8f, 0);
	m_SystemFriction = Vector3(1.0f, 1.0f, 0.1f);
	m_FixedStep = FIXED_STEP_FOR_PHYSICS;
}

MagnetLogic::~MagnetLogic()
{

}

bool MagnetLogic::VInitialize()
{
	if (!BaseGameLogic::VInitialize())
	{
		return false;
	}

	g_pApp->GetSimulationOptions().SetSceneName("Scenes/Electromagnetism_contour_NEW.xml");
	SubscribeToEvent(Urho3D::E_UPDATE, URHO3D_HANDLER(MagnetLogic, HandlePhysicsStepDelegate));



	return true;
}

void MagnetLogic::VShutdown()
{
	BaseGameLogic::VShutdown();
	
}

void MagnetLogic::VChangeState(BaseGameState newState)
{
	BaseGameLogic::VChangeState(newState);

	switch (newState)
	{
		case BGS_WaitingForPlayers:
		{
			// spawn all local players (should only be one, though we might support more in the future)
			assert(m_ExpectedPlayers == 1);
			for (int i = 0; i < m_ExpectedPlayers; i++)
			{
				SharedPtr<IGameView> playerView(new MagnetView(context_, g_pApp->GetRenderer()));
				VAddView(playerView);
			}
			break;
		}

		case BGS_SpawningPlayersActors:
		{
			for (auto it = m_GameViews.Begin(); it != m_GameViews.End(); ++it)
			{
				SharedPtr<IGameView> pView = *it;
				if (pView->VGetType() == GameView_Human)
				{
				
				}
			}
			break;
		}
	}

}

void MagnetLogic::VAddView(SharedPtr<IGameView> pView, NodeId nodeId)
{
	BaseGameLogic::VAddView(pView, nodeId);

	SharedPtr<MagnetView> view = DynamicCast<MagnetView>(pView);
	if (view.NotNull())
	{
		// New local human is attached to the game
		m_HumanPlayersAttached++;
	}
}


void MagnetLogic::VInitializeComponents()
{
	BatteryComponent::RegisterObject(context_);
	ContourComponent::RegisterObject(context_);
	MagnetComponent::RegisterObject(context_);
}

bool MagnetLogic::VLoadGameDelegate(String pLevelData)
{
	if (m_pScene)
	{
		// Get simulation objects
		m_pContour = m_pScene->GetChild("Contour", true);
		Node* pBattery = m_pScene->GetChild("Magnet", true);
		Node* pMagnet = m_pScene->GetChild("Battery", true);

		if (m_pContour && pBattery && pMagnet)
		{
			m_pContourComponent = m_pContour->CreateComponent<ContourComponent>();
			m_pBatteryComponent = pBattery->CreateComponent<BatteryComponent>();
			m_pMagnetComponent = pMagnet->CreateComponent<MagnetComponent>();
		}
		else
		{
			return false;
		}
	}

	VariantMap data;
	data[CONTOUR_ENVIRONMENT_CREATED::P_SCENE] = m_pScene;
	data[CONTOUR_ENVIRONMENT_CREATED::P_CONTOUR_LOGIC] = this;
	SendEvent(EVENT_DATA_MAGNET_APP_ENVIRONMENT_CREATED, data);

	return true;
}

// Do not use PHYSICS SYSTEM on Android
void MagnetLogic::HandlePhysicsStepDelegate(StringHash eventType, VariantMap& eventData)
{
	if (m_pContourComponent)
	{
		const ContourData& contourData = m_pContourComponent->GetCountourData();
		const MagnetData& magnetData = m_pMagnetComponent->GetMagnetData();
		const BatteryData& batteryData = m_pBatteryComponent->GetBatteryData();

		Vector3 eulerOrientation = m_pContour->GetRotation().EulerAngles();
		if (eulerOrientation.z_ >= 360 || eulerOrientation.z_ <= -360)
		{
			m_pContour->SetRotation(Quaternion(0, 0, 0));
		}

		float alpha_angle = eulerOrientation.z_;
		float fi_angle = eulerOrientation.z_ > 0 ? (180 - (90 + eulerOrientation.z_)) : (-180 + abs(-90 + eulerOrientation.z_));
		
		Vector3 ampereTorque;
		if (batteryData.m_bIsCircuitEnabled)
		{
			float armOfForceL1 = contourData.m_Height * Sin(fi_angle);

			ampereTorque.z_ = GetAmperesForce(
				batteryData.m_Intensity, 
				magnetData.m_MagneticField, 
				Vector3(0, armOfForceL1, 0),
				alpha_angle);	
		}
		else
		{

		}

		Vector3 gravityTorque;
		float armOfForceL2 = contourData.m_Height * Cos(fi_angle);
		gravityTorque.z_ = GetGravityForce(contourData.m_Mass, Vector3(0, armOfForceL2, 0), alpha_angle);

		Vector3 friction = GetFriction(m_SystemFriction, contourData.m_AngularVelocity);

		float totalTorque = ampereTorque.z_ - gravityTorque.z_ - friction.z_;
		m_pContourComponent->SetTorque(totalTorque);

		Vector3 newAngularAcceleration;
		if (totalTorque > TORQUE_EPSILON || totalTorque < TORQUE_EPSILON)
		{
			newAngularAcceleration.z_ = totalTorque / contourData.m_RotationalInertia;
		}
		else
		{
			newAngularAcceleration.z_ = 0;
		}

		Vector3 currentAngularVelocity = contourData.m_AngularVelocity;
		currentAngularVelocity.z_ += newAngularAcceleration.z_ * m_FixedStep;

		m_pContourComponent->SetAngularVelocity(currentAngularVelocity);
		m_pContourComponent->SetAngularAcceleration(newAngularAcceleration);

		float amperesForce = contourData.m_Width * batteryData.m_Intensity.Length() * magnetData.m_MagneticField.Length();
		float a = amperesForce / (contourData.m_Mass * m_Gravity.Length());

		m_pContourComponent->SetAmpereForce(amperesForce);
		m_pContourComponent->SetPaceOff(a);
	}
}

float MagnetLogic::GetAmperesForce(Vector3 intensity, Vector3 magneticField, Vector3 armOfForceL1, float angle)
{
	float l1 = intensity.z_ * magneticField.y_ * armOfForceL1.Length() * Cos(angle);
	return l1;
}

float MagnetLogic::GetGravityForce(float mass, Vector3 armOfForceL2, float angle)
{
	float l2 = mass * m_Gravity.Length() * armOfForceL2.Length() * Sin(angle);
	return l2;
}

Vector3 MagnetLogic::GetFriction(Vector3 friction, Vector3 angularVelocity)
{
	float x = friction.x_ * angularVelocity.x_;
	float y = friction.y_ * angularVelocity.y_;
	float z = friction.z_ * angularVelocity.z_;
	return Vector3(x, y, z);
}