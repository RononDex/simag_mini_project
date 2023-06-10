#pragma once

#include <string>

#include "ITask.h"

class TaskSolarSystem_CometLoss : public CTask {
  public:
    TaskSolarSystem_CometLoss() : CTask() {}

    // ITask
    virtual const char *name() const override {
        return "TaskSolarSystem_CometLoss";
    }
    virtual void setForces() override;
    virtual void doWork() override;
    virtual void draw() const override;
    virtual void imGui() override;
    virtual const char *toString() const override;

  private:
    float m_accumulatedMassLossKg = 0;
    float m_massLossThresholdNewParticle = 50000;
    float m_sunIntensity = 1.0;
    long double m_appliedForceOnMassLoss = 50;

    // Calculated through solving 1/(1AU)^2 * x = 1365
    long double m_intensityConstant = 3.021236E18L;
    // How much mass is lost per hour per watt?
    long double m_massToIntensityRatio = 3;
};
