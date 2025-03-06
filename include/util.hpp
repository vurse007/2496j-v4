#pragma once
#include "api.h"
#include "pid.hpp"
#include "global.hpp"

class timer {
    private:
        double startTime;
        double targetTime;

    public:
        bool running = false;
        timer(double target);
        void set_target(double target);
        void start();
        double getTime();
        bool targetReached();
        void reset(double target);
};

class tPoly {
    private:
        std::vector<long double> coefficients;

    public:
        tPoly(const std::vector<long double>& coeffs);

        void update_coefficients(const std::vector<long double>& coeffs);

        double evaluate(double x) const;

        static long double scientificNotation(double number, double exponent);
};

extern tPoly driveTimeoutTPOLY;
extern tPoly turnTimeoutTPOLY;
extern tPoly turnKDTPOLY;
extern tPoly turnMogoKDTPOLY;
extern tPoly driveKDTPOLY;
extern tPoly driveMogoKDTPOLY;


//pid util
double inches_to_chassis_ticks(double inches, double wheel_diameter = 3.25, double ticks_per_rev = 300);

//lady brown macro util
extern int lbPID;
extern double ladyBrownCorrectPosition;
extern double ladyBrownCorrectPosition_2;
extern double ladyBrownCorrectPosition_3;
extern double ladyBrownCorrectPosition_4;
extern PID ladyBrownPID;
void ladyBrownTask();

//stall protection and color sort util
extern int cnt;
extern int rev_cnt;
extern int cur_pos;
extern int prev_pos;
extern bool stalled;
void stallProtection();