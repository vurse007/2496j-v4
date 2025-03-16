#include "main.h"
#include "global.hpp"
#include "pid.hpp"
#include "util.hpp"
#include "autons.hpp"
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

	//intake below
	if (con.get_digital(E_CONTROLLER_DIGITAL_R1)){
		intake.move(127);
		//stallProtection();
	}

	else if(con.get_digital(E_CONTROLLER_DIGITAL_R2)){
	 	intake.move(-127);
	}

	else{
	 	intake.move(0);
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
	if (autoclamp.get() <= 60){
		triggerTime = pros::millis();
	}
	if (autoclamp.get() <= 60 && (pros::millis() - triggerTime) >= 0.2 && (pros::millis() - releaseTime) > 2.00){
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

	eject_b=false;

	rchassis.move(power - turn);
	lchassis.move(power + turn);
	

	//intake below
	if (con.get_digital(E_CONTROLLER_DIGITAL_R1)){
		
		stallProtection();
	}

	else if(con.get_digital(E_CONTROLLER_DIGITAL_R2)){
	 	intake.move(-127);
	}

	else{
	 	intake.move(0);
	}

	//lady brown code below
	if (con.get_digital_new_press(E_CONTROLLER_DIGITAL_UP)){
		lbPID = 1;     //this is just so we pass insepction.
	}
	
	else{
		if (con.get_digital(E_CONTROLLER_DIGITAL_L1)){
			ldb.move(127);
			lbPID = 0;
		}
		else if (con.get_digital(E_CONTROLLER_DIGITAL_L2)){
			ldb.move(-127);
			lbPID = 0;
		}
		else{
			ldb.move(0);
		}
	}

	if (con.get_digital_new_press(E_CONTROLLER_DIGITAL_A)){
		heading_correction_pid.kP = 0;
		heading_correction_pid.kI = 0;
		heading_correction_pid.kD = 0;
		drive(-280, M_TICKS, 1000, 0, 50);
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

int autonSequencer = 0;
bool autonFlag = false;
bool timerStarted = false;
timer autonSelectorTimer(3000);
timer autonTotalTimer(15000);

void initialize() {
	pros::lcd::initialize();
	con.clear();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);

	ldb.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

	colorsort.set_integration_time(3);

	//starting tasks
	pros::Task stallProtector(stallProtection);
	pros::Task ladyBrownMacroTask(ladyBrownTask);
	//pros::Task cs_eject_blueTask(eject_blue);
	//pros::Task cs_eject_blueTask(cs_eject_blue);
	//pros:: Task csauto_eject_redTask(csauto_eject_red);
	//pros:: Task csauto_eject_blueTask(csauto_eject_blue);


	//updating PID objects
	default_drive_pid.update_constants(0.8, 0.03, 15.4, 1000, 1000, 70);
	default_drive_mogo_pid.update_constants(0.8, 0.03, 15.6,1000,1000,70);
	heading_correction_pid.update_constants(0.5,0,0,1000,1000,127);
	default_turn_pid.update_constants(3,0.0001,24.2,1000, 1000, 127);
	default_turn_mogo_pid.update_constants(4,0.0007,36,1000, 1000, 127);
	default_arc_pid.update_constants(0.8, 0.03, 15.4, 1000, 1000, 70);
	default_arc_mogo_pid.update_constants(0.8, 0.03, 15.6,1000,1000,70);

	//updating tPoly objects
	driveTimeoutTPOLY.update_coefficients({1800});
	turnTimeoutTPOLY.update_coefficients({2000});

	turnMogoKDTPOLY.update_coefficients({
		tPoly::scientificNotation(5.90104, -9),
		-0.00000320986,
		0.000653145,
		-0.06051,
		2.50143,
		0.0501999
	});

	turnKDTPOLY.update_coefficients({
		tPoly::scientificNotation(-5.21982, -10),  // x^5
		tPoly::scientificNotation(-2.80159, -8),   // x^4
		tPoly::scientificNotation(0.0000858705, 0),// x^3
		tPoly::scientificNotation(-0.017407, 0),   // x^2
		tPoly::scientificNotation(1.24785, 0),     // x
		tPoly::scientificNotation(-3.16719, 0)     // constant
	});

	driveKDTPOLY.update_coefficients({
		15.4
	}); //constant kd value

	driveMogoKDTPOLY.update_coefficients({
		15.6
	}); //constant kd value


	//auton selector with lim
	// autonTotalTimer.start();
	// while (autonFlag == false){

	// 	if (autonTotalTimer.targetReached() == true){
	// 		break;
	// 	}

	// 	//no more than five options
	// 	if (autonSequencer > 5){
	// 		autonSequencer = 0;
	// 	}

	// 	if (autoselect.get_value() == true){
	// 		delay(500);
	// 		autonSequencer++;
	// 		timerStarted = false;
	// 	}

	// 	if (autonSequencer == 0){
	// 		pros::lcd::set_text(2, "No Auto Selected");
	// 		con.print(0, 0, "No Auto Selected");
	// 	}
	// 	else if (autonSequencer == 1){
	// 		pros::lcd::set_text(2, "Red Goal");
	// 		con.print(0, 0, "Red Goal");
	// 	}
	// 	else if (autonSequencer == 2){
	// 		pros::lcd::set_text(2, "Blue Goal");
	// 		con.print(0, 0, "Blue Goal");
	// 	}
	// 	else if (autonSequencer == 3){
	// 		pros::lcd::set_text(2, "Red Ring");
	// 		con.print(0, 0, "Red Ring");
	// 	}
	// 	else if (autonSequencer == 4){
	// 		pros::lcd::set_text(2, "Blue Ring");
	// 		con.print(0, 0, "Blue Ring");
	// 	}
	// 	else if (autonSequencer == 5){
	// 		pros::lcd::set_text(2, "Skills");
	// 		con.print(0, 0, "Skills");
	// 	}
		

	// 	if (autonSequencer > 0 && timerStarted == false){
	// 		autonSelectorTimer.start();
	// 		timerStarted = true;
	// 	}
	// 	else if (autonSelectorTimer.targetReached() == true && timerStarted == true){
	// 		//if you stayed on one for long enough
	// 		con.rumble(".");
	// 		autonFlag = true;
	// 	}
	// }
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
 * his task will exit when the robot is enabled and autonomous or opcontrol
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

	// if (autonSequencer == 0){
	// 	//if u didnt press the ls, skip auto
	// }
	// else if(autonSequencer == 1){
	// 	//red goal
	// 	redGoal();
	// }
	// else if(autonSequencer == 2){
	// 	//blue goal
	// 	blueGoal();
	// }
	// else if(autonSequencer == 3){
	// 	//red ring
	// 	redRing();
	// }
	// else if(autonSequencer == 4){
	// 	//blue ring
	// 	blueRing();
	// }
	// else if(autonSequencer == 5){
	// 	//skills
	// 	skills();
	// }

	skills();
	
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
	con.clear();

	lbPID = false;
	ladyBrownCorrectPosition = 130;

	eject_r = false;
	eject_b = false;

	while (true) {
		driverProfileManu();

		delay(5);
	}

}