#include "main.h"
#include "pros/misc.h"
#include <string>

void opcontrol() {
    //DEFINITIONS
	  // configure digital port 1 as an output
  	 pros::ADIDigitalOut piston ('A');
    pros::Vision see(8);
    pros::Motor topmotor (3,true);
    pros::Motor Amotor(4,true);
    pros::Motor bottommotor (2, true);
    pros::Controller master(pros::E_CONTROLLER_MASTER);
    //Motor ports (BIG BOT)
    pros::Motor topleft(3, false);
	pros::Motor topright(4, false);	
	pros::Motor bottleft(2, true);
	pros::Motor bottright(1, true);
	bool intake = false;

    int ymotion;
	int xmotion;

    while(1){
    
    ymotion = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
	xmotion = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
	int right = -xmotion + ymotion; //-power + turn
	int left = xmotion + ymotion; //power + turn
	topleft.move(left);
	bottleft.move(-left);
	bottright.move(right);
	topright.move(-right);
	pros::delay(10);

    if(master.get_digital(DIGITAL_R1)){ //intake
		if (intake == false) {
		intake = true;
        topmotor.move_velocity(120);
        Amotor.move_velocity(120);
        pros::delay(1000);
		}
		if (intake == true) { //intake
		intake = false;
        topmotor.move_velocity(0);
        Amotor.move_velocity(0);
        pros::delay(1000);
		}
    }
    }
    if(master.get_digital(DIGITAL_R2)){ //flywheel
        bottommotor.move_velocity(1111111120);
   	 	pros::delay(500);
    }
	if(master.get_digital(DIGITAL_A)) { //piston
		piston.set_value(false);
		pros::delay(1000);
		piston.set_value(true);
	}


    }
 