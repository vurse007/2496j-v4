#include "pid.hpp"
#include "global.hpp"
#include "util.hpp"
#include "api.h"
#include <cmath>
#include <algorithm>
#include <array>
#include <string_view>
#include <optional>

using namespace glb;

double PID::overallHeading = 0;

PID::PID(double kP, double kI, double kD, double integralThreshold, double maxIntegral, double slew){ //constructor for creating a pid objects with distinct constants and calculations
    this->kP = kP;
    this->kI = kI;
    this->kD = kD;

    this->maxIntegral = maxIntegral;
    this->integralThreshold = integralThreshold;

    this->slew = slew;
}



double PID::calculate(double error, double speed_limit, std::string NAME) {
    // Proportional term
    this->error = error;

    // Integral term (scaled properly for 200 Hz loop)
    if (fabs(this->error) < this->integralThreshold) {
        this->totalError += (this->error + this->prevError)/* / 2.0 * 0.005*/; // 0.005 = 5ms (1/200 Hz)
    }
    this->totalError = std::clamp(this->totalError, -this->maxIntegral, this->maxIntegral);

    // Reset integral when crossing zero
    if ((this->error > 0 && this->prevError < 0) || (this->error < 0 && this->prevError > 0)) {
        this->totalError = 0;
    }

    // Derivative term (scaled for 200 Hz)
    this->derivative = (this->error - this->prevError) /** 200.0*/; // 1 / 0.005 = 200

    // Calculate speed based on context
    if (NAME == "drive") {
        if (fabs(error) < 50) {
            this->speed = (this->kP * 1.8 * this->error) + (this->kI * this->totalError) + (this->kD * 0.8 * this->derivative);
        } else {
            this->speed = (this->kP * this->error) + (this->kI * this->totalError) + (this->kD * this->derivative);
        }
    } else {
        if (fabs(error) < 5) {
            this->speed = (this->kP * 2.5 * this->error) + (this->kI * this->totalError) + (this->kD * this->derivative);
        } else {
            this->speed = (this->kP * this->error) + (this->kI * this->totalError) + (this->kD * this->derivative);
        }
    }

    // Slew rate limit
    double deltaSpeed = this->speed - this->prevSpeed;
    if (deltaSpeed > this->slew) {
        this->speed = this->prevSpeed + this->slew;
    } else if (deltaSpeed < -this->slew) {
        this->speed = this->prevSpeed - this->slew;
    }

    // Ensure speed is within limits
    this->speed = std::clamp(this->speed, -speed_limit, speed_limit);

    // Prepare for next iteration
    this->prevError = this->error;
    this->prevSpeed = this->speed;

    return this->speed;
}


bool PID::settled(double threshold, double time){
    if (fabs(this->error) < threshold){
        count++;
    }

    if (count >= (time/5)){
        count = 0;
        return true;
    } else{
        return false;
    }
}

void PID::update_constants(std::optional<double> kP, std::optional<double> kI, std::optional<double> kD, std::optional<double> integralThreshold, std::optional<double> maxIntegral, std::optional<double> slew){
    if (kP.has_value()) {this->kP = kP.value();}
    if (kI.has_value()) {this->kI = kI.value();}
    if (kD.has_value()) {this->kD = kD.value();}
    if (integralThreshold.has_value()) {this->integralThreshold = integralThreshold.value();}
    if (maxIntegral.has_value()) {this->maxIntegral = maxIntegral.value();}
    if (slew.has_value()) {this->slew = slew.value();}
}

void PID::reset_PID(){
    this->error = 0;
    this->prevError = 0;
    this->totalError = 0;
    this->derivative = 0;
    this->prevSpeed = 0;
    this->speed = 0;
    this->count = 0;
}


//creating blank pid objects to be updated with values later in init function
PID default_drive_pid(0.0, 0.0, 0.0, 0.0, 0.0);
PID default_drive_mogo_pid(0.0, 0.0, 0.0, 0.0, 0.0);
PID default_turn_pid(0.0, 0.0, 0.0, 0.0, 0.0);
PID default_turn_mogo_pid(0.0,0.0,0.0,0.0,0.0);
PID heading_correction_pid(0.0, 0.0, 0.0, 0.0, 0.0);
PID default_arc_pid(0.0, 0.0, 0.0, 0.0, 0.0);
PID default_arc_mogo_pid(0.0, 0.0, 0.0, 0.0, 0.0);

