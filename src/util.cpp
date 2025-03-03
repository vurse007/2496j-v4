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
tPoly driveTimeoutTPOLY({0.0});
tPoly turnTimeoutTPOLY({0.0});
tPoly driveKDTPOLY({0.0});
tPoly turnKDTPOLY({0.0});
tPoly driveMogoKDTPOLY({0.0});
tPoly turnMogoKDTPOLY({0.0});


//pid util:
double inches_to_chassis_ticks(double inches, double wheel_diameter, double ticks_per_rev){
    return (inches)*(ticks_per_rev/(M_PI*wheel_diameter));
}

//lady brown macro util
bool lbPID = false;
double ladyBrownCorrectPosition = 130.00;
double ladyBrownCurrentPosition;
PID ladyBrownPID(2.5,0,0,10, 100, 127);

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
int cnt=0;
int rev_cnt=0;
int cur_pos;
int prev_pos;
bool stalled=false;
void stallProtection() {
   
    prev_pos=cur_pos;
   pros::delay(50);
   cur_pos=glb::intake.get_position();
   
   if(abs(cur_pos-prev_pos)<5 && lbPID==false)
   {
        cnt++;
        glb::con.print(0,0, "prt: %lf", cnt);
        
   }
   if(cnt>=5)
   {
        stalled=true;
        cnt=0;
   }
   if(stalled)
   {
        glb::intake.move(-127);
        rev_cnt++;
        if(rev_cnt>=5)
        {
            stalled=false;
            rev_cnt=0;
        }

   }
   else
   {
    glb::intake.move(127);
   }

   
}