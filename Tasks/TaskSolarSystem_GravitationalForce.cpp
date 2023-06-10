#include "TaskSolarSystem_GravitationalForce.h"
#include "../Context/GlobalEnvironment.h"
#include "glm/gtx/quaternion.hpp"
#include <iostream>
#include <sstream>

void TaskSolarSystem_GravitationalForce::setForces() {
    for (int i = 0; i < gEnv->solarSystemPS.getParticleCount(); i++) {
        auto &particle = gEnv->solarSystemPS.get(i);

        for (int j = 0; j < gEnv->solarSystemPS.getParticleCount(); j++) {
            // Ignore gravitational forces with one self
            if (j == i)
                continue;

            // Calculate and add the gravitational force for this particle to all
            // other particles
            auto &otherParticle = gEnv->solarSystemPS.get(j);
            if (otherParticle.getType() == Planet ||
                otherParticle.getType() == Star) {
                auto delta_Vector =
                    otherParticle.getPosition() - particle.getPosition();
                auto distance_squared = glm::length2(delta_Vector);

                auto F_G = delta_Vector / std::sqrt(distance_squared) *
                           particle.getMass() * otherParticle.getMass() *
                           m_gravityConstant / distance_squared;

                particle.getForce() += F_G;
            }
        }
    }
}
void TaskSolarSystem_GravitationalForce::doWork() {}
void TaskSolarSystem_GravitationalForce::imGui() {}
const char *TaskSolarSystem_GravitationalForce::toString() const {
    std::stringstream ss;
    ss << "Description:"
       << "\n";
    ss << "Integrator for the high precision SolarSystemPS"
       << "\n";

    m_string = ss.str();
    return m_string.c_str();
}
void TaskSolarSystem_GravitationalForce::draw() const {}