void drive(double target, std::string_view units, std::optional<double> timeout, double chainPos, std::optional<double> speed_limit, bool auto_clamp, PID* pid){

    //based on value passed into the function, convert into motor encoder ticks
    if (units == M_TICKS){
        target = target;
    } else if (units == M_INCHES){
        target = inches_to_chassis_ticks(target);
    }
    else if (units == M_TILES){
        target = inches_to_chassis_ticks(target*24);
    }
    

    //for motion chaining
    bool chain;
    if (chainPos == 0){
        chain = false;
    } else{
        chain = true;
        //target = target + chainPos;
    }

    //for timeouts
    timer driveTimer(-1);
    if (timeout.has_value()){
        driveTimer.set_target(timeout.value());
    }
    else{
        double tpolyTimeoutOutput = driveTimeoutTPOLY.evaluate(target);
        driveTimer.set_target(tpolyTimeoutOutput);
    }

    //for tpoly kd values
    if (pid == &default_drive_pid){ //if default is used, nothing special requested
        double tpolyKDOutput = driveKDTPOLY.evaluate(target);
        pid->update_constants(std::nullopt, std::nullopt, tpolyKDOutput);
    }
    else if (pid == &default_drive_mogo_pid){
        double tpolyKDOutput = driveMogoKDTPOLY.evaluate(target);
        pid->update_constants(std::nullopt, std::nullopt, tpolyKDOutput);
    }



    chassis.tare_position();
    chassis.set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);
    pid->reset_PID();
    heading_correction_pid.reset_PID();

    double initialHeading = imu.get_heading();
    if (initialHeading > 180){
		initialHeading = ((360-initialHeading) * -1);
	}
    //change it to global tracker

    driveTimer.start();

    con.clear();

    while (true){
        //force timeout check
        if (driveTimer.targetReached()){
            break;
        }
        if (auto_clamp==true && autoclamp.get()<60)
        {
            mogo.set_value(true);
        }

        //heading correction
        double position = imu.get_heading();
        //double turnV;
        if (position > 180){ //make only > if not working
            position = ((360-position) * -1);
        }

        if ((initialHeading < 0) && (position > 0)){
            if ((position - initialHeading) >= 180){
                initialHeading = initialHeading + 360;
                position = imu.get_heading();
                //turnV = (initialHeading - position); 
            }
            else {
                //turnV = (abs(position) + abs(initialHeading));
            }
        }
        else if ((initialHeading > 0) && (position < 0)) {
            if ((initialHeading - position) >= 180){
                position = imu.get_heading();
                //turnV = abs(abs(position) - abs(target));
            }
            else {
                //turnV = (abs(position) + target); 
            }
        }
        else {
            //turnV = abs(abs(position) - abs(target));
        }
        double headingError = initialHeading - position;
        double headingCorrection = heading_correction_pid.calculate(headingError);

        //drive calculations
        double currentpos = (FR.get_position() + FL.get_position() + MR.get_position() + ML.get_position() + BR.get_position() + BL.get_position())/6;
        double driveError = target - currentpos;
        double speed = pid->calculate(driveError, speed_limit.value_or(127), "drive");

        //ouput speeds
        lchassis.move(speed + headingCorrection);
        rchassis.move(speed - headingCorrection);

        //debugging with printing error to controller screen
        con.print(0,0, "prt: %lf", driveError);
        
        //settling
        if (pid->settled(5,80)){
            break;
        }

        //chaining movements
        if (chain == true && fabs(driveError) <= chainPos){
            break;
        }

        pros::delay(5);
    }
    chassis.brake();
    
}

