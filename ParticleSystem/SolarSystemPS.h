#pragma once

#include "SolarSystemParticle.h"
#include "glm/detail/qualifier.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include <vector>

class SolarSystemPS {

  public:
    SolarSystemPS();
    SolarSystemParticle get(int particleId);
    int getParticleCount();
    void add(glm::vec<3, long double> pos, glm::vec<3, long double> vel,
             glm::vec<3, long double> force, long double mass, glm::vec4 color);
    void add(glm::vec<3, long double> pos, glm::vec<3, long double> vel,
             long double mass);

  private:
    std::vector<SolarSystemParticle> m_particles;
};
