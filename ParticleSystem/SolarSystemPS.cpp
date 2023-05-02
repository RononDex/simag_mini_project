#include "SolarSystemPS.h"
#include "SolarSystemParticle.h"
#include "glm/detail/qualifier.hpp"

SolarSystemPS::SolarSystemPS() {}

SolarSystemParticle SolarSystemPS::get(int particleId) {
    return this->m_particles[particleId];
}

int SolarSystemPS::getParticleCount() { return this->m_particles.size(); }

void SolarSystemPS::add(glm::vec<3, long double> pos,
                        glm::vec<3, long double> vel,
                        glm::vec<3, long double> force, long double mass) {
    auto particle = new SolarSystemParticle();
    particle->setForce(force);
    particle->setMass(mass);
    particle->setPosition(pos);
    particle->setVelocity(vel);
    this->m_particles.insert(m_particles.end(), *particle);
}
void SolarSystemPS::add(glm::vec<3, long double> pos,
                        glm::vec<3, long double> vel, long double mass) {
    this->add(pos, vel, *new glm::vec<3, long double>(0), mass);
}
