#pragma once

#include <glm/glm.hpp>
#include <map>

#include "ITask.h"

class TaskSolarSystem_CopySolarSystemPSToNativePS : public CTask {
  public:
    TaskSolarSystem_CopySolarSystemPSToNativePS() : CTask() {}

    // ITask
    virtual const char *name() const override {
        return "TaskSolarSystem_CopySolarSystemPSToNativePS";
    }
    virtual void setForces() override;
    void findNamedParticles(int &count);
    virtual void doWork() override;
    virtual void draw() const override;
    virtual void imGui() override;
    virtual const char *toString() const override;

  private:
    int m_particleCount;
    int m_psId = 0;
    std::vector<int> m_particleNameIndices;
    std::vector<std::string> m_particleNames;
    // Scale rendering so that, 1 billion (10^9) km equals 1 unit
    // rendering
    long double m_scalingFactor = (long double)1 / (long double)1e9;
    int m_selectedCenterBody = 0;
};
