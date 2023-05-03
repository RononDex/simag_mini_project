#include "TaskMiniProject_CopySolarSystemPSToNativePS.h"
#include "../Context/GlobalEnvironment.h"
#include "glm/ext/vector_float3.hpp"
#include "imgui/imgui.h"
#include <iostream>
#include <sstream>

void TaskMiniProject_CopySolarSystemPSToNativePS::setForces() {}
void TaskMiniProject_CopySolarSystemPSToNativePS::doWork() {
    gEnv->stateSim->isRunning = false;

    int count = gEnv->solarSystemPS.getParticleCount();
    if (count != m_particleCount) {
        particleSystem(m_psId).clear();
        for (int i = 0; i < count; i++) {
            particleSystem(m_psId).add(glm::vec3(0, 0, 0));
            particleSystem(m_psId).particle(i).setStatic(true);
        }

        m_particleCount = count;
    }
    for (int i = 0; i < count; i++) {
        auto solarSystemParticle = gEnv->solarSystemPS.get(i);
        auto ps = particleSystem(m_psId);

        ps.positions()[i] = solarSystemParticle.getPosition();
        ps.velocities()[i] = solarSystemParticle.getVelocity();
        ps.colors()[i] = solarSystemParticle.getColor();
        ps.forces()[i] = solarSystemParticle.getForce();
    }
}

void TaskMiniProject_CopySolarSystemPSToNativePS::imGui() {
    ImGui::Checkbox(paramName("Enable copying of SolarSystemPS to PS1"),
                    &m_enable);

    ImGui::Separator();
    if (ImGui::Button("Clear PS1")) {
        particleSystem(1).clear();
    }
}

const char *TaskMiniProject_CopySolarSystemPSToNativePS::toString() const {
    std::stringstream ss;
    ss << "Description:"
       << "\n";
    ss << "Adds whole ParticleSystem from SolarSystemPS into PS0 and stops "
          "simulation in PS0"
       << "\n";

    m_string = ss.str();
    return m_string.c_str();
}

void TaskMiniProject_CopySolarSystemPSToNativePS::draw() const {}
