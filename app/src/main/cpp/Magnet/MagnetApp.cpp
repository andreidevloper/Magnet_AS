#include "MagnetStd.h"

#include "Simulation/Events/SimulationsEvents.h"
#include "Simulation/Mainloop/Initialization.h"

#include "GameLogic/MagnetLogic.h"
#include "GameView/MagnetView.h"

#include "MagnetApp.h"

// Application entry-point
URHO3D_DEFINE_APPLICATION_MAIN(MagnetApp)

MagnetApp::MagnetApp(Context* context) : SimulationApp(context)
{
	g_pApp = this;
	m_bIsInit = false;
}

MagnetApp::~MagnetApp()
{

}


void MagnetApp::Setup()
{
	SimulationApp::Setup();


}


void MagnetApp::Start()
{
	SimulationApp::Start();

    SendEvent(EVENT_DATA_START_GAME);
	m_bIsInit = true;
}

void MagnetApp::Stop()
{
	SimulationApp::Stop();

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

