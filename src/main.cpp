#include "main.h"
#include "pros/misc.h"
#include "pros/motors.hpp"
#include <string>
#include <vector>
#include <math.h>

#define maxfly 198
//testing code upload


bool rollerUp= false;
bool distancetrue = false; 
bool lookFoDisk = false;
int flywheelV = maxfly; //Max speed = 200 Medium = 120 Low = 60
int distance = 700;
int diskCheckVelocity = 20;
int v = 0;
int ymotion;
int xmotion;
int Vision;
int distread;
int intakeV = 200;

void initialize(){

	pros::lcd::initialize();
	//pros::lcd::set_text(1, "Mohammed2");
	
	//pros::lcd::set_text(2, "Analog Sticks to Drive");
	//pros::lcd::set_text(3, "L1 - Slow down on turns");
	//pros::lcd::set_text(4, "R1 - Intake Motor");
	//pros::lcd::set_text(5, "R2 - Flywheel");
	//pros::lcd::set_text(6, "A - Shoot Disc");


}

pros::ADIDigitalOut piston ('A'); 
   // pros::Vision see(8);

    pros::Motor topbelt (2,true); //
    pros::Motor bottombelt(1,true); //
    pros::Motor flywheel (5, false); //flywheel
	pros::Motor flywheelbott (17, true);
	pros::Vision vision(16);
	pros::Distance dist(13);
	pros::Motor roller(15, false);
    //Wheel motor ports
    pros::Motor topleft(9, false);
	pros::Motor topright(11, true);	
	pros::Motor bottleft(8, true);
	pros::Motor bottright(12, false);

	    pros::Motor_Group left({12,11});
    pros::Motor_Group right({8,9});
	pros::Motor_Group belt({2,1});

	bool intake = false;


void Count() {

    int Disk_Count= 0; 

 
    while( Disk_Count < 3){
		//have the bot move to pick up discs
	//L.move_velocity(-300);
    //R.move_velocity(300);`
	belt.move_velocity(150);
    pros::vision_object_s_t color = vision.get_by_sig(0, 1);
	//vision_object_s_t rtn = vision.get_by_size(0);
    Vision = color.signature;
    pros::lcd::set_text(2, "Count: " + std::to_string(Disk_Count));
	dist.get();
	distread = dist.get();
  	pros::delay(1);
	

    if (distread < 130 && Vision == 1)
    {
     	Disk_Count ++;
        pros::lcd::set_text(2, "Count: " + std::to_string(Disk_Count));
        pros::delay(2000);


    }
    else
    {
	   
       pros::delay(1);

    }
    }

}



void autonomous()
{
topleft.tare_position();
topright.tare_position();
bottleft.tare_position();
bottright.tare_position();
topleft.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);

//3535 encoder clicks to CENTER
//Omni wheels are 4 inches in diameter: circumference is 12.566 inches, so 360 encoder clicks = 12.566 inches of movement (theoretically)
topleft.move_absolute(-3535, 100); 
topright.move_absolute(-3535, 100);
bottleft.move_absolute(3535, 100);
bottright.move_absolute(3535, 100);
pros::lcd::set_text(2, std::to_string(topleft.get_position()));
//R.move_absolute(600, 100);// Moves 100 units forward
 /* while (!((topleft.get_position() < 605) && (topleft.get_position() > 595))) {
    // Continue running this loop as long as the motor is not within +-5 units of its goal
    pros::delay(2);
  }*/
//pros::delay(2000);

//Count();




}




void opcontrol() {
    //DEFINITIONS
  	

   
	roller.tare_position();
    int ymotion;
	int xmotion;

    while(true){
		pros::lcd::set_text(2, std::to_string(topleft.get_position()));

 	pros::Controller master(pros::E_CONTROLLER_MASTER);
    //driving control code
    ymotion = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X); //ik this looks wrong, but it works
	xmotion = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);

	int right = -xmotion + ymotion; //-power + turn
	int left = xmotion + ymotion; //power + turn

	topleft.move(left); //Swap negatives if you want the bot to drive in the other direction
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
		topbelt.move_velocity(intakeV);
        bottombelt.move_velocity(intakeV);

    } else{
		topbelt.move_velocity(0);
        bottombelt.move_velocity(0);}
    

    if(master.get_digital(DIGITAL_R2)){ //runs flywheel while holding R2 down

        flywheel.move_velocity(flywheelV); //Changes flywheel velocity
        flywheelbott.move_velocity(flywheelV);
		pros::lcd::set_text(5, std::to_string(flywheel.get_actual_velocity()));
		pros::lcd::set_text(6, std::to_string(flywheelbott.get_actual_velocity()));


		} else {
			flywheel.move_velocity(0);
			flywheelbott.move_velocity(0);
	}

	if(master.get_digital(DIGITAL_A)) { //piston
		piston.set_value(false);
		pros::delay(500);
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

			intakeV = 100;
			

		} else { intakeV = 200;}

	
    if(master.get_digital(DIGITAL_UP)) {
      pros::lcd::set_text(6, std::to_string(flywheelV));
		
	/*	if (flywheelV == 200) {

			flywheelV = 120;	
			pros::delay(500);
	  		flywheelV = 60;
			//pros::delay(500);

		} 
*/
		if (flywheelV == 150) {
      pros::delay(500);
			flywheelV = maxfly;
      
  
		}

		else if (flywheelV == 90) {
       pros::delay(500);
			flywheelV = 150;
     
      
		}
     pros::delay(10);

		}

	if(master.get_digital(DIGITAL_DOWN)) {
    pros::lcd::set_text(6, std::to_string(flywheelV));
		
		if (flywheelV == maxfly) {
			      flywheelV = 150;
      		pros::delay(1000);
			

		} 

		else if (flywheelV == 150) {
			
			pros::delay(500);
			flywheelV = 90;
   
		}
/*
		if (flywheelV == 60) {
			flywheelV = 200;
      pros::delay(100);
		} */
	}

	if(master.get_digital(DIGITAL_B)) {
		
		roller.move_velocity(400);

		} else {
			roller.move_velocity(0);
		}

	

}

}
