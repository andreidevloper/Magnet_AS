#pragma once

//----------------------------------------------------------------------------------------------------
// MagnetApp - Application class contains all necessary components in order to run program correctly.
//----------------------------------------------------------------------------------------------------

class MagnetApp : public GameEngineApp
{
public:
	// Constructors
	MagnetApp(Context* context);
	virtual ~MagnetApp();

	// Application class overrided functions
	virtual void Setup();						// Called before engine initialization. Logging system and
	// user specific game options init here

	virtual void Start();						// Called after engine initialization. Setup application & subscribe to events here
	virtual void Stop();						// Perform optional cleanup after main loop has terminated

	// Create game logic and game views
	bool virtual VCreateViewLogic();			// Create specific game logic and game view.

	virtual String GetWindowIcon() { return "System/Atom.jpg"; }
	virtual String GetWindowTitle() { return "Labaratory"; }
};
