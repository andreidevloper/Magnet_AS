#include "MagnetStd.h"

#include "GameEngine/Events/GameEngineEvents.h"
#include "GameLogic/MagnetLogic.h"

#include "MagnetApp.h"

// Application entry-point
URHO3D_DEFINE_APPLICATION_MAIN(MagnetApp)

MagnetApp::MagnetApp(Context* context) : GameEngineApp(context)
{
	g_pApp = this;
	m_bIsInit = false;
}

MagnetApp::~MagnetApp()
{

}


void MagnetApp::Setup()
{
    GameEngineApp::Setup();


}


void MagnetApp::Start()
{
    GameEngineApp::Start();

    SendEvent(EVENT_DATA_START_GAME);
	m_bIsInit = true;
}

void MagnetApp::Stop()
{
    GameEngineApp::Stop();

}


bool MagnetApp::VCreateViewLogic()
{
	m_pGameLogic = new MagnetLogic(context_);
	context_->RegisterSubsystem(m_pGameLogic);

	if (!m_pGameLogic->VInitialize())
	{
		URHO3D_LOGERROR("Game logic init failed");
		m_bIsInit = false;
	}
	URHO3D_LOGDEBUG("Game logic successfully initialized");

	return true;
}