void turn(double target, std::optional<double> timeout, double chainPos, std::optional<double> speed_limit, PID* pid){
    //PID::overallHeading = target;
    
    //for motion chaining
    bool chain;
    if (chainPos == 0){
        chain = false;
    } else {
        chain = true;
        target = target + chainPos;
    }

    //for timeouts
    timer turnTimer(-1);
    if (timeout.has_value()){
        turnTimer.set_target(timeout.value());
    }
    else {
        double tpolyTimeoutOutput = turnTimeoutTPOLY.evaluate(target);
        turnTimer.set_target(tpolyTimeoutOutput);
    }

    double position = imu.get_heading();
    double turnV;
    if (position > 180){ //make only > if not working
        position = ((360-position) * -1);
    }

    if ((target < 0) && (position > 0)){
        if ((position - target) >= 180){
            target = target + 360;
            position = imu.get_heading();
            turnV = (target - position); 
        }
        else {
            turnV = (abs(position) + abs(target));
        }
    }
    else if ((target > 0) && (position < 0)) {
        if ((target - position) >= 180){
            position = imu.get_heading();
            turnV = abs(abs(position) - abs(target));
        }
        else {
            turnV = (abs(position) + target); 
        }
    }
    else {
        turnV = abs(abs(position) - abs(target));
    }

    //for tpoly kd values
    double tpolykdoutputdeletelater;
    if (pid == &default_turn_pid){ //if default is used, nothing special requested
        double tpolyKDOutput = turnKDTPOLY.evaluate(turnV);
        tpolykdoutputdeletelater = tpolyKDOutput;
        pid->update_constants(std::nullopt, std::nullopt, tpolyKDOutput);
    }
    else if (pid == &default_turn_mogo_pid){
        double tpolyKDOutput = turnMogoKDTPOLY.evaluate(turnV);
        pid->update_constants(std::nullopt, std::nullopt, tpolyKDOutput);
    }

    chassis.set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);
    pid->reset_PID();

    turnTimer.start();

    while (true){
        //force timeout check
        if (turnTimer.targetReached()){
            break;
        }

        //turn logic
        double position = imu.get_heading();
        double turnV;
        if (position > 180){ //make only > if not working
            position = ((360-position) * -1);
        }

        if ((target < 0) && (position > 0)){
            if ((position - target) >= 180){
                target = target + 360;
                position = imu.get_heading();
                //turnV = (target - position); 
            }
            else {
                //turnV = (abs(position) + abs(target));
            }
        }
        else if ((target > 0) && (position < 0)) {
            if ((target - position) >= 180){
                position = imu.get_heading();
                //turnV = abs(abs(position) - abs(target));
            }
            else {
                //turnV = (abs(position) + target); 
            }
        }
        else {
            //turnV = abs(abs(position) - abs(target));
        }

        double headingError = target - position;
        double speed = pid->calculate(headingError, 127, "turn");

        //ouput speeds
        lchassis.move(speed);
        rchassis.move(-speed);

        //debugging with printing error to controller screen
        con.print(0,0, "error: %lf", headingError);
        
        //settling
        if (pid->settled(2, 80)){
            break;
        }

        //chaining movements
        if (chain == true && fabs(headingError) <= chainPos){
            break;
        }

        pros::delay(5);
    }
    chassis.brake();
}

void arc_right(double target, double radius, std::optional<double> timeout, double chainPos, std::optional<double> speed_limit, PID* pid){
    bool chain;
    double trueTheta;
    if (chainPos == 0){
        chain = false;
    } else{
        chain = true;
        trueTheta = target;
        target = target + chainPos;
    }

    double rightArcLength = (target/360)*2*M_PI*(radius-550);
    double leftArcLength = (target/360)*2*M_PI*(radius+550);

    double speedProp = rightArcLength/leftArcLength; //will be a fraction less than zero bc we are arcing to the right

    // taylor polynomials
    timer arcTimer(-1);
    if (timeout.has_value()){
        arcTimer.set_target(timeout.value());
    }
    else {
        double tPolyTimeoutOutput = driveTimeoutTPOLY.evaluate(leftArcLength);
        arcTimer.set_target(tPolyTimeoutOutput);
    }

    //for tpoly kd values
    if (pid == &default_arc_pid){ //if default is used, nothing special requested
        double tpolyKDOutput = driveKDTPOLY.evaluate(leftArcLength);
        pid->update_constants(std::nullopt, std::nullopt, tpolyKDOutput);
    }
    else if (pid == &default_arc_mogo_pid){
        double tpolyKDOutput = driveMogoKDTPOLY.evaluate(leftArcLength);
        pid->update_constants(std::nullopt, std::nullopt, tpolyKDOutput);
    }


    chassis.tare_position();
    chassis.set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);

    double init_heading = imu.get_heading();

    pid->reset_PID();
    heading_correction_pid.reset_PID();
    
    arcTimer.start();

    con.clear();

    while (true){
        //wrap the init heading to 180,-180
        if (init_heading > 180){
            init_heading-=360;
        }

        double currentRightPos = (FR.get_position() + MR.get_position() + BR.get_position())/3;
        double currentLeftPos = (FL.get_position() + ML.get_position() + BL.get_position())/3;

        double right_error = rightArcLength - currentRightPos;
        double left_error = leftArcLength - currentLeftPos;

        double headingCorrect = (((currentRightPos+currentLeftPos)/2)*360) / (2*M_PI*radius);
        
        double heading = imu.get_heading(); 
		if (heading > 180){
            heading = heading - 360;
        }
        if(((init_heading + headingCorrect) < 0) && (heading > 0)){
            if((heading - (init_heading + headingCorrect)) >= 180){
                init_heading = init_heading + 360;
                heading = imu.get_heading();
            } 
        } else if (((init_heading + headingCorrect)> 0) && (heading < 0)){
            if(((init_heading + headingCorrect) - heading) >= 180){
            heading = imu.get_heading();
            }
        } 

		double headingCorrectionError = init_heading + headingCorrect - heading;

        con.print(0,0, "error: %lf", left_error);

        lchassis.move(pid->calculate(left_error, speed_limit.value_or(127)) + heading_correction_pid.calculate(headingCorrectionError));
        rchassis.move(speedProp*pid->calculate(left_error, speed_limit.value_or(127)) - heading_correction_pid.calculate(headingCorrectionError));

        if (default_arc_pid.settled(20,500)) break;

        if (chain == true && fabs(heading - init_heading) >= trueTheta) break;

        pros::delay(5);
    }
    chassis.brake();
}

