#include "TaskMiniProject_SetupSolarSystem.h"
#include "../Context/GlobalEnvironment.h"
#include "glm/detail/qualifier.hpp"
#include "imgui/imgui.h"
#include <iostream>
#include <sstream>

void TaskMiniProject_SetupSolarSystem::setupSolarSystem() {
    gEnv->stateGui->bDrawFloor = false;
    gEnv->stateGui->bDrawAxis = false;
    gEnv->solarSystemPS.clear();

    long double scaleFactor = 1.0;

    // Data source: https://ssd.jpl.nasa.gov/horizons/app.html
    // Start Time: 1990-01-01 00:00:00 UTC
    // Sun
    gEnv->solarSystemPS.add(
        glm::vec<3, long double>(0), glm::vec<3, long double>(1),
        glm::vec<3, long double>(0), 1.9885e30L, glm::vec4(1, 1, 0, 1));

    // Mercury
    gEnv->solarSystemPS.add(
        glm::vec<3, long double>(2.449118280444862E+07L, 3.944680940519455E+07L,
                                 9.732604371460751E+05L),
        glm::vec<3, long double>(-5.109267189897491E+01L,
                                 2.765412990432811E+01L,
                                 6.949526145116829E+00L),
        glm::vec<3, long double>(0), 0.33010E+24L, glm::vec4(1.0, 1.0, 0.3, 1.0));
}

void TaskMiniProject_SetupSolarSystem::setForces() {}
void TaskMiniProject_SetupSolarSystem::doWork() {}
void TaskMiniProject_SetupSolarSystem::imGui() {
    if (ImGui::Button("Clear PS")) {
        gEnv->solarSystemPS.clear();
    }
    if (ImGui::Button("Setup Solar System")) {
        setupSolarSystem();
    }
}

const char *TaskMiniProject_SetupSolarSystem::toString() const {
    std::stringstream ss;
    ss << "Description:"
       << "\n";
    ss << "Sets up the solar system particle system"
       << "\n";

    m_string = ss.str();
    return m_string.c_str();
}
void TaskMiniProject_SetupSolarSystem::draw() const {}
