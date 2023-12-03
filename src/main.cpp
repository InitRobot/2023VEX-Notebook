#include "vex.h"

using namespace vex;

const int VOLTAGE3 = 128;//3位数电压上限，方便*100
const int VOLTAGE5 = 12800;//5位数电压上限
float sensitivity = 1, sensitivity_turn = 0.5;//底盘移动灵敏度、底盘转向灵敏度(0,1]
int roller_dir = 0;//滚筒方向 -1外 0停 1内

//循环内变量需要提前定义，在循环内重新定义会增加运算量
float axis1,axis2,axis3,axis4,chassis_L_volt,chassis_R_volt;

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
motor chassis_RBD = motor(PORT9, ratio6_1, true);
motor_group chassis_R = motor_group(chassis_RF, chassis_RBU, chassis_RBD);
motor roller = motor(PORT7, ratio18_1, false);

//底盘移动函数，添加了防止过载的处理
void chassis_move()
{
  chassis_L_volt = sensitivity * (VOLTAGE3 * axis1 * abs(axis1) / 100.0 * sensitivity_turn + VOLTAGE3 * axis2);
  chassis_R_volt = sensitivity * (VOLTAGE3 * axis1 * abs(axis1) / 100.0 * sensitivity_turn - VOLTAGE3 * axis2);
  if (chassis_L_volt > VOLTAGE5)
  {
    chassis_L.spin(forward, VOLTAGE5, voltageUnits::mV);
    chassis_R.spin(forward, chassis_R_volt / chassis_L_volt * VOLTAGE5, voltageUnits::mV);
  }
  else if(chassis_R_volt > VOLTAGE5)
  {
    chassis_L.spin(forward, chassis_L_volt / chassis_R_volt * VOLTAGE5, voltageUnits::mV);
    chassis_R.spin(forward, VOLTAGE5, voltageUnits::mV);
  }
  else
  {
    chassis_L.spin(forward, chassis_L_volt, voltageUnits::mV);
    chassis_R.spin(forward, chassis_R_volt, voltageUnits::mV);
  }
  return;
}

// 摇杆位置更新
void axis_update()
{
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
  return;
}

//滚筒向外移动
void roller_up()
{
  if(roller_dir!=-1) roller_dir--;
  Controller1.Screen.print("%d", roller_dir);
  return;
}

//滚筒向内移动
void roller_down()
{
  if(roller_dir!=1) roller_dir++;
  Controller1.Screen.print("%d", roller_dir);
  return;
}

//滚筒移动
void roller_move()
{
  //若按钮按下则更新roller_dir，详见README
  Controller1.ButtonUp.pressed(roller_up);
  Controller1.ButtonDown.pressed(roller_down);
  //更新旋转方向
  roller.spin(forward, VOLTAGE5*roller_dir, voltageUnits::mV);
  return;
}

//Pre-Autonomous Functions
void pre_auton(void) 
{
  //刹车模式滑行
  chassis_L.setStopping(coast);
  chassis_R.setStopping(coast);

}

//Autonomous Functions
void autonomous(void) 
{
  //刹车模式回位
  chassis_L.setStopping(hold);
  chassis_R.setStopping(hold);
}

//User Contorl Functions
void usercontrol(void) {
  //刹车模式滑行
  chassis_L.setStopping(coast);
  chassis_R.setStopping(coast);

  // User control code here, inside the loop
  while (1) 
  {
    axis_update();//更新摇杆位置
    chassis_move();//底盘移动
    roller_move();//滚筒移动

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
