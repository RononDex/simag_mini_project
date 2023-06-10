#pragma once

#include <string>

#include "ITask.h"

class TaskSolarSystem_SetupSolarSystem : public CTask {
  public:
    TaskSolarSystem_SetupSolarSystem() : CTask() {}

    // ITask
    virtual const char *name() const override { return "TaskSolarSystem_SetupSolarSystem"; }
    virtual void setForces() override;
    virtual void doWork() override;
    virtual void draw() const override;
    virtual void imGui() override;
    virtual const char *toString() const override;

  private:
    void setupSolarSystem(bool comets);
};
