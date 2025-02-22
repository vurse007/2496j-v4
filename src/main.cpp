#include "main.h"
#include "global.hpp"
#include "pid.hpp"
#include "util.hpp"
using namespace glb;
using namespace pros;

bool mogoState;
bool releaseRecorded;
double releaseTime;
double triggerTime;

void driverProfileAyush(){
	//tank control below
	double rightstick = con.get_analog(E_CONTROLLER_ANALOG_RIGHT_Y);
	double leftstick = con.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);

	rchassis.move(rightstick);
	lchassis.move(leftstick);

	//intake below - only give a specific velocity to move if intake does not need to be interrupted by stall and color sort tasks
	if (con.get_digital(E_CONTROLLER_DIGITAL_R1) && intakeInterrupt == false){
		intake.move(127);
	}
	else if(con.get_digital(E_CONTROLLER_DIGITAL_R2) && intakeInterrupt == false){
		intake.move(-127);
	}
	else{
		if (intakeInterrupt == false){
			intake.move(0);
		}
	}

	//lady brown code below
	if (con.get_digital_new_press(E_CONTROLLER_DIGITAL_UP)){
		lbPID = true;
	}
	
	else{
		if (con.get_digital(E_CONTROLLER_DIGITAL_L1)){
			ldb.move(127);
			lbPID = false;
		}
		else if (con.get_digital(E_CONTROLLER_DIGITAL_L2)){
			ldb.move(-127);
			lbPID = false;
		}
		else{
			ldb.move(0);
		}
	}

	//clamp and auto clamp code
	if (con.get_digital_new_press(E_CONTROLLER_DIGITAL_B)){
		mogoState = !mogoState;
		if (mogoState == true){
			mogo.set_value(true);
		}
		else{
			mogo.set_value(false);
			releaseRecorded = false;
		}
	}
	if (mogoState == false && releaseRecorded == false){
		releaseTime = pros::millis();
		releaseRecorded = true;
	}
	if (autoclamp.get() <= 200){
		triggerTime = pros::millis();
	}
	if (autoclamp.get() <= 200 && (pros::millis() - triggerTime) >= 0.2 && (pros::millis() - releaseTime) > 2.00){
		mogoState = true;
		mogo.set_value(true);
	}

	//doinker code below
	doinker.set_value((con.get_digital(E_CONTROLLER_DIGITAL_LEFT)));

}

void driverProfileManu(){
	//reg arcade control below
	double power = con.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
	double turn = con.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);

	rchassis.move(power - turn);
	lchassis.move(power + turn);


	//intake below
	if (con.get_digital(E_CONTROLLER_DIGITAL_R1)){
		intake.move(127);
	}

	else if(con.get_digital(E_CONTROLLER_DIGITAL_R2)){
		intake.move(-127);
	}

	else{
		intake.move(0);
	}

	//lady brown code below
	if (con.get_digital_new_press(E_CONTROLLER_DIGITAL_UP)){
		//lbPID = true;
	}
	
	else{
		if (con.get_digital(E_CONTROLLER_DIGITAL_L1)){
			ldb.move(127);
			//lbPID = false;
		}
		else if (con.get_digital(E_CONTROLLER_DIGITAL_L2)){
			ldb.move(-127);
			//lbPID = false;
		}
		else{
			ldb.move(0);
		}
	}

	//clamp and auto clamp code
	if (con.get_digital_new_press(E_CONTROLLER_DIGITAL_B)){
		mogoState = !mogoState;
		if (mogoState == true){
			mogo.set_value(true);
		}
		else{
			mogo.set_value(false);
			releaseRecorded = false;
		}
	}

	//doinker code below
	doinker.set_value((con.get_digital(E_CONTROLLER_DIGITAL_LEFT)));
}


/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);

	ldb.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

	//pros::Task stallProtector(stallProtection);
	pros::Task ladyBrownMacroTask(ladyBrownTask);

	//updating PID objects
	default_drive_pid.update_constants(10, 100, 10, 300, 10000, 127);
	heading_correction_pid.update_constants(1, 0, 0, 15, 10000, 127);

	//updating tPoly objects
	driveTimeoutTPOLY.update_coefficients({5000});
	turnTimeoutTPOLY.update_coefficients({5000});
	driveKDTPOLY.update_coefficients({10});
	turnKDTPOLY.update_coefficients({5});
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
void autonomous() {
	PID driveStraight(0.5,0,300,1000,1000,70);   //drive straight
	//drive(1000, M_TICKS, 5000, 0, std::nullopt, &test);
	PID dirveMogo(1.2,0,0.1,1000,1000,127);      //drive mogo
	// mogo.set_value(true);
	// delay(300);
	// drive(1000, M_TICKS, 5000, 0, std::nullopt, &test);
	PID turnNormal(13,0,0.85,1000,1000,127);    // turnPID at ALL VALUES
	// turn(180, 15000,0,std::nullopt, &test);
	PID turnMogo(12,0.2,1,1000,1000,127);  // turnPIDMogo up to 120
	drive(-4000, M_TICKS, 5000, 0,  std::nullopt, &driveStraight);
	delay(300);
	mogo.set_value(true);
	turn(150,15000,0,std::nullopt,&turnMogo);	
	  
	//turn(150, 15000,0,std::nullopt, &test);

	
}

/**
 * Runs the operator control code. This function will be sta
 * rted in its own task
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

	chassis.set_brake_modes(E_MOTOR_BRAKE_COAST);

	while (true) {
		driverProfileManu();

		delay(5);
	}
}