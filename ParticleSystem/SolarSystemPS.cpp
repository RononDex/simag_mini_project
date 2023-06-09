#include "SolarSystemPS.h"
#include "SolarSystemParticle.h"
#include "glm/detail/qualifier.hpp"
#include "glm/ext/vector_float4.hpp"

SolarSystemPS::SolarSystemPS() {}

SolarSystemParticle& SolarSystemPS::get(int particleIdx) {
    return this->m_particles[particleIdx];
}

int SolarSystemPS::getParticleCount() { return this->m_particles.size(); }

void SolarSystemPS::add(glm::vec<3, long double> pos,
                        glm::vec<3, long double> vel,
                        glm::vec<3, long double> force, long double mass,
                        glm::vec4 color,
                        const char *name) {
    SolarSystemParticle particle;
    particle.setForce(force);
    particle.setMass(mass);
    particle.setPosition(pos);
    particle.setVelocity(vel);
    particle.setColor(color);
    particle.setName(name);
    this->m_particles.insert(m_particles.end(), particle);
}

void SolarSystemPS::remove(int particleIdx) {
    this->m_particles.erase(this->m_particles.begin() + particleIdx);
}

void SolarSystemPS::add(glm::vec<3, long double> pos,
                        glm::vec<3, long double> vel, long double mass) {
    this->add(pos, vel, glm::vec<3, long double>(0), mass,
              glm::vec4(1, 1, 1, 0), NULL);
}

void SolarSystemPS::clear() {
    this->m_particles.clear();
}
