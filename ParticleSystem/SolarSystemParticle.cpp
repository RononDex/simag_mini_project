#include "SolarSystemParticle.h"
#include "glm/gtx/string_cast.hpp"
#include <sstream>
#include <string>

SolarSystemParticle::SolarSystemParticle() {}

std::string SolarSystemParticle::toString() const {
    char *type = "Planet";
    if (this->m_type == Comet)
        type = "Comet";
    std::stringstream ss;
    ss << "Name     :     " << this->m_name << "\n";
    ss << "Mass [kg]:     " << this->m_mass << "\n";
    ss << "Pos  [km]:     "
       << "X: " << (this->m_pos.x) << " Y: " << (this->m_pos.y)
       << " Z: " << (this->m_pos.z) << "\n";
    ss << "Vel [km/s]:    "
       << "X: " << (this->m_vel.x) << " Y: " << (this->m_vel.y)
       << " Z: " << (this->m_vel.z) << "\n";
    ss << "Force:         "
       << "X: " << (this->m_force.x) << " Y: " << (this->m_force.y)
       << " Z: " << (this->m_force.z) << "\n";
    ss << "Type:          " << type << "\n";
    return ss.str();
}
