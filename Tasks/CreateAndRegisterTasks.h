#include "../Context/App.h"
#include "../Globals.h"

// Base Tasks
#include "TaskBase.h"
#include "TaskCreatePsFromImage.h"
#include "TaskEmpty.h"
#include "TaskEnqueuePS.h"
#include "TaskMiniProject_CopySolarSystemPSToNativePS.h"
#include "TaskMiniProject_Integrator.h"
#include "TaskMiniProject_SetupSolarSystem.h"
#include "TaskMouseAddVelocity.h"
#include "TaskPickParticle.h"
#include "TaskTools.h"

// Lecture Tasks
#include "TaskLect02_MyFirstTask_Differentiation.h"

#include "TaskLect03_Billiard.h"
#include "TaskLect03_CarCollision.h"

#include "TaskLect04_AddForce.h"
#include "TaskLect04_CircularMotion_Integration.h"
#include "TaskLect04_Pendula.h"

#include "TaskLect05_Springs.h"

#include "TaskFlocking.h"

// Assignment Tasks
#include "TaskAssignment_Fireworks.h"
#include "TaskAssignment_GravityPlanets.h"
#include "TaskAssignment_Morph.h"

#include "TaskCollision.h"
#include "TaskCollisionFloor.h"

// To add new tasks, the following 2 steps are needed:
// 1. Add new tasks here (as '#include "TaskMyTask.h')
// 2. Register new task below in createTasks(){...}

namespace {

void createTasks(App *app) {
    // Base Tasks
    app->registerTask<TaskEmpty>();
    app->registerTask<TaskBase>();
    app->registerTask<TaskTools>();
    app->registerTask<TaskPickParticle>();
    app->registerTask<TaskEnqueuePS>();
    app->registerTask<TaskCreatePsFromImage>();
    app->registerTask<TaskMouseAddVelocity>();

    // Lecture Tasks
    app->registerTask<TaskLect02_MyFirstTask_Differentiation>();

    app->registerTask<TaskLect03_CarCollision>();
    app->registerTask<TaskLect03_Billiard>();

    app->registerTask<TaskLect04_AddForce>();
    app->registerTask<TaskLect04_CircularMotion_Integration>();
    app->registerTask<TaskLect04_Pendula>();

    app->registerTask<TaskLect05_Springs>();

    // Assignment Tasks
    app->registerTask<TaskAssignment_Morph>();
    app->registerTask<TaskAssignment_Fireworks>();
    app->registerTask<TaskAssignment_GravityPlanets>();

    app->registerTask<TaskFlocking>();

    // Assignment Tasks
    app->registerTask<TaskAssignment_Morph>();
    app->registerTask<TaskAssignment_Fireworks>();
    app->registerTask<TaskAssignment_GravityPlanets>();
    app->registerTask<TaskCollision>();
    app->registerTask<TaskCollisionFloor>();

    // Register your own tasks here...
    // Mini Project
    app->registerTask<TaskMiniProject_Integrator>();
    app->registerTask<TaskMiniProject_CopySolarSystemPSToNativePS>();
    app->registerTask<TaskMiniProject_SetupSolarSystem>();
}

void setDefaultTasks(App *app) {
    // Called on startup, you might setup the start-up-task-configuration here.
    // Thus, the tasks would not be initialized as 'empty'.
    // Just push the tasks, which you would like to show up on startup.

    // Examples:

    // Setup Tools
    // app->pushDefaultTask<TaskPickParticle>();
    // app->pushDefaultTask<TaskTools>();
    // app->pushDefaultTask<TaskEnqueuePS>();

    // Setup Pendula
    // app->pushDefaultTask<TaskPickParticle>();
    // app->pushDefaultTask<TaskLect04_Pendula>();

    // Setup Gravity Planets
    // app->pushDefaultTask<TaskAssignment_GravityPlanets>();
    // app->pushDefaultTask<TaskEnqueuePS>();

    // Setup Fireworks
    // app->pushDefaultTask<TaskAssignment_Fireworks>();
    // app->pushDefaultTask<TaskLect04_AddForce>();
    //
    // Assignment 2 Task 2 Cloth collisions
    /* app->pushDefaultTask<TaskPickParticle>(); */
    /* app->pushDefaultTask<TaskLect05_Springs>(); */
    /* app->pushDefaultTask<TaskCollision>(); */
    /* app->pushDefaultTask<TaskCollisionFloor>(); */
    /* app->pushDefaultTask<TaskLect04_AddForce>(); */

    // MiniProject SolarSystemSimulation
    app->pushDefaultTask<TaskMiniProject_SetupSolarSystem>();
    app->pushDefaultTask<TaskMiniProject_CopySolarSystemPSToNativePS>();
    app->pushDefaultTask<TaskMiniProject_Integrator>();
    app->pushDefaultTask<TaskPickParticle>();
}
} // namespace
