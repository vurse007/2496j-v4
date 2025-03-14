#include "global.hpp"

namespace glb{
    // motor inits
    pros::Motor FR(P_FR, pros::E_MOTOR_GEAR_600, false);
    pros::Motor BR(P_BR, pros::E_MOTOR_GEAR_600, false);
    pros::Motor MR(P_MR, pros::E_MOTOR_GEAR_600, true);
    pros::Motor FL(P_FL, pros::E_MOTOR_GEAR_600, true);
    pros::Motor BL(P_BL, pros::E_MOTOR_GEAR_600, true);
    pros::Motor ML(P_ML, pros::E_MOTOR_GEAR_600, false);
    pros::MotorGroup rchassis({P_FR, P_BR, P_MR});
    pros::MotorGroup lchassis({P_FL, P_BL, P_ML});
    pros::MotorGroup chassis({P_FR, P_BR, P_MR, P_FL, P_BL, P_ML});

    pros::Motor intake(P_INTAKE, pros::E_MOTOR_GEAR_600, true);
    pros::Motor ldb(P_LDB, pros::E_MOTOR_GEAR_200, true);

    pros::ADIDigitalOut mogo(P_MOGO, false);
    pros::ADIDigitalOut intake_lift(P_INTAKE_LIFT, false);
    pros::ADIDigitalOut doinker(P_DOINKER, false);
    pros::ADIDigitalOut doinker2(P_DOINKER_2, false);
    pros::Imu imu(P_IMU);
    pros::Rotation ldbrotation(P_LDB_ROTATION);
    pros::Optical colorsort(P_COLORSORT);
    pros::Distance autoclamp(P_AUTOCLAMP);

    // pros::Rotation x_pod(P_X_POD);
    // pros::Rotation y_pod(P_Y_POD);

    pros::Controller con(pros::E_CONTROLLER_MASTER);

    pros::ADIDigitalIn autoselect(P_AUTOSELECT);

}