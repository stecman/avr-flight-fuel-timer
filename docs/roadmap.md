# Fuel timer features

## Core functionality

* Display time remaining in each tank
* Swtich active tank (including crossfeed)
* Pre-flight programming state allowing at least configuration of time in each tank
* Start flight action
* End flight action (to prevent warning features tripping)
* Keep accurate time and state across loss of main power and reset.

## Nice to haves

* Display elapsed flight time
* Display elapsed time on current tank
* Show elapsed time vs. reserve time vs. total remaining time
* Show use of reserve time differently visually (more urgent)
* Display flight start time
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
* Configure fuel change time
* Configure date and time
* Display time in UTC
* SAR time monitoring with audible warnings to update/terminate (active even outside of flight mode)

## Other (probably feature creep)

* Multiple count up/down timers for navigation
* Scroll through log of fuel changes this flight
* Log previous flights (duration), aircraft registration / "totals flight log"
* Allow entering fuel quantities and have the program work out the time per
  tank. At minimum, this could be a single screen where instead of configuring
  time for two tanks, you config quantity in each tank, plus burn per hour and
  unusable fuel. The burn per hour and unusable could be recalled from EEPROM.
* Low power mode where the user has to wake with an interupt to see the display
  for five seconds at a time.
  could show time remaining in current tank/flight, or some configurable value.
