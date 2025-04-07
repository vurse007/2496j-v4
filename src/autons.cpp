#include "autons.hpp"
using namespace glb;
using namespace pros;

void skills(){
    // old skills
	// intake.move(127);
	// delay(600);
	// drive(500, M_TICKS, std::nullopt, 0, 100, true, false);
	// intake.move(0);
	// turn(-90, std::nullopt, 0, std::nullopt);
	// drive(-1000, M_TICKS, std::nullopt, 0, 50, true, false);
	// turn(21, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	// intake.move(127);
	// //doinker.set_value(true);
	
	// drive(3200, M_TICKS, std::nullopt, 0, std::nullopt, true, true, M_DRIVE_MOGO);
	// //doinker.set_value(false);
	// delay(150);
	// lbPID=1;
	// delay(200);
	// drive(500, M_TICKS, std::nullopt, 0, std::nullopt, true, true, M_DRIVE_MOGO);
	// delay(200);
	// drive(-1530, M_TICKS, std::nullopt, 10, 60, true,true, M_DRIVE_MOGO);
	// drive(-300, M_TICKS, std::nullopt, 0, 40);
	
	// turn(90, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	// drive(800, M_TICKS, 1000, 0, 70, true, true, M_DRIVE_MOGO);
	
	// intake.move(0);
	// lbPID=2; 
	// delay(700);
	// intake.move(127);
	// ldb.move(0);
	
	
	
	// //ladybrown shit

	// drive(-550, M_TICKS, 1000, 0, 70, true, true, M_DRIVE_MOGO);
	// lbPID=4;
	// intake.move(-127);
	// turn(179, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	// intake.move(127);
	
	// drive(1200, M_TICKS, std::nullopt, 20, std::nullopt, true, true,  M_DRIVE_MOGO);
	// drive(850, M_TICKS, 800, 0, 90, false, true, M_DRIVE_MOGO);
	// delay(500);
	// turn(50, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	// drive(450, M_TICKS, 600, 0, 127, true,  M_DRIVE_MOGO);
	// turn(-32, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	// mogo.set_value(false);
	// drive(-800, M_TICKS, 700, 0, 70, false, M_DRIVE_MOGO);
	// intake.move(-127);

	// //left off here -first corner reset
	// turn(-32, std::nullopt, 0, std::nullopt);
	// drive(800);
	// turn(-2);
	// intake.move(127);
	// //lbPID=1;
	// drive(2950, M_TICKS, std::nullopt, 5, 127, false);
	// drive(400, M_TICKS, std::nullopt, 0, 50, false);
	// intake.move(0);
	// turn(120);
	
	
	// drive(-1000, M_TICKS,  std::nullopt, 0, 60, true);
	// turn(78, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	// doinker.set_value(true);
	// intake.move(127);
	// drive(1100, M_TICKS, 700, 0, 127, false, M_DRIVE_MOGO);
	// turn(180, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	// doinker.set_value(false);
	// turn(-125, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	// mogo.set_value(false);
	// drive(-370, M_TICKS, 500, 0, 70, false, M_DRIVE_MOGO);
	// turn(-125, std::nullopt, 0, std::nullopt);
	// intake.move(0);

	// drive(850);
	
	// turn(90);
	
	// drive(-1600, M_TICKS, std::nullopt, 0, 70, true);
	// // turn(0, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	// // intake.move(127);
	// // drive(500, M_TICKS, 700, 0, 90, true, M_DRIVE_MOGO);
	
	// // drive(-290, M_TICKS, std::nullopt, 0, 55, true, M_DRIVE_MOGO);
	// // intake.move(0);
	// // lbPID=2;
	// // delay(1000);	
	// // drive(150, M_TICKS, std::nullopt, 2, 50, true, M_DRIVE_MOGO);
	// // lbPID=4;
	// // delay(200);
	// // drive(-150, M_TICKS, std::nullopt, 2, 50, true, M_DRIVE_MOGO);
	// turn(139, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	// intake.move(127);
	// drive(1350, M_TICKS, std::nullopt, 0, std::nullopt, true, M_DRIVE_MOGO);
	// turn(-135, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	// intake.move(0);
	// drive(2300, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);
	// intake.move(127);
	// drive(1500, M_TICKS, std::nullopt, 0, 60, true, M_DRIVE_MOGO);
	// turn(179, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	// drive(300, M_TICKS, 500, 0, 127, true, M_DRIVE_MOGO);
	// turn(-45, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	// drive(700, M_TICKS, 700, 0, 127, true, M_DRIVE_MOGO);
	// turn(24, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	// mogo.set_value(false);
	// intake.move(0);
	// lbPID=1;
	// drive(-700, M_TICKS, 1000, 0, 70, false, M_DRIVE_MOGO);
	// turn(24, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	
	// intake.move(127);
	// drive(1350, M_TICKS, std::nullopt, 0, 127);
	
	
	// turn(-49, std::nullopt, 0, std::nullopt);
	// drive(-1100, M_TICKS, std::nullopt, 0, 70, true);
	// turn(-20, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	// drive(1800, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);
	// turn(-90, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	// drive(620, M_TICKS, 700, 0, 127, true, M_DRIVE_MOGO);
	// intake.move(0);
	// lbPID=2;
	// delay(600);
	// lbPID=4;
	// intake.move(0);
	// drive(-500, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);
	
	// turn(0, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	// intake.move(127);
	// drive(1800, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);
	// turn(-90, std::nullopt, 5, std::nullopt,M_TURN_MOGO);
	// drive(400, M_TICKS, std::nullopt, 20, 127, true, M_DRIVE_MOGO);
	
	// turn(165, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	// intake.move(-127);
	// mogo.set_value(false);
	// delay(200);


	// drive(-500, M_TICKS, std::nullopt, 30, 127, false, M_DRIVE_MOGO);
	// drive(300, M_TICKS, std::nullopt, 30, 127, false, M_DRIVE_MOGO);







	// new skills
	intake.move(127);
	delay(600);
	drive(500, M_TICKS, std::nullopt, 0, 100, true, false);
	intake.move(0);
	turn(-90, std::nullopt, 0, std::nullopt);
	drive(-1000, M_TICKS, std::nullopt, 0, 50, true, false);
	turn(21, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	intake.move(127);
	//doinker.set_value(true);
	
	drive(3200, M_TICKS, std::nullopt, 0, std::nullopt, true, true, M_DRIVE_MOGO);
	//doinker.set_value(false);
	delay(150);
	lbPID=1;
	delay(200);
	drive(500, M_TICKS, std::nullopt, 0, std::nullopt, true, true, M_DRIVE_MOGO);
	delay(200);
	drive(-1530, M_TICKS, std::nullopt, 10, 60, true,true, M_DRIVE_MOGO);
	drive(-300, M_TICKS, std::nullopt, 0, 40);
	
	turn(90, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	drive(800, M_TICKS, 1000, 0, 70, true, true, M_DRIVE_MOGO);
	
	intake.move(0);
	lbPID=2; 
	delay(700);
	intake.move(127);
	ldb.move(0);
	
	
	
	//ladybrown shit

	drive(-550, M_TICKS, 1000, 0, 70, true, true, M_DRIVE_MOGO);
	lbPID=4;
	intake.move(-127);
	turn(179, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	intake.move(127);
	
	drive(1200, M_TICKS, std::nullopt, 20, std::nullopt, true, true,  M_DRIVE_MOGO);
	drive(850, M_TICKS, 800, 0, 90, false, true, M_DRIVE_MOGO);
	delay(500);
	turn(50, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	drive(450, M_TICKS, 600, 0, 127, true,  M_DRIVE_MOGO);
	turn(-32, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	mogo.set_value(false);
	drive(-800, M_TICKS, 700, 0, 70, false, M_DRIVE_MOGO);
	intake.move(-127);

	//first corner reset
	turn(-32, std::nullopt, 0, std::nullopt);
	drive(800);
	turn(-2);
	intake.move(127);
	//lbPID=1;
	drive(2950, M_TICKS, std::nullopt, 20, 127, false);
	drive(415, M_TICKS, std::nullopt, 0, 70, false);
	intake.move(0);
	turn(120);
	
	
	drive(-1000, M_TICKS,  std::nullopt, 0, 60, true);
	turn(78, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	doinker.set_value(true);
	intake.move(127);
	drive(1100, M_TICKS, 700, 0, 127, false, M_DRIVE_MOGO);
	turn(180, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	doinker.set_value(false);
	turn(-125, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	mogo.set_value(false);
	drive(-370, M_TICKS, 500, 0, 70, false, M_DRIVE_MOGO);
	turn(-125, std::nullopt, 0, std::nullopt);
	intake.move(0);

	drive(500, M_TICKS, std::nullopt, 10);
	drive(360, M_TICKS, std::nullopt, 0, 60);
	
	turn(90);
	
	drive(-1000, M_TICKS, std::nullopt, 10, 70, true);
	drive(-610, M_TICKS, std::nullopt, 0, 50, true);
	// turn(0, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	// intake.move(127);
	// drive(500, M_TICKS, 700, 0, 90, true, M_DRIVE_MOGO);
	
	// drive(-290, M_TICKS, std::nullopt, 0, 55, true, M_DRIVE_MOGO);
	// intake.move(0);
	// lbPID=2;
	// delay(1000);	
	// drive(150, M_TICKS, std::nullopt, 2, 50, true, M_DRIVE_MOGO);
	// lbPID=4;
	// delay(200);
	// drive(-150, M_TICKS, std::nullopt, 2, 50, true, M_DRIVE_MOGO);
	turn(139, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	intake.move(127);
	drive(1350, M_TICKS, std::nullopt, 0, std::nullopt, true, M_DRIVE_MOGO);
	turn(-135, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	intake.move(0);
	drive(2300, M_TICKS, std::nullopt, 10, 127, true, M_DRIVE_MOGO);
	intake.move(127);
	drive(1560, M_TICKS, std::nullopt, 0, 60, true, M_DRIVE_MOGO);
	turn(179, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	drive(300, M_TICKS, 500, 0, 127, true, M_DRIVE_MOGO);
	turn(-45, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	drive(700, M_TICKS, 700, 0, 127, true, M_DRIVE_MOGO);
	turn(24, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	mogo.set_value(false);
	intake.move(0);
	lbPID=1;
	drive(-700, M_TICKS, 1000, 0, 70, false, M_DRIVE_MOGO);
	turn(24, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	
	intake.move(127);
	drive(1000, M_TICKS, std::nullopt, 10, 127);
	drive(360, M_TICKS, std::nullopt, 0, 60, false, true);
	
	
	turn(-49, std::nullopt, 0, std::nullopt);
	drive(-700, M_TICKS, std::nullopt, 10, 70, true);
	drive(-400, M_TICKS, std::nullopt, 0, 60, true);
	turn(-20, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	drive(1000, M_TICKS, std::nullopt, 10, 127,true, M_DRIVE_MOGO);
	drive(810, M_TICKS, std::nullopt, 0, 60, true, M_DRIVE_MOGO);
	//ended here
	turn(-90, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	drive(620, M_TICKS, 700, 0, 127, true, M_DRIVE_MOGO);
	intake.move(0);
	lbPID=2;
	delay(600);
	lbPID=0;
	//lbPID=4;
	intake.move(0);
	drive(-550, M_TICKS, std::nullopt, 0, 70, true, M_DRIVE_MOGO);
	
	turn(0, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	intake.move(127);
	drive(1850, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);
	// turn(-90, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	// drive(400, M_TICKS, std::nullopt, 20, 127, true, M_DRIVE_MOGO);
	
	turn(135, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	intake.move(-127);
	mogo.set_value(false);
	delay(200);


	drive(-600, M_TICKS, std::nullopt, 5, 127, false, M_DRIVE_MOGO);
	drive(300, M_TICKS, std::nullopt, 0, 127, false, M_DRIVE_MOGO);

}
void redGoal(){
    //RED goal side
	drive(420, M_TICKS, std::nullopt, 0, 50);
	turn(-63);
	lbPID=2;
	delay(700);
	drive(-1200, M_TICKS, std::nullopt, 0, 70, true, M_DRIVE_MOGO);
	lbPID=4;
	turn(50, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	drive(750, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);
	doinker2.set_value(true);
	delay(100);
	turn(69, std::nullopt, 5, std::nullopt,M_TURN_MOGO);
	drive(75, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);
	doinker.set_value(true);
	delay(100);
	drive(-1300, M_TICKS, std::nullopt, 50, 127, true, M_DRIVE_MOGO);
	drive(-200, M_TICKS, std::nullopt, 0, 30, true, M_DRIVE_MOGO);
	doinker.set_value(false);
	doinker2.set_value(false);
	delay(400);
	turn(50, std::nullopt, 5, std::nullopt,M_TURN_MOGO);
	eject_b=true;
	pros::Task cs_eject_blueTask(eject_blue);
	drive(500, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);

	turn(160, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	drive(1600, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);
	turn(-108, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	drive(1200, M_TICKS, std::nullopt, 50, 127, true, M_DRIVE_MOGO);
	drive(200, M_TICKS, 500, 0, 30, true, M_DRIVE_MOGO);
	delay(500);
	drive(-400, M_TICKS, 500, 0, 127, true, M_DRIVE_MOGO);
	turn(45, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	cs_eject_blueTask.remove();
	mogo.set_value(false);
	lbPID=2;
	drive(2000);

}
void blueGoal(){
    //BLUE goal side
	drive(420, M_TICKS, std::nullopt, 0, 50);
	turn(63);
	lbPID=2;
	delay(700);
	drive(-1250, M_TICKS, std::nullopt, 0, 70, true, M_DRIVE_MOGO);
	lbPID=4;
	turn(-47, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	drive(700, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);
	doinker.set_value(true);
	delay(100);
	turn(-70, std::nullopt, 5, std::nullopt,M_TURN_MOGO);
	drive(125, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);
	doinker2.set_value(true);
	delay(100);
	drive(-1300, M_TICKS, std::nullopt, 50, 127, true, M_DRIVE_MOGO);
	drive(-500, M_TICKS, std::nullopt, 0, 30, true, M_DRIVE_MOGO);
	doinker.set_value(false);
	doinker2.set_value(false);
	delay(400);
	turn(-52, std::nullopt, 5, std::nullopt,M_TURN_MOGO);
	eject_r=true;
	pros::Task cs_eject_redTask(eject_red);
	drive(700, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);

	turn(-165, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	drive(1550, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);
	turn(113, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	drive(1400, M_TICKS, std::nullopt, 50, 127, true, M_DRIVE_MOGO);
	drive(300, M_TICKS, 500, 0, 30, true, M_DRIVE_MOGO);
	delay(500);
	drive(-1000, M_TICKS, 500, 50, 127, true, M_DRIVE_MOGO);
	//turn(-40, std::nullopt, 20, std::nullopt,M_TURN_MOGO);
	cs_eject_redTask.remove();
	//mogo.set_value(false);
	//lbPID=2;
	//drive(1500);
}
void redRing(){
	//RED ring side
	drive(400, M_TICKS, 1000, 0, 70, true, M_DRIVE_MOGO);
	turn(62);
	lbPID=2;
	delay(700);
	drive(-1400, M_TICKS, std::nullopt, 0, 70, true, M_DRIVE_MOGO);
	lbPID=4;
	turn(-105, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	eject_b=true;
	pros::Task cs_eject_blueTask(eject_blue);
	heading_correction_pid.kP = 0;
	heading_correction_pid.kI = 0;
	heading_correction_pid.kD = 0;
	arc_left(64, 700, 1000, 0, 127, M_ARC_MOGO);
	turn(180, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	drive(600);
	
	turn(71, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	drive(600, M_TICKS, std::nullopt, 0, 70, true, M_DRIVE_MOGO);
	turn(110, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	drive(1400, M_TICKS, std::nullopt, 50, 127, true, M_DRIVE_MOGO);
	drive(500, M_TICKS, 800, 0, 60, true, M_DRIVE_MOGO);
	drive(-400, M_TICKS, std::nullopt, 50, 127, true,  M_DRIVE_MOGO);
	turn(0, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	cs_eject_blueTask.remove();
	drive(1700, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);
	drive(700, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);
}
void blueRing(){
    //BLUE ring side
	drive(400, M_TICKS, 1000, 0, 70, true, M_DRIVE_MOGO);
	turn(-62);
	lbPID=2;
	delay(700);
	drive(-1400, M_TICKS, std::nullopt, 0, 70, true, M_DRIVE_MOGO);
	lbPID=4;
	turn(105, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	eject_r=true;
	pros::Task cs_eject_redTask(eject_red);
	heading_correction_pid.kP = 0;
	heading_correction_pid.kI = 0;
	heading_correction_pid.kD = 0;
	arc_right(64, 700, 1000, 0, 127, M_ARC_MOGO);
	turn(180, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	drive(600);
	
	turn(-71, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	drive(600, M_TICKS, std::nullopt, 0, 70, true, M_DRIVE_MOGO);
	turn(-110, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	drive(1400, M_TICKS, std::nullopt, 50, 127, true, M_DRIVE_MOGO);
	drive(500, M_TICKS, 800, 0, 60, true, M_DRIVE_MOGO);
	drive(-400, M_TICKS, std::nullopt, 50, 127, true,  M_DRIVE_MOGO);
	turn(0, std::nullopt, 0, std::nullopt,M_TURN_MOGO);
	cs_eject_redTask.remove();
	drive(1700, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);
	drive(700, M_TICKS, std::nullopt, 0, 127, true, M_DRIVE_MOGO);
}

void iainRed(){
	//iain counter red goal side
	drive(-950, M_TICKS, 1000,0, 70, true);
	delay(200);
	intake.move(127);
	turn(115, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	intake.move(0);
	
	drive(700, M_TICKS, 1000,0, 70, true, M_DRIVE_MOGO);
	doinker2.set_value(true);
	delay(200);
	turn(140, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	drive(100, M_TICKS, 1000,0, 127, true, M_DRIVE_MOGO);
	doinker.set_value(true);
	delay(300);
	turn(115, 500, 0, std::nullopt, M_TURN_MOGO);
	drive(-2200, M_TICKS, 1000,0,127, true, M_DRIVE_MOGO);
	doinker.set_value(false);
	doinker2.set_value(false);
	delay(200);
	turn(90, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	eject_b=true;
	pros::Task cs_eject_blueTask(eject_blue);
	drive(500, M_TICKS, 1000,0, 127, true, M_DRIVE_MOGO);
	turn(-150, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	drive(1200, M_TICKS, 1000,0, 127, true, M_DRIVE_MOGO);
	turn(-50, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	drive(1300, M_TICKS, 1000,0, 127, true, M_DRIVE_MOGO);
	drive(500, M_TICKS, 900,0, 50, true, M_DRIVE_MOGO);
	delay(500);
	turn(-108, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	cs_eject_blueTask.remove();
	mogo.set_value(false);
	
	lbPID=3;
	drive(1500, M_TICKS, 1000,0, 127, true, M_DRIVE_MOGO);
}
void iainBlue(){
	//iain counter blue goal side
	drive(-950, M_TICKS, 1000,0, 70, true);
	delay(200);
	intake.move(127);
	turn(-115, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	intake.move(0);
	
	drive(700, M_TICKS, 1000,0, 70, true, M_DRIVE_MOGO);
	doinker.set_value(true);
	delay(200);
	turn(-140, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	drive(100, M_TICKS, 1000,0, 127, true, M_DRIVE_MOGO);
	doinker2.set_value(true);
	delay(300);
	turn(-115, 500, 0, std::nullopt, M_TURN_MOGO);
	drive(-1800, M_TICKS, 1000,0,127, true, M_DRIVE_MOGO);
	doinker.set_value(false);
	doinker2.set_value(false);
	delay(200);
	turn(-90, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	eject_r=true;
	pros::Task cs_eject_redTask(eject_red);
	drive(500, M_TICKS, 1000,0, 127, true, M_DRIVE_MOGO);
	turn(150, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	drive(1200, M_TICKS, 1000,0, 127, true, M_DRIVE_MOGO);
	turn(50, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	drive(1300, M_TICKS, 1000,0, 127, true, M_DRIVE_MOGO);
	drive(500, M_TICKS, 900,0, 50, true, M_DRIVE_MOGO);
	delay(500);
	turn(-108, std::nullopt, 0, std::nullopt, M_TURN_MOGO);
	mogo.set_value(false);
	cs_eject_redTask.remove();
	lbPID=3;
	drive(1500, M_TICKS, 1000,0, 127, true, M_DRIVE_MOGO);
}