#include "util.hpp"
#include "global.hpp"

//timer object functions:
timer::timer(double target){
    this->targetTime = target;
}

void timer::set_target(double target){
    this->targetTime = target;
}

void timer::start(){
    this->startTime = pros::millis();
    this->running = true;
}

double timer::getTime(){
    return (pros::millis() - this->startTime);
}

bool timer::targetReached(){
    if (this->getTime() >= this->targetTime){
        this->running = false;
        return true;
    } else{
        return false;
    }
}

void timer::reset(double target=-1){
    this->startTime = pros::millis();
    this->running = true;
    if (target != -1){
        this->targetTime = target;
    }
}



//tpoly object functions:
tPoly::tPoly(const std::vector<long double>& coeffs) {
    this->coefficients = coeffs; //copy inputted vector into the class's coefficients vector
}

void tPoly::update_coefficients(const std::vector<long double>& coeffs){
    this->coefficients = coeffs;
}

double tPoly::evaluate(double x) const{
    double y = 0.00;
    for (const double& coeff : this->coefficients){
        y = y*x + coeff; //using const & to prevent changing of coefficients and to prevent copying the vector every time which can be slow
    }
    return y;
}

long double tPoly::scientificNotation(double number, double exponent){
    return number * pow(10, exponent);
}

//creating blank taylor polynomials to update in init function
tPoly driveTimeoutTPOLY({0});
tPoly turnTimeoutTPOLY({0});
tPoly turnKDTPOLY({0});
tPoly turnMogoKDTPOLY({0});
tPoly driveKDTPOLY({0});
tPoly driveMogoKDTPOLY({0});


//pid util:
double inches_to_chassis_ticks(double inches, double wheel_diameter, double ticks_per_rev){
    return (inches)*(ticks_per_rev/(M_PI*wheel_diameter));
}

//lady brown macro util
bool lbPID = false;
double ladyBrownTarget = 329.00;
double ladyBrownPos;
PID ladyBrownPID(-3, -0, -0, 1000, 1000, 127);
void ladyBrownTask(){
    while (1){
        ladyBrownPos = (glb::ldbrotation.get_angle())/100;
        double ladyBrownError = (ladyBrownTarget - ladyBrownPos);
		if (lbPID == true){
			glb::ldb.move(ladyBrownPID.calculate(ladyBrownError));
		}
        pros::delay(5);
    }
}


//stall protection and color sort util:
bool intakeInterrupt = false;
void stallProtection() {
    timer stallTimer(1000);
    timer reverseTimer(1000);
    int returnVelocity = 0;
    glb::con.clear();

    while (1) {
        
        if (abs(glb::intake.get_actual_velocity()) <= 127 && glb::intake.get_target_velocity() != 0 && stallTimer.running == false) {
            returnVelocity = glb::intake.get_target_velocity();
            stallTimer.start(); //if stalled start the stall timer
        }
        if (stallTimer.targetReached()){
            if (abs(glb::intake.get_actual_velocity()) <= 1 && glb::intake.get_target_velocity() != 0){
                glb::intake.move(-127); //if the intake is still stalling after timer expires
                reverseTimer.start();
                while (reverseTimer.targetReached() == false){
                    pros::delay(5); //lock out while reverser timer is counting
                }
                glb::intake.move(returnVelocity);
            }
        }
        
        pros::delay(5);
        glb::con.print(0,0,"time: %d", stallTimer.getTime());
    }
}