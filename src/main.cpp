#include "vex.h"

using namespace vex;

//循环内变量需要提前定义，在循环内重新定义会增加运算量
float axis1,axis2,axis3,axis4,chassis_L_volt,chassis_R_volt;

//定义电压上限
const int VOLTAGE3 = 128;//3位数，方便*100
const int VOLTAGE5 = 12800;//5位数

float sensitivity=1;//底盘移动灵敏度(0,1]

competition Competition;//定义比赛
controller Controller1 = controller(primary);//定义遥控器

//定义电机，详见README
//L:left R:right F:front B:back U:up D:down
motor chassis_LF = motor(PORT1, ratio6_1, true);
motor chassis_LBU = motor(PORT2, ratio6_1, false);
motor chassis_LBD = motor(PORT3, ratio6_1, true);
motor_group chassis_L = motor_group(chassis_LF, chassis_LBU, chassis_LBD);
motor chassis_RF = motor(PORT4, ratio6_1, true);
motor chassis_RBU = motor(PORT5, ratio6_1, false);
motor chassis_RBD = motor(PORT6, ratio6_1, true);
motor_group chassis_R = motor_group(chassis_RF, chassis_RBU, chassis_RBD);

//底盘移动函数，添加了防止过载的处理
void chassis_move(float left_volt, float right_volt)
{
  if (left_volt > VOLTAGE5)
  {
    chassis_L.spin(forward, VOLTAGE5, voltageUnits::mV);
    chassis_R.spin(forward, right_volt / left_volt * VOLTAGE5, voltageUnits::mV);
  }
  else if(right_volt > VOLTAGE5)
  {
    chassis_L.spin(forward, left_volt / right_volt * VOLTAGE5, voltageUnits::mV);
    chassis_R.spin(forward, VOLTAGE5, voltageUnits::mV);
  }
  else
  {
    chassis_L.spin(forward, left_volt, voltageUnits::mV);
    chassis_R.spin(forward, right_volt, voltageUnits::mV);
  }
  return ;
}

//Pre-Autonomous Functions
void pre_auton(void) 
{
  
}

//Autonomous Functions
void autonomous(void) 
{

}

//User Contorl Functions
void usercontrol(void) {
  // User control code here, inside the loop
  while (1) 
  {
    //更新摇杆位置
    axis1 = Controller1.Axis1.position(percent);
    axis2 = Controller1.Axis2.position(percent);
    axis3 = Controller1.Axis3.position(percent);
    axis4 = Controller1.Axis4.position(percent);

    /*
    //调试用：输出摇杆位置
    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(4,1);
    Controller1.Screen.print("%f %f", axis1, axis2);
    wait(50, msec);
    */

    //底盘移动
    chassis_L_volt = VOLTAGE3 * axis1 + VOLTAGE3 * axis2 * sensitivity;
    chassis_R_volt = VOLTAGE3 * axis1 - VOLTAGE3 * axis2 * sensitivity;
    chassis_move(chassis_L_volt, chassis_R_volt);
    //chassis_L.spin(forward, axis1*VOLTAGE3, voltageUnits::mV);
    //chassis_R.spin(forward, axis2*VOLTAGE3, voltageUnits::mV);

    wait(20, msec); // Sleep the task for a short amount of time to prevent wasted resources.
  }
}

// Main will set up the competition functions and callbacks.
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) 
  {
    wait(100, msec);
  }
}
