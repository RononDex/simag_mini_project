#include "TaskSolarSystem_CopySolarSystemPSToNativePS.h"
#include "../Context/GlobalEnvironment.h"
#include "glm/ext/vector_float3.hpp"
#include "imgui/imgui.h"
#include <cstring>
#include <iostream>
#include <sstream>

void TaskSolarSystem_CopySolarSystemPSToNativePS::setForces() {}
void TaskSolarSystem_CopySolarSystemPSToNativePS::findNamedParticles(
    int &count) {
    m_particleNames.clear();
    m_particleNameIndices.clear();
    for (int i = 0; i < count; i++) {
        if (gEnv->solarSystemPS.get(i).getName() != NULL) {
            m_particleNameIndices.push_back(i);
            m_particleNames.push_back(gEnv->solarSystemPS.get(i).getName());
        }
    }
}
void TaskSolarSystem_CopySolarSystemPSToNativePS::doWork() {

    int count = gEnv->solarSystemPS.getParticleCount();
    if (count != m_particleCount) {
        particleSystem(m_psId).clear();
        for (int i = 0; i < count; i++) {
            particleSystem(m_psId).add(glm::vec3(0, 0, 0));
            particleSystem(m_psId).particle(i).setStatic(true);
        }

        m_particleCount = count;
    }

    findNamedParticles(count);

    auto &ps = particleSystem(m_psId);
    auto &colors = ps.colors();
    auto &forces = ps.forces();
    auto &positions = ps.positions();
    auto &velocities = ps.velocities();
    auto centerPosition = glm::vec3(0);

    if (m_selectedCenterBody >= 0 &&
        m_selectedCenterBody < m_particleNameIndices.size()) {

        centerPosition =
            gEnv->solarSystemPS.get(m_particleNameIndices[m_selectedCenterBody])
                .getPosition() *
            m_scalingFactor;
    }

    for (int i = 0; i < count; i++) {
        auto solarSystemParticle = gEnv->solarSystemPS.get(i);

        positions[i] = solarSystemParticle.getPosition() * m_scalingFactor;
        positions[i] -= centerPosition;

        velocities[i] = solarSystemParticle.getVelocity() * m_scalingFactor;
        colors[i] = solarSystemParticle.getColor();
        forces[i] = solarSystemParticle.getForce() * m_scalingFactor;
    }
}

void TaskSolarSystem_CopySolarSystemPSToNativePS::imGui() {
    std::vector<const char *> names;
    for (std::string const &str : m_particleNames) {
        names.push_back(str.data());
    }
    ImGui::Combo("ChooseCenteredObject", &m_selectedCenterBody, names.data(),
                 m_particleNames.size());
}

const char *TaskSolarSystem_CopySolarSystemPSToNativePS::toString() const {
    std::stringstream ss;
    ss << "Description:"
       << "\n";
    ss << "Adds whole ParticleSystem from SolarSystemPS into PS0 and stops "
          "simulation in PS0"
       << "\n";

    m_string = ss.str();
    return m_string.c_str();
}

void TaskSolarSystem_CopySolarSystemPSToNativePS::draw() const {}
