#include "TaskMiniProject_CopySolarSystemPSToNativePS.h"
#include "../Context/GlobalEnvironment.h"
#include "glm/ext/vector_float3.hpp"
#include "imgui/imgui.h"
#include <iostream>
#include <sstream>

void TaskMiniProject_CopySolarSystemPSToNativePS::setForces() {}
void TaskMiniProject_CopySolarSystemPSToNativePS::doWork() {

    int count = gEnv->solarSystemPS.getParticleCount();
    if (count != m_particleCount) {
        particleSystem(m_psId).clear();
        for (int i = 0; i < count; i++) {
            particleSystem(m_psId).add(glm::vec3(0, 0, 0));
            particleSystem(m_psId).particle(i).setStatic(true);
        }

        m_particleCount = count;
    }

    auto &ps = particleSystem(m_psId);
    auto &colors = ps.colors();
    auto &forces = ps.forces();
    auto &positions = ps.positions();
    auto &velocities = ps.velocities();

    for (int i = 0; i < count; i++) {
        auto solarSystemParticle = gEnv->solarSystemPS.get(i);

        positions[i] = solarSystemParticle.getPosition() * m_scalingFactor;
        velocities[i] = solarSystemParticle.getVelocity() * m_scalingFactor;
        colors[i] = solarSystemParticle.getColor();
        forces[i] = solarSystemParticle.getForce() * m_scalingFactor;
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
