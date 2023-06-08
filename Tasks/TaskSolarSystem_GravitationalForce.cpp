#include "TaskSolarSystem_GravitationalForce.h"
#include "../Context/GlobalEnvironment.h"
#include <iostream>
#include <sstream>

void TaskSolarSystem_GravitationalForce::setForces() {
    for (int i = 0; i < gEnv->solarSystemPS.getParticleCount(); i++) {
        auto &particle = gEnv->solarSystemPS.get(i);

        for (int j = 0; j < gEnv->solarSystemPS.getParticleCount(); j++) {
            // Ignore gravitational forces with one self
            if (j == i)
                continue;

            // Calculate and add the gravitional force for this particle to all
            // other particles
            auto &otherParticle = gEnv->solarSystemPS.get(j);
            auto delta_Vector =
                otherParticle.getPosition() - particle.getPosition();
            auto distance_squared = std::pow(particle.getPosition().x -
                                                 otherParticle.getPosition().x,
                                             2) +
                                    std::pow(particle.getPosition().y -
                                                 otherParticle.getPosition().y,
                                             2) +
                                    std::pow(particle.getPosition().z -
                                                 otherParticle.getPosition().z,
                                             2);

            auto F_G = delta_Vector / std::sqrt(distance_squared) *
                       particle.getMass() * otherParticle.getMass() *
                       m_gravityConstant / distance_squared;

            particle.getForce() += F_G;
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
