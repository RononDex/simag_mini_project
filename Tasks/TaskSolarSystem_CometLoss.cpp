#include "TaskSolarSystem_CometLoss.h"
#include "../Context/GlobalEnvironment.h"
#include "glm/geometric.hpp"
#include "glm/gtx/quaternion.hpp"
#include <iostream>
#include <sstream>
#include <type_traits>

void TaskSolarSystem_CometLoss::setForces() {
    for (int i = 0; i < gEnv->solarSystemPS.getParticleCount(); i++) {
        auto &particle = gEnv->solarSystemPS.get(i);

        if (particle.getType() == Comet) {
            for (int j = 0; j < gEnv->solarSystemPS.getParticleCount(); j++) {
                auto &otherParticle = gEnv->solarSystemPS.get(j);

                if (otherParticle.getType() == Star) {
                    auto delta =
                        otherParticle.getPosition() - particle.getPosition();
                    auto distance = glm::length(delta);

                    // Light Intensity follows the inversed square law (falls of
                    // with distance squared)
                    // At ~1AU it should be around 1365 W/m² for our sun
                    auto intensity =
                        1 / std::pow(distance, 2) * m_intensityConstant;

                    auto materialLossInKg = intensity * m_massToIntensityRatio *
                                            (gEnv->stateSim->dt / 3600);
                    m_accumulatedMassLossKg += materialLossInKg;
                    if (m_accumulatedMassLossKg >=
                        m_massLossThresholdNewParticle) {
                        particle.getMass() -= m_accumulatedMassLossKg;
                        particle.getForce() +=
                            glm::normalize(delta) * m_appliedForceOnMassLoss;

                        gEnv->solarSystemPS.add(
                            glm::vec<3, long double>(particle.getPosition().x,
                                                     particle.getPosition().y,
                                                     particle.getPosition().z),
                            glm::vec<3, long double>(particle.getVelocity().x,
                                                     particle.getVelocity().y,
                                                     particle.getVelocity().z),
                            -glm::normalize(delta) * m_appliedForceOnMassLoss,
                            m_accumulatedMassLossKg, particle.getColor(), "",
                            CometFragment);

                        m_accumulatedMassLossKg = 0;
                    }
                }
            }
        }
    }
}
void TaskSolarSystem_CometLoss::doWork() {}
void TaskSolarSystem_CometLoss::imGui() {}
const char *TaskSolarSystem_CometLoss::toString() const {
    std::stringstream ss;
    ss << "Description:"
       << "\n";
    ss << "Simulates the breakup of comets from solar radiation"
       << "\n";

    m_string = ss.str();
    return m_string.c_str();
}
void TaskSolarSystem_CometLoss::draw() const {}
