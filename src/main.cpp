/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\zhuye                                            */
/*    Created:      Wed Oct 18 2023                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// Motor18              motor         18              
// Motor10              motor         10              
// Motor1               motor         1               
// Motor2               motor         2               
// Motor3               motor         3               
// Motor4               motor         4               
// Motor5               motor         5               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

int main() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  
  while(1){
    ///////////////底盘//////////////////////////////////////////////    
    float axis3 = Controller1.Axis3.position(percent);
    float axis4 = Controller1.Axis4.position(percent);

    float trans_fb = axis3;
    float rot = axis4;

    Motor1.spin(forward, 128*trans_fb + 95*rot ,voltageUnits::mV);
    Motor2.spin(forward, -128*trans_fb + 95*rot ,voltageUnits::mV);  
    
    wait(5,msec);
    ///////////////////////////////旋臂/////////////////////////////////
    float axis2 = Controller1.Axis2.position(percent);
    
    float rot_xuan_bi = axis2;

    Motor3.spin(forward, -95*rot_xuan_bi ,voltageUnits::mV);
    Motor4.spin(forward, 95*rot_xuan_bi ,voltageUnits::mV);  
    
    wait(5,msec);
    //////////////////////收集/////////////////////////////////////////
    if (Controller1.ButtonR2.pressing()) {
      Motor5.spin(forward, 10.0, volt);
    }
    if (Controller1.ButtonL2.pressing()) {
      Motor5.stop();
    }
    wait(5, msec);
  }
}
