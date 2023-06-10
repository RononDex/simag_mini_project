#include "TaskSolarSystem_Integrator.h"
#include "../Context/GlobalEnvironment.h"
#include "glm/detail/qualifier.hpp"
#include <iostream>
#include <sstream>

void TaskSolarSystem_Integrator::setForces() {
    if (gEnv->stateSim->isRunning) {
        this->Euler_Cromer();

        for (int i = 0; i < gEnv->solarSystemPS.getParticleCount(); i++) {
            auto &force = gEnv->solarSystemPS.get(i).getForce();
            force = glm::vec<3, long double>();
        }

        gEnv->solarSystemPS.getSimulationDate() += gEnv->stateSim->dtFixed;
    }
}
void TaskSolarSystem_Integrator::doWork() {}
void TaskSolarSystem_Integrator::imGui() {}
const char *TaskSolarSystem_Integrator::toString() const {
    std::stringstream ss;
    ss << "Description:"
       << "\n";
    ss << "Integrator for the high precision SolarSystemPS"
       << "\n";

    m_string = ss.str();
    return m_string.c_str();
}
void TaskSolarSystem_Integrator::draw() const {}

void TaskSolarSystem_Integrator::ensureSizeOfTemps() {
    if (m_tempPos.size() != (size_t)size()) {
        m_tempPos.resize(size());
    }
    if (m_tempVel.size() != (size_t)size()) {
        m_tempVel.resize(size());
    }
}

void TaskSolarSystem_Integrator::Euler_Cromer() {

    long double dt = gEnv->stateSim->dtFixed;
    int nSize = gEnv->solarSystemPS.getParticleCount();

    for (int i = 0; i < nSize; i++) {

        auto &particle = gEnv->solarSystemPS.get(i);

        auto &p = particle.getPosition();
        auto &v = particle.getVelocity();
        auto &m = particle.getMass();
        auto &f = particle.getForce();

        // velocity-update
        v = v + (f / m) * (dt);
        // position-update
        p = p + v * (dt);
    }
}
