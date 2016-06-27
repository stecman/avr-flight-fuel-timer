# Fuel timer features

## Core

* Display time remaining in each tank
* Swtich active tank
  Fuel can be drained from one tank at a time. The user selects the tank that is
  currently selected in the aircraft, which causes that tank's endurance timer
  to count down.
* Pre-flight programming state allowing at least configuration of time in each tank
* Start flight action
* End flight action (to prevent warning features tripping)

## Useful (automatic)

* Display elapsed flight time
* Display elapsed time on current tank
* Indicator light when near and at fuel change time. Blinking light when in
  reserve time.
* Show elapsed time vs. reserve time vs. total remaining time
* Show in reserve time differently visually (more urgent)
* Display flight start time
* Allow pausing a flight and adding time to tanks
* Protect against accidental changes to tank quantity
* Automatic fuel balancing - alter user configured fuel change time in flight to
  balance fuel in tanks.
* Sleep between updates to save power? We only need to be awake to update the
  screen every few seconds during a flight.
* Alert sound loud enough to be heard in the cockpit:
  * short chirp to indicate it's time to switch tanks, or time is overdue
  * a more worrying alert to notify entering reserve time, plus a repeated
    alert every five or ten minutes to remind the pilot of the status.
  * persistent alarm should sound when running dangerously low on tank time
    or on total remaining flight time. For tank time where there is adequete
    fuel remaining in the other tank, the alarm may not be dismissable. For
    other cases, the persistent alarm may be silenced to a chirp every minute.

## Useful (some manual config/action required)

* Configure fuel change time
* Configure date and time
* Scroll through log of fuel changes this flight

## Other

* Persist state across Arduino power outages (aka. store start time, start
  config and fuel changes in EEPROM so they can be recovered on power restored).
  This is useful to stop bumps that may dislodge the battery, an accidental
  power off, or a change of battery from interrupting an in progress flight.
* Log previous flights (duration), aircraft registration / "totals flight log"
* Allow entering fuel quantities and have the program work out the time per
  tank. At minimum, this could be a single screen where instead of configuring
  time for two tanks, you config quantity in each tank, plus burn per hour and
  unusable fuel. The burn per hour and unusable could be recalled from EEPROM.
* Low power mode where the user has to trigger an interupt to see the display
  for five seconds at a time.
