#pragma once

#include <glm/glm.hpp>

#include "ITask.h"

class TaskMiniProject_CopySolarSystemPSToNativePS : public CTask {
  public:
    TaskMiniProject_CopySolarSystemPSToNativePS() : CTask() {}

    // ITask
    virtual const char *name() const override {
        return "TaskMiniProject_CopySolarSystemPSToNativePS";
    }
    virtual void setForces() override;
    virtual void doWork() override;
    virtual void draw() const override;
    virtual void imGui() override;
    virtual const char *toString() const override;

  private:
    bool m_enable = true;
    int m_particleCount;
    int m_psId = 0;
    // Scale rendering so that, 1 billion (10^9) km equals 1 unit
    // rendering
    long double m_scalingFactor = (long double)1 / (long double)1e9;
};
