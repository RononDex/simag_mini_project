#pragma once

#include "glm/detail/qualifier.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include <glm/vec3.hpp>
#include <string>

enum ParticleType { Star, Planet, Comet, CometFragment };

class SolarSystemParticle {
  public:
    SolarSystemParticle();
    void setMass(long double mass) { this->m_mass = mass; }
    long double &getMass() { return this->m_mass; }

    void setVelocity(glm::vec<3, long double> velocity) {
        this->m_vel = velocity;
    }
    glm::vec<3, long double> &getVelocity() { return this->m_vel; }

    void setPosition(glm::vec<3, long double> pos) { this->m_pos = pos; }
    glm::vec<3, long double> &getPosition() { return this->m_pos; }

    void setForce(glm::vec<3, long double> force) { this->m_force = force; }
    glm::vec<3, long double> &getForce() { return this->m_force; }

    void setColor(glm::vec4 color) { this->m_color = color; }
    glm::vec4 &getColor() { return this->m_color; }

    void setName(const char *name) { this->m_name = name; }
    const char *getName() { return this->m_name; }

    ParticleType getType() { return this->m_type; }
    void setType(ParticleType newType) { this->m_type = newType; }

    std::string toString() const;

  private:
    const char *m_name;
    long double m_mass;
    glm::vec<3, long double> m_pos;
    glm::vec<3, long double> m_vel;
    glm::vec<3, long double> m_force;
    glm::vec4 m_color;
    ParticleType m_type = Planet;
};
