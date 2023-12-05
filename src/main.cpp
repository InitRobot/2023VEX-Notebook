#include "vex.h"
#include "basic-functions.h"
#include "parameters.h"
#include "autonomous.h"
#include "my-timer.h"
#include "controller.h"
#include "cstdlib"
#include "iostream"
#include "GPS.h"
#include "sensors.h"
#include "adjusment.h"

using namespace vex;
competition Competition;

void pre_auton(void) {
  vexcodeInit();
}

void autonomous(void) {
  switch(auton_strategy) {
    case 0:
      Brain.Screen.print("%10s", "auto one");
      break;
    case 1:
      Brain.Screen.print("%10s", "auto two");
      break;
    case 2:
      Brain.Screen.print("%10s", "auto three");
      break;
    case 3:
      Brain.Screen.print("%10s", "auto four");
      break;
    case 4:
      Brain.Screen.print("%10s", "yousb");
      auton_sb();
      break;
  }
}

void usercontrol(void) {
  // User control code here, inside the loop
  bool is_base_locked = false;
  while (1) {
    // Controller Input
    defineController();
    // Base Movement Control
  A1 = A1 * abs(A1) / 100.0 * SENSITIVITY_TURN;//对转弯做非线性处理
  if (std::abs(A1) < JOYSTICK_DEADZONE)
    A1 = 0;
  if (std::abs(A2) < JOYSTICK_DEADZONE)
    A2 = 0;
  if (std::abs(A1 + A2) > MOVEMENT_LOWER_LIMIT)
    moveLeft(A1 + A2);
  else
    unlockLeft();
  if (std::abs(A1 - A2) > MOVEMENT_LOWER_LIMIT)
    moveRight(A1 - A2);
  else
    unlockRight();
  }

  // Base lock
  if(B && !last_B) {
    is_base_locked = !is_base_locked;
    if(is_base_locked) {
      lockBase();
    }
    else {
      unlockBase();
    }
  }

  if(DOWN && !last_DOWN) {
    runAuton();
    // tuning_robot();
  }

  // Print on brain
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.print("Heading: %3.2f", my_sensors.getBaseHeading());
  Brain.Screen.setCursor(2, 1);
  Brain.Screen.print("Forward Position: %4.1f", my_sensors.getBaseForwardPos());
  Brain.Screen.setCursor(3, 1);
  this_thread::sleep_for(5);
}

int main() {
  thread AutonSensors(autonSensors);
  thread AutonGps(autonGPS);
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  // Run the pre-autonomous function.
  pre_auton();
  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}