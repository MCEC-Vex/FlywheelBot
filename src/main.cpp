#include "main.h"
#include "pros/misc.h"
#include <string>
//testing code upload

  	pros::ADIDigitalOut piston ('A'); 
    pros::Vision see(8);

    pros::Motor topbelt (2,true); //
    pros::Motor bottombelt(1,true); //
    pros::Motor flywheel (5, true); //flywheel
	pros::Motor flywheeltwo (6, false);

    //Wheel motor ports
    pros::Motor topleft(12, false);
	pros::Motor topright(8, true);	
	pros::Motor bottleft(11, true);
	pros::Motor bottright(9, false);

	bool intake = false;

    int ymotion;
	int xmotion;


void initialize(){

	pros::lcd::initialize();
	pros::lcd::set_text(1, "Mohammed");
	pros::lcd::set_text(2, "Analog Sticks to Drive");
	pros::lcd::set_text(3, "L1 - Slow down on turns");
	pros::lcd::set_text(4, "R1 - Intake Motor");
	pros::lcd::set_text(5, "R2 - Flywheel");
	pros::lcd::set_text(6, "A - Shoot Disc");
	
}





void opcontrol() {
    //DEFINITIONS

    while(true){

 	pros::Controller master(pros::E_CONTROLLER_MASTER);
    //driving control code
    ymotion = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X); //ik this looks wrong, but it works
	xmotion = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);

	int right = -xmotion + ymotion; //-power + turn
	int left = xmotion + ymotion; //power + turn

	topleft.move(left);
	bottleft.move(-left);
	bottright.move(right);
	topright.move(-right);
	pros::delay(1);
	////
	
    if(master.get_digital(DIGITAL_R1)){ //intake 
		/*
		if (intake == false) {
		intake = true;
        topbelt.move_velocity(120);
        bottombelt.move_velocity(120);
		}
		if (intake == true) { //intake
		intake = false;
        topbelt.move_velocity(0);
        bottombelt.move_velocity(0);
		}
		*/
		topbelt.move_velocity(200);
        bottombelt.move_velocity(200);

    } else{
		topbelt.move_velocity(0);
        bottombelt.move_velocity(0);}
    

    if(master.get_digital(DIGITAL_R2)){ //runs flywheel while holding R2 down
        flywheel.move_velocity(999);
		flywheeltwo.move_velocity(999);
    } else {
		flywheel.move_velocity(0);
		flywheeltwo.move_velocity(0);
	}

	if(master.get_digital(DIGITAL_A)) { //piston
		piston.set_value(false);
		pros::delay(200);
		piston.set_value(true);
	}

	// Slowdown feature (Cuts robots speed in half while holding down L1 on controller)
	if(master.get_digital(DIGITAL_L1)) { 

			int right = (-xmotion + ymotion)/3; //-power + turn
			int left = (xmotion + ymotion)/3; //power + turn

			topleft.move(left);
			bottleft.move(left);
			bottright.move(right);
			topright.move(right);

		}

    }
}