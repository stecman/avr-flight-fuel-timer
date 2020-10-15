#pragma once

#include <stdint.h>
#include "control/rtc.h"

// SARTIME in the next 24 hours
typedef struct SARTime {
    uint8_t utcHour;
    uint8_t utcMinute;
} SARTime;

typedef enum TimerRunState {
    // Timer is not running. User can configure all aspects of device
    kTimerRunState_Off = 0,

    // Timer configuration is locked. Awaiting engine state
    kTimerRunState_Armed,

    // Engine is running on the ground
    kTimerRunState_EngineRun,

    // Engine is running in flight
    kTimerRunState_InFlight,
} TimerRunState;

typedef enum TimerRunEvent {
    // The user has requested to leave the current state
    kTimerRunEvent_UserDeactivate = 0,

    // The user has requested to activate the next state
    kTimerRunEvent_UserActivate = 1,
} TimerRunEvent;

typedef enum FuelTank {
    // Draw from all tanks equally
    kTank_XFeed = 0,

    // Single tank identifiers
    kTank_Left,
    kTank_Right,
} FuelTank;

typedef struct SystemState {

    // Amount of fuel in each tank (UQ8.24 fixed point)
    uint32_t fuelQuantityLeft;
    uint32_t fuelQuantityRight;

    // Tank selection
    FuelTank selectedTank;
    RtcTime lastTankChange;

    // Operation state the device is in
    TimerRunState runState;

    SARTime sartime;
} SystemState;