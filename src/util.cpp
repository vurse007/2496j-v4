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
    if (this->getTime() >= this->targetTime && this->running == true){
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
int lbPID = 0;
double ladyBrownCorrectPosition = 131.00;
double ladyBrownCorrectPosition_4 = 90.00;
double ladyBrownCurrentPosition;
double ladyBrownCorrectPosition_2= 320.00;
double ladyBrownCorrectPosition_3= 260.00;
PID ladyBrownPID(2.5,0,0,10, 100, 127);

void ladyBrownTask(){
	while(1){
		ladyBrownCurrentPosition = (glb::ldbrotation.get_angle())/100;
		
        double lberror = (ladyBrownCorrectPosition - ladyBrownCurrentPosition);
        if (lbPID==1)
        {
            lberror = (ladyBrownCorrectPosition - ladyBrownCurrentPosition);
            glb::ldb.move(0.7*ladyBrownPID.calculate(lberror));
        }
        if (lbPID==2)
        {
            lberror = (ladyBrownCorrectPosition_2 - ladyBrownCurrentPosition);
            glb::ldb.move(2*ladyBrownPID.calculate(lberror));
        }
        if (lbPID==3)
        {
            lberror = (ladyBrownCorrectPosition_3 - ladyBrownCurrentPosition);
            glb::ldb.move(0.5*ladyBrownPID.calculate(lberror));
        }
        if (lbPID==4)
        {
            lberror = (ladyBrownCorrectPosition_4 - ladyBrownCurrentPosition);
            glb::ldb.move(ladyBrownPID.calculate(lberror));
        }
		
        //glb::con.print(0,0, "err: %lf", lberror);
	}
}


//stall protection and color sort util:
int cnt=0;
int rev_cnt=0;
int cur_pos;
int prev_pos=-1;
bool stalled=false;


void stallProtection (){
    cur_pos=glb::intake.get_position();
   if(cur_pos==prev_pos && lbPID==false)
   {
        cnt++;
        glb::con.print(0,0, "prt: %lf", cnt);
        
   }
   else 
   {
    prev_pos=cur_pos;
    cnt=0;
   }
   if(cnt>=5)
   {
        stalled=true;
        cnt=0;
    }
    if(stalled)
    {
        glb::intake.move(-127);
        //return false;
        rev_cnt++;
        if(rev_cnt>=30 )
        {
            stalled=false;
            rev_cnt=0;
            cnt=0;
        }

   }
   else
   {
    glb::intake.move(127);
    //cnt=0;
    //rev_cnt=0;
   }
}
bool sensed=false;
int cnt2=0;
int revcnt=0;
bool eject_b=false;
void eject_blue()
{
    glb::colorsort.set_led_pwm(70);
  while(eject_b)
  {
    
    if(glb::colorsort.get_hue()>200 && glb::colorsort.get_hue()<260 && glb::colorsort.get_proximity()>100)
    {
            //glb::intake.move(-127);
        sensed=true;    
    }

   if (sensed==true)
   { 
    cnt++;
   }
   
   if(cnt>7000 && revcnt<6000)
   {
    glb::intake.move(-127);
    revcnt++;
   }
   else
   {
    glb::intake.move(127);
   }
   if(revcnt==6000)
   {
    sensed=false;
    cnt=0;
    revcnt=0;
   }
   
   
  }
}






bool sense_red=false;
int cnt3=0;
int revcnt2=0;
bool eject_r=false;
void eject_red()
{
    glb::colorsort.set_led_pwm(70);
  while(eject_r)
  {
    
    if((glb::colorsort.get_hue()<40 || glb::colorsort.get_hue()>340) && glb::colorsort.get_hue()<260 && glb::colorsort.get_proximity()>100)
    {
            //glb::intake.move(-127);
        sense_red=true;    
    }

   if (sense_red==true)
   { 
    cnt3++;
   }
   
   if(cnt3>7000 && revcnt2<6000)
   {
    glb::intake.move(-127);
    revcnt2++;
   }
   else
   {
    glb::intake.move(127);
   }
   if(revcnt2==6000)
   {
    sense_red=false;
    cnt3=0;
    revcnt2=0;
   }
   
   
  }
}

   


   //prev_pos=cur_pos;
   

   
