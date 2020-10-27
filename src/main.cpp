#include "main.h"

#define DEADZONE(x) fabs(x)<15?0:x

using namespace pros;
Controller master(E_CONTROLLER_MASTER);
Motor leftMtr(1);
Motor rightMtr(2);
Motor elevatorMtr(3);
Motor switchMtr(4);
Motor leftIntake(5);
Motor rightIntake(6);
Motor bottomIntake(7);


bool elevatorToggle = false;
int eleVar = 0;


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	lcd::initialize();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	Controller master(E_CONTROLLER_MASTER);
	Motor leftMtr(1);
	Motor rightMtr(2);
	Motor elevatorMtr(3);
	Motor switchMtr(4);
	Motor leftIntake(5);
	Motor rightIntake(6);
	Motor bottomIntake(7);

	leftMtr.set_brake_mode(MOTOR_BRAKE_BRAKE);
	rightMtr.set_brake_mode(MOTOR_BRAKE_BRAKE);
	elevatorMtr.set_brake_mode(MOTOR_BRAKE_BRAKE);
	switchMtr.set_brake_mode(MOTOR_BRAKE_BRAKE);
	elevatorMtr.set_gearing(MOTOR_GEARSET_6);
	switchMtr.set_gearing(MOTOR_GEARSET_6);

	while (true) {
/*--------------------------------------------
Drive Train
----------------------------------------------*/
		int power = DEADZONE(master.get_analog(ANALOG_LEFT_Y) * (200/128.0));
		int turn = DEADZONE(master.get_analog(ANALOG_RIGHT_X) * (200/128.0));
		int left = power + turn;
	 	int right = power - turn;
		leftMtr.move_velocity(left);
	 	rightMtr.move_velocity(right);
/*--------------------------------------------
Elevator Control
----------------------------------------------*/
		if (master.get_digital(DIGITAL_R1)) {
			leftIntake.move_velocity(200);
			rightIntake.move_velocity(-200);
			bottomIntake.move_velocity(200);
		} else if (master.get_digital(DIGITAL_R2)) {
			leftIntake.move_velocity(-200);
			rightIntake.move_velocity(200);
			bottomIntake.move_velocity(-200);
		} else {
			leftIntake.move_velocity(0);
			rightIntake.move_velocity(0);
			bottomIntake.move_velocity(0);
		}
/*--------------------------------------------
Elevator Toggle
----------------------------------------------*/
		if (master.get_digital_new_press(DIGITAL_X)) {
			eleVar = 1 - eleVar;
		}
/*--------------------------------------------
Switch Control
----------------------------------------------*/
		if (master.get_digital(DIGITAL_L2)){
		  elevatorToggle = true;
		} else {
			elevatorToggle = false;
		}
		/*--------------------------------------------
Elevator Control
----------------------------------------------*/
		if (elevatorToggle == true && eleVar == 1) {
			switchMtr.move_velocity(600);
			elevatorMtr.move_velocity(600);
		} else if (elevatorToggle == false && eleVar == 1){
			switchMtr.move_velocity(-600);
			elevatorMtr.move_velocity(600);
		} else {
			switchMtr.move_velocity(0);
			elevatorMtr.move_velocity(0);
		}


/*	if (master.get_digital(DIGITAL_L1)) {
		elevatorMtr.move_velocity(600);
		switchMtr.move_velocity(-600);
	} else if (master.get_digital(DIGITAL_L2)) {
		elevatorMtr.move_velocity(-600);
			switchMtr.move_velocity(600);

	} else {
		elevatorMtr.move_velocity(0);
			switchMtr.move_velocity(0);
	}

*/
		delay(20);
	}
}
