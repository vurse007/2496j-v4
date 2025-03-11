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

	//intake below
	if (con.get_digital(E_CONTROLLER_DIGITAL_R1)){
		//intake.move(127);
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

	rchassis.move(power - turn);
	lchassis.move(power + turn);
	

	//intake below
	if (con.get_digital(E_CONTROLLER_DIGITAL_R1)){
		//intake.move(127);
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
		lbPID = 1;
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

	//starting tasks
	pros::Task stallProtector(stallProtectionTask);
	pros::Task ladyBrownMacroTask(ladyBrownTask);
	//pros::Task cs_eject_redTask(cs_eject_red);
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
	driveTimeoutTPOLY.update_coefficients({2000});
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
	//skills
	//delay(1500);
	
	drive(500, M_TICKS, std::nullopt, 0, 100, true);
	turn(-90, std::nullopt, 0, std::nullopt);
	drive(-900, M_TICKS, std::nullopt, 0, 50, true);
	turn(21, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	intake.move(127);
	drive(3200, M_TICKS, std::nullopt, 0, std::nullopt, true, M_DRIVE_MOGO);
	delay(150);
	lbPID=1;
	delay(200);
	drive(500, M_TICKS, std::nullopt, 0, std::nullopt, true, M_DRIVE_MOGO);
	delay(200);
	drive(-1890, M_TICKS, std::nullopt, 0, 60, true, M_DRIVE_MOGO);
	turn(90, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	drive(750, M_TICKS, 700, 0, 90, true, M_DRIVE_MOGO);
	lbPID=0;
	intake.move(0);
	ldb.move(127);
	delay(700);
	intake.move(127);
	ldb.move(0);
	lbPID=4;
	
	
	//ladybrown shit
	drive(-600, M_TICKS, 1000, 0, 70, true, M_DRIVE_MOGO);
	turn(179, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	drive(1200, M_TICKS, std::nullopt, 20, std::nullopt, true, M_DRIVE_MOGO);
	drive(1000, M_TICKS, std::nullopt, 0, 90, false, M_DRIVE_MOGO);
	turn(50, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	drive(450, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);
	turn(-32, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	mogo.set_value(false);
	drive(-800, M_TICKS, 1000, 0, 70, false, M_DRIVE_MOGO);
	turn(-32, std::nullopt, 0, std::nullopt);
	drive(800);
	turn(-2);
	lbPID=1;
	drive(2950, M_TICKS, std::nullopt, 5, 127, false);
	drive(400, M_TICKS, std::nullopt, 0, 70, false);
	turn(115);
	intake.move(0);
	drive(-1000, M_TICKS,  std::nullopt, 0, 70, true);
	turn(78, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	doinker.set_value(true);
	drive(1200, M_TICKS, 700, 0, 127, false, M_DRIVE_MOGO);
	turn(180, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	doinker.set_value(false);
	turn(-125, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	mogo.set_value(false);
	drive(-370, M_TICKS, 300, 0, 70, false, M_DRIVE_MOGO);
	turn(-125, std::nullopt, 0, std::nullopt);
	drive(850);
	turn(90);
	drive(-1650, M_TICKS, std::nullopt, 0, 70, true);
	turn(0, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	drive(500, M_TICKS, 700, 0, 90, true, M_DRIVE_MOGO);
	intake.move(127);
	drive(-290, M_TICKS, std::nullopt, 0, 55, true, M_DRIVE_MOGO);
	lbPID=2;
	delay(1000);	
	drive(150, M_TICKS, std::nullopt, 2, 50, true, M_DRIVE_MOGO);
	lbPID=4;
	//delay(200);
	drive(-150, M_TICKS, std::nullopt, 2, 50, true, M_DRIVE_MOGO);
	turn(145, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	drive(1500, M_TICKS, std::nullopt, 0, std::nullopt, true, M_DRIVE_MOGO);
	turn(-135, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	intake.move(0);
	drive(2500, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);
	intake.move(127);
	drive(1490, M_TICKS, std::nullopt, 0, 70, true, M_DRIVE_MOGO);
	turn(179, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	drive(300, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);
	turn(-45, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	drive(500, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);
	turn(22, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	mogo.set_value(false);
	drive(-600, M_TICKS, std::nullopt, 0, 70, false, M_DRIVE_MOGO);
	turn(30, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	drive(1200, M_TICKS, std::nullopt, 0, 127);
	lbPID=1;
	turn(-50, std::nullopt, 0, std::nullopt);
	drive(-1000, M_TICKS, std::nullopt, 0, 70, true);
	turn(-25, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	drive(1900, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);
	turn(-90, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	drive(400, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);
	lbPID=0;
	ldb.move(127);
	delay(600);
	drive(-500, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);
	turn(0, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	drive(2000, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);
	turn(135, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	mogo.set_value(false);
	drive(-500, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);


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

	while (true) {
		driverProfileManu();

		delay(5);
	}

}