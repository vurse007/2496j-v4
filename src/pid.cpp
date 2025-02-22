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

double PID::calculate(double error, double speed_limit){
    //proportional
    this->error = error;

    //integral
    if (fabs(this->error) < this->integralThreshold){
        this->totalError = (((this->error+this->prevError)/2)+this->totalError)/200; //scale with sampling frequency
    }
    this->totalError = std::clamp(this->totalError, -this->maxIntegral, this->maxIntegral);
    if ((this->error > 0 && this->prevError < 0) || (this->error < 0 && this->prevError > 0)){
        this->totalError = 0; //when crossing setpoint, reset integral
    }

    //derivative
    this->derivative = (this->error - this->prevError)*200; //scale with sampling frequency

    //calculate speed
    this->speed = (this->kP*this->error) + (this->kI*this->totalError) + (this->kD*this->derivative);
    double deltaSpeed = this->speed - this->prevSpeed;
    if (deltaSpeed > this->slew){
        this->speed = this->prevSpeed + this->slew;
    } else if (deltaSpeed < -this->slew){
        this->speed = this->prevSpeed - this->slew;
    }
    this->speed = std::clamp(this->speed, -speed_limit, speed_limit);

    //prepare for next iteration
    this->prevError = this->error;
    this->prevSpeed = this->speed;

    return this->speed;
}

bool PID::settled(double threshold, double time){
    if (this->error < threshold){
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

void drive(double target, std::string_view units, std::optional<double> timeout, double chainPos, std::optional<double> speed_limit, PID* pid){

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

    double initialHeading = PID::overallHeading;

    driveTimer.start();

    con.clear();

    while (true){
        //force timeout check
        if (driveTimer.targetReached()){
            break;
        }

        //heading correction
        double currentHeading = imu.get_heading();
        double headingError = fmod(initialHeading - currentHeading + 360, 360);
        if (headingError > 180) {
            headingError -= 360;
        }
        double headingCorrection = heading_correction_pid.calculate(headingError);

        //drive calculations
        double currentpos = (FR.get_position() + FL.get_position() + MR.get_position() + ML.get_position() + BR.get_position() + BL.get_position())/6;
        double driveError = target - currentpos;
        double speed = pid->calculate(driveError, speed_limit.value_or(127));

        //ouput speeds
        lchassis.move(speed + headingCorrection);
        rchassis.move(speed - headingCorrection);

        //debugging with printing error to controller screen
        con.print(0,0, "prt: %lf", driveTimer.getTime());
        
        //settling
        if (pid->settled(10, 500)){
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
    PID::overallHeading = target;
    
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

    //for tpoly kd values
    if (pid == &default_turn_pid){ //if default is used, nothing special requested
        double tpolyKDOutput = turnKDTPOLY.evaluate(target);
        pid->update_constants(std::nullopt, std::nullopt, tpolyKDOutput);
    }
    else if (pid == &default_turn_mogo_pid){
        double tpolyKDOutput = turnMogoKDTPOLY.evaluate(target);
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
        double currentHeading = imu.get_heading();
        double headingError = fmod(target - currentHeading + 360, 360);
        if (headingError > 180) {
            headingError -= 360;
        }
        double speed = pid->calculate(headingError, 127);

        //ouput speeds
        lchassis.move(speed);
        rchassis.move(speed);

        //debugging with printing error to controller screen
        con.print(0,0, "error: %lf", headingError);
        
        //settling
        if (pid->settled(5, 500)){
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

// void arc_right(double target, double radius, std::optional<double> timeout = std::nullopt, double chainPos = 0, std::optional<double> speed_limit = std::nullopt, PID* pid = &default_drive_pid){
//     bool chain;
//     if (chainPos == 0){
//         chain = false;
//     } else{
//         chain = true;
//         double trueTheta = target;
//         target = target + chainPos;
//     }

//     double rightArcLength = (target/360)*2*M_PI*(radius-275);
//     double leftArcLength = (target/360)*2*M_PI*(radius+275);

//     double speedProp = rightArcLength/leftArcLength; //will be a fraction less than zero bc we are arcing to the right

//     chassis.tare_position();
//     chassis.set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);

//     double init_heading = imu.get_heading();

//     while (true){
//         //wrap the init heading to 180,-180
//         if (init_heading > 180){
//             init_heading-=360;
//         }

//         double currentRightPos = (FR.get_position() + MR.get_position() + BR.get_position())/3;
//         double currentLeftPos = (FL.get_position() + ML.get_position() + BL.get_position())/3;

//         double right_error = rightArcLength - currentRightPos;
//         double left_error = leftArcLength - currentLeftPos;
//     }
// }