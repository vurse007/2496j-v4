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
tPoly driveTimeoutTPOLY({2000});
tPoly turnTimeoutTPOLY({2000});

tPoly turnMogoKDTPOLY({
    tPoly::scientificNotation(3.58829605, -24),   // m
    tPoly::scientificNotation(-3.71641178, -21),  // l
    tPoly::scientificNotation(1.65001058, -18),   // k
    tPoly::scientificNotation(-4.08877045, -16),  // j
    tPoly::scientificNotation(6.13725372, -14),   // i
    tPoly::scientificNotation(-5.56871764, -12),  // h
    tPoly::scientificNotation(2.64601281, -10),   // g
    tPoly::scientificNotation(-8.39436294, -7),   // f
    tPoly::scientificNotation(0.0000544905519, 0), // d
    tPoly::scientificNotation(-0.001699751186, 0), // c
    tPoly::scientificNotation(0.0259173614, 0),    // b
    tPoly::scientificNotation(0.00000647857494, 0) // a
});

tPoly turnMogoKDTPOLY({
    tPoly::scientificNotation(4.026645186, -23),  
    tPoly::scientificNotation(-3.981714098, -20),  
    tPoly::scientificNotation(1.69477948, -17),  
    tPoly::scientificNotation(-4.039800608, -15),  
    tPoly::scientificNotation(5.841132192, -13),  
    tPoly::scientificNotation(-5.092175166, -11),  
    tPoly::scientificNotation(2.3017719, -9),  
    tPoly::scientificNotation(-0.00006091306382, 0),  
    tPoly::scientificNotation(0.0003346808217, 0),  
    tPoly::scientificNotation(-0.008177025417, 0),  
    tPoly::scientificNotation(0.08665183809, 0),  
    tPoly::scientificNotation(7.865198524, -7)  
});

tPoly driveKDTPOLY({
    tPoly::scientificNotation(-4.78412, -8),  
    0.000135672,  
    0.0890769  
});

tPoly driveMogoKDTPOLY({
    tPoly::scientificNotation(-4.97897, -8),  
    0.000128362,  
    0.0171967  
});



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