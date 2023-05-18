#pragma once

#include <string>

#include "../Context/GlobalEnvironment.h"
#include "ITask.h"

class TaskMiniProject_Integrator : public CTask {
  public:
    TaskMiniProject_Integrator() : CTask() {}

    // ITask
    virtual const char *name() const override {
        return "TaskMiniProject_Integrator";
    }
    virtual void setForces() override;
    virtual void doWork() override;
    virtual void draw() const override;
    virtual void imGui() override;
    virtual const char *toString() const override;

  private:
    void RK2_Midpoint();
    int m_passNumber = 0;
    void ensureSizeOfTemps();
    size_t size() const { return (int)gEnv->solarSystemPS.getParticleCount(); }

    mutable std::vector<glm::vec<3, long double>> m_tempPos;
    mutable std::vector<glm::vec<3, long double>> m_tempVel;
};
