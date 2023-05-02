#pragma once

#include <memory>

#include "States.h"
#include "../Helper/Camera.h"
#include "../ParticleSystem/SolarSystemPS.h"

struct GlobalEnvironment
{
	StateSimulation* stateSim = nullptr;
	StateGui* stateGui = nullptr;
	Camera* camera = nullptr;
    SolarSystemPS solarSystemPS;

	bool logDebug = false;

	// ---------------------------

	GlobalEnvironment();
	~GlobalEnvironment();
	
	static void init();

	void createCameras(glm::ivec2 const& sizeFramebuffer, glm::ivec2 const& sizeWindow);
	void updateCamera(float dt);
	void switchCamera();
};

extern GlobalEnvironment* gEnv;
