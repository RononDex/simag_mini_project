#include "TaskMiniProject_Integrator.h"
#include "../Context/GlobalEnvironment.h"
#include "glm/detail/qualifier.hpp"
#include <iostream>
#include <sstream>

void TaskMiniProject_Integrator::setForces() {}
void TaskMiniProject_Integrator::doWork() {
    if (gEnv->stateSim->isRunning) {
        this->RK2_Midpoint();

        for (int i = 0; i < gEnv->solarSystemPS.getParticleCount(); i++) {
            auto &force = gEnv->solarSystemPS.get(i).getForce();
            force = glm::vec<3, long double>();
        }

        gEnv->solarSystemPS.getSimulationDate() += gEnv->stateSim->dt;
    }
}
void TaskMiniProject_Integrator::imGui() {}
const char *TaskMiniProject_Integrator::toString() const {
    std::stringstream ss;
    ss << "Description:"
       << "\n";
    ss << "Integrator for the high precision SolarSystemPS"
       << "\n";

    m_string = ss.str();
    return m_string.c_str();
}
void TaskMiniProject_Integrator::draw() const {}

void TaskMiniProject_Integrator::ensureSizeOfTemps() {
    if (m_tempPos.size() != (size_t)size()) {
        m_tempPos.resize(size());
    }
    if (m_tempVel.size() != (size_t)size()) {
        m_tempVel.resize(size());
    }
}

void TaskMiniProject_Integrator::RK2_Midpoint() {

    long double dt = 3600;
    gEnv->stateSim->dtFixed = dt;
    gEnv->stateSim->dtFixedNoOfStepsPerFrame = 24;

    int nSize = gEnv->solarSystemPS.getParticleCount();
    ensureSizeOfTemps();

    for (int i = 0; i < nSize; i++) {

        auto &particle = gEnv->solarSystemPS.get(i);

        auto &p = particle.getPosition();
        auto &v = particle.getVelocity();
        auto &m = particle.getMass();
        auto &f = particle.getForce();

        if (m_passNumber == 0) {
            // Pass 1
            // Use t0 and t1 as temporary values, which can be used in pass 2
            m_tempPos[i] = p;
            m_tempVel[i] = v;

            // velocity-update
            v = v + (f / m) * (dt / 2);
            // position-update
            p = p + v * (dt / 2);

            m_passNumber++;
        }
        if (m_passNumber == 1) {
            // Pass 2
            // velocity-update
            v = m_tempVel[i] + (f / m) * (dt);
            // position-update
            p = m_tempPos[i] + v * (dt);

            m_passNumber = 0;
        }
    }
}
