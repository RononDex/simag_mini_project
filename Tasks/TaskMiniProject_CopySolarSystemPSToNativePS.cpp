#include "TaskMiniProject_CopySolarSystemPSToNativePS.h"
#include "../Context/GlobalEnvironment.h"
#include "glm/ext/vector_float3.hpp"
#include "imgui/imgui.h"
#include <iostream>
#include <sstream>

void TaskMiniProject_CopySolarSystemPSToNativePS::setForces() {}
void TaskMiniProject_CopySolarSystemPSToNativePS::doWork() {
    particleSystem(1).clear();

    int count = gEnv->solarSystemPS.getParticleCount();
    for (int i = 0; i < count; i++) {
        auto solarSystemParticle = gEnv->solarSystemPS.get(i);

        particleSystem(1).add(glm::vec3(solarSystemParticle.getPosition().x,
                                        solarSystemParticle.getPosition().y,
                                        solarSystemParticle.getPosition().z));
        auto particle = particleSystem(1).particle(i);
        particle.color() = solarSystemParticle.getColor();
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
    ss << "Adds whole ParticleSystem from SolarSystemPS into PS1."
       << "\n";

    m_string = ss.str();
    return m_string.c_str();
}

void TaskMiniProject_CopySolarSystemPSToNativePS::draw() const {}
