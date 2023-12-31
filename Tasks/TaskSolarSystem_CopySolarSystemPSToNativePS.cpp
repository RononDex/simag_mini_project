#include "TaskSolarSystem_CopySolarSystemPSToNativePS.h"
#include "../Context/GlobalEnvironment.h"
#include "glm/detail/qualifier.hpp"
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
    m_particleNames.push_back("None");
    m_particleNameIndices.push_back(-1);
    for (int i = 0; i < count; i++) {
        if (gEnv->solarSystemPS.get(i).getName() != NULL &&
            strcmp("", gEnv->solarSystemPS.get(i).getName()) != 0) {
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

    this->findNamedParticles(count);

    auto &ps = particleSystem(m_psId);
    auto &colors = ps.colors();
    auto &forces = ps.forces();
    auto &positions = ps.positions();
    auto &velocities = ps.velocities();
    TaskSolarSystem_CopySolarSystemPSToNativePS::center = glm::vec3(0);

    if (m_selectedCenterBody > 0 &&
        m_selectedCenterBody < m_particleNameIndices.size()) {
        TaskSolarSystem_CopySolarSystemPSToNativePS::center =
            gEnv->solarSystemPS.get(m_particleNameIndices[m_selectedCenterBody])
                .getPosition() *
            SCALING_FACTOR;
    }

    for (int i = 0; i < count; i++) {
        auto solarSystemParticle = gEnv->solarSystemPS.get(i);

        positions[i] = solarSystemParticle.getPosition() * SCALING_FACTOR;
        positions[i] -= TaskSolarSystem_CopySolarSystemPSToNativePS::center;

        velocities[i] = solarSystemParticle.getVelocity() * SCALING_FACTOR;
        colors[i] = solarSystemParticle.getColor();
        forces[i] = solarSystemParticle.getForce() * SCALING_FACTOR;
    }
}

void TaskSolarSystem_CopySolarSystemPSToNativePS::imGui() {
    std::vector<const char *> names;
    for (std::string const &str : m_particleNames) {
        names.push_back(str.data());
    }
    ImGui::Combo(paramName("Centered Object"), &m_selectedCenterBody,
                 names.data(), m_particleNames.size());
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

glm::vec3 TaskSolarSystem_CopySolarSystemPSToNativePS::center = glm::vec3(0);
const long double TaskSolarSystem_CopySolarSystemPSToNativePS::SCALING_FACTOR =
    (long double)1 / (long double)1e9;