void arc_left(double target, double radius, std::optional<double> timeout, double chainPos, std::optional<double> speed_limit, PID* pid){
    bool chain;
    double trueTheta;
    if (chainPos == 0){
        chain = false;
    } else{
        chain = true;
        trueTheta = target;
        target = target + chainPos;
    }

    double rightArcLength = (target/360)*2*M_PI*(radius+550);
    double leftArcLength = (target/360)*2*M_PI*(radius-550);

    double speedProp = leftArcLength/rightArcLength;

    //timeout tpoly
    timer arcTimer(-1);
    if (timeout.has_value()){
        arcTimer.set_target(timeout.value());
    }
    else {
        double tPolyTimeoutOutput = driveTimeoutTPOLY.evaluate(rightArcLength);
        arcTimer.set_target(tPolyTimeoutOutput);
    }

    //for tpoly kd values
    if (pid == &default_arc_pid){ //if default is used, nothing special requested
        double tpolyKDOutput = driveKDTPOLY.evaluate(rightArcLength);
        pid->update_constants(std::nullopt, std::nullopt, tpolyKDOutput);
    }
    else if (pid == &default_arc_mogo_pid){
        double tpolyKDOutput = driveMogoKDTPOLY.evaluate(rightArcLength);
        pid->update_constants(std::nullopt, std::nullopt, tpolyKDOutput);
    }

    chassis.tare_position();
    chassis.set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);

    double init_heading = imu.get_heading();

    pid->reset_PID();
    heading_correction_pid.reset_PID();

    arcTimer.start();

    while (true){
        //wrap the init heading to 180,-180
        if (init_heading > 180){
            init_heading-=360;
        }

        double currentRightPos = (FR.get_position() + MR.get_position() + BR.get_position())/3;
        double currentLeftPos = (FL.get_position() + ML.get_position() + BL.get_position())/3;

        double right_error = rightArcLength - currentRightPos;
        double left_error = leftArcLength - currentLeftPos;

        double headingCorrect = (((currentRightPos+currentLeftPos)/2)*360) / (2*M_PI*radius);

        double heading = imu.get_heading(); 
		if (heading > 180){
            heading = heading - 360;
        }
        if(((init_heading + headingCorrect) < 0) && (heading > 0)){
            if((heading - (init_heading + headingCorrect)) >= 180){
                init_heading = init_heading + 360;
                heading = imu.get_heading();
            } 
        } else if (((init_heading + headingCorrect)> 0) && (heading < 0)){
            if(((init_heading + headingCorrect) - heading) >= 180){
            heading = imu.get_heading();
            }
        } 

		double headingCorrectionError = init_heading + headingCorrect - heading;

        lchassis.move(speedProp*pid->calculate(left_error, speed_limit.value_or(127)) + heading_correction_pid.calculate(headingCorrectionError));
        rchassis.move(pid->calculate(left_error, speed_limit.value_or(127)) - heading_correction_pid.calculate(headingCorrectionError));

        if (default_arc_pid.settled(20,500)) break;

        if (chain == true && fabs(heading - init_heading) >= trueTheta) break;

        pros::delay(5);
    }
    chassis.brake();
}