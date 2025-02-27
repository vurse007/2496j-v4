#include "main.h"
#include "global.hpp"
#include "pid.hpp"
#include "util.hpp"
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
tPoly arcTimeoutTPOLY({0});


//pid util:
double inches_to_chassis_ticks(double inches, double wheel_diameter, double ticks_per_rev){
    return (inches)*(ticks_per_rev/(M_PI*wheel_diameter));
}

//lady brown macro util
bool lbPID = false;
double ladyBrownCorrectPosition = 130.00;
double ladyBrownCurrentPosition;
PID ladyBrownPID(3,0,0,10, 100, 127);

void ladyBrownTask(){
	while(1){
		ladyBrownCurrentPosition = (glb::ldbrotation.get_angle())/100;
		double lberror = (ladyBrownCorrectPosition - ladyBrownCurrentPosition);
		if (lbPID == true){
			glb::ldb.move(ladyBrownPID.calculate(lberror));
		}
        //glb::con.print(0,0, "err: %lf", lberror);
	}
}


//stall protection and color sort util:
