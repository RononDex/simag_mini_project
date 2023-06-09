#include "TaskSolarSystem_SetupSolarSystem.h"
#include "../Context/GlobalEnvironment.h"
#include "glm/detail/qualifier.hpp"
#include "imgui/imgui.h"
#include <ctime>
#include <iostream>
#include <sstream>

void TaskSolarSystem_SetupSolarSystem::setupSolarSystem() {
    gEnv->stateGui->bDrawFloor = false;
    gEnv->stateGui->bDrawAxis = false;
    gEnv->solarSystemPS.clear();
    gEnv->camera->setNear(0.01f);

    // 1990-01-01
    struct tm date = {.tm_mday = 1, .tm_mon = 0, .tm_year = 90};
    gEnv->solarSystemPS.setSimulationDate(mktime(&date));

    // Setup simulation parameters
    gEnv->stateSim->dtFixed = 3600;
    gEnv->stateSim->dtFixedNoOfStepsPerFrame = 24;

    // Data source: https://ssd.jpl.nasa.gov/horizons/app.html
    //  -   Use suns center of mass as reference point (@sun)
    //  -   Choose "Generate Vector Table"
    // Start Time: 1990-01-01 00:00:00 UTC
    // Units: Kilometers (km) and Seconds (s) and kilograms (kg)
    // Sun
    gEnv->solarSystemPS.add(
        glm::vec<3, long double>(0), glm::vec<3, long double>(1),
        glm::vec<3, long double>(0), 1.9885e30L, glm::vec4(1, 1, 0, 1), "Sun");

    // Mercury
    gEnv->solarSystemPS.add(glm::vec<3, long double>(2.449118280444862E+07L,
                                                     3.944680940519455E+07L,
                                                     9.732604371460751E+05L),
                            glm::vec<3, long double>(-5.109267189897491E+01L,
                                                     2.765412990432811E+01L,
                                                     6.949526145116829E+00L),
                            glm::vec<3, long double>(0), 0.33010E+24L,
                            glm::vec4(0.7, 0.7, 0.7, 1.0), "Mercury");

    // Venus
    gEnv->solarSystemPS.add(glm::vec<3, long double>(6.370471178752395E+05L,
                                                     1.076509578782607E+08L,
                                                     1.431144385739453E+06L),
                            glm::vec<3, long double>(-3.513982897679553E+01L,
                                                     2.214594971075456E-02L,
                                                     2.028985897437458E+00L),
                            glm::vec<3, long double>(0), 4.8673E+24L,
                            glm::vec4(0.7, 0.6, 0.2, 1.0), "Venus");

    // Earth
    gEnv->solarSystemPS.add(glm::vec<3, long double>(-2.667149360810669E+07L,
                                                     1.446668846810489E+08L,
                                                     3.218721453152597E+03L),
                            glm::vec<3, long double>(-2.978694866288836E+01L,
                                                     -5.521925698920421E+00L,
                                                     -1.361791058985595E-03L),
                            glm::vec<3, long double>(0), 5.9722E+24L,
                            glm::vec4(0.1, 0.1, 0.9, 1.0), "Earth");

    // Mars
    gEnv->solarSystemPS.add(glm::vec<3, long double>(-1.460585480466399E+08L,
                                                     -1.797057215871828E+08L,
                                                     -1.707885377997905E+05L),
                            glm::vec<3, long double>(1.972017359483419E+01L,
                                                     -1.320856997950926E+01L,
                                                     -7.617352184465345E-01L),
                            glm::vec<3, long double>(0), 0.64169E+24L,
                            glm::vec4(0.4, 0.35, 0.2, 1.0), "Mars");

    // Jupiter
    gEnv->solarSystemPS.add(glm::vec<3, long double>(-8.483726247556832E+07L,
                                                     7.658586501579256E+08L,
                                                     -1.269434662249386E+06L),
                            glm::vec<3, long double>(-1.316554559845475E+01L,
                                                     -8.319802694775795E-01L,
                                                     2.983343752214045E-01L),
                            glm::vec<3, long double>(0), 1898.13E+24L,
                            glm::vec4(0.35, 0.2, 0.65, 1.0), "Jupiter");

    // Saturn
    gEnv->solarSystemPS.add(glm::vec<3, long double>(4.200205213646205E+08L,
                                                     -1.440011254791890E+09L,
                                                     8.412369189701736E+06L),
                            glm::vec<3, long double>(8.743104200372343E+00L,
                                                     2.678164385452360E+00L,
                                                     -3.948148438761898E-01L),
                            glm::vec<3, long double>(0), 568.32E+24L,
                            glm::vec4(0.1, 0.85, 0.2, 1.0), "Saturn");

    // Uranus
    gEnv->solarSystemPS.add(glm::vec<3, long double>(2.863535123540787E+08L,
                                                     -2.885000824213084E+09L,
                                                     -1.441960578606236E+07),
                            glm::vec<3, long double>(6.715776823305395E+00L,
                                                     3.560785287875484E-01L,
                                                     -8.557523627458505E-02L),
                            glm::vec<3, long double>(0), 86.811E+24L,
                            glm::vec4(0.0, 0.4, 1.0, 1.0), "Uranus");

    // Neptune
    gEnv->solarSystemPS.add(glm::vec<3, long double>(9.563524137819563E+08L,
                                                     -4.416536139446790E+09L,
                                                     6.891211901480341E+07),
                            glm::vec<3, long double>(5.267418714818683E+00L,
                                                     1.180140172482391E+00L,
                                                     -1.458325266649975E-01L),
                            glm::vec<3, long double>(0), 102.409E+24L,
                            glm::vec4(1.0, 0.0, 0.5, 1.0), "Neptune");

    // Activate simulation
    gEnv->stateSim->isRunning = true;
}

void TaskSolarSystem_SetupSolarSystem::setForces() {}
void TaskSolarSystem_SetupSolarSystem::doWork() {}
void TaskSolarSystem_SetupSolarSystem::imGui() {
    if (ImGui::Button("Clear PS")) {
        gEnv->solarSystemPS.clear();
    }
    if (ImGui::Button("Setup Solar System")) {
        setupSolarSystem();
    }

    ImGui::Separator();
    ImGui::Text("%s %s", "Simulation Date",
                ctime(&gEnv->solarSystemPS.getSimulationDate()));
}

const char *TaskSolarSystem_SetupSolarSystem::toString() const {
    std::stringstream ss;
    ss << "Description:"
       << "\n";
    ss << "Sets up the solar system particle system"
       << "\n";

    m_string = ss.str();
    return m_string.c_str();
}
void TaskSolarSystem_SetupSolarSystem::draw() const {}
