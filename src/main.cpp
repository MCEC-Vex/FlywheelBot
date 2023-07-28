#include "main.h"
#include "pros/misc.h"
#include "pros/motors.hpp"
#include <string>
#include <vector>
#include <math.h>

#define maxfly 185
#define midfly 165
#define smallfly 140
// testing code upload

// The number of encoder clicks needed to rotate the competition robot by exactly 1 degree.
// Note that this value is surface-dependent.
const double ENCODER_CLICKS_PER_DEGREE_WOOD_TABLE = 750.0 / 90;
const double ENCODER_CLICKS_PER_DEGREE_LIBRARY_CARPET = 885.0 / 90; 


int velocity;
bool rollerUp = false;
bool distancetrue = false;
bool lookFoDisk = false;
int flywheelV = smallfly; // Max speed = 200 Medium = 120 Low = 60
int distance = 700;
int diskCheckVelocity = 20;
int v = 0;
int ymotion;
int xmotion;
int Vision;
int distread;
int intakeV = 200;
int flywheelSpeed;
int reverseintakeV = -200;
int diskCount = 0;
bool auton = true;
int xval;
int TLturn;
int TRturn;
int BLturn;
int BRturn;

pros::ADIDigitalOut piston('A');
// pros::Vision see(8);

pros::Motor topbelt(3, true);	 //
pros::Motor bottombelt(4, true); //
pros::Motor flywheel(2, false);	 // flywheel
pros::Motor flywheelbott(17, true);
pros::Vision vision(16);
pros::Distance dist(7);
pros::Motor roller(1, false);
// Wheel motor ports
pros::Motor topleft(10, false);
pros::Motor topright(13, true);
pros::Motor bottleft(8, true);
pros::Motor bottright(15, false);

pros::Motor_Group left({12, 11});
pros::Motor_Group right({8, 9});
pros::Motor_Group belt({2, 1});

bool intake = false;

void initialize()
{

	pros::lcd::initialize();
	piston.set_value(true); // put piston to down position
}

void Count()
{

	int Disk_Count = 0;

	while (Disk_Count < 3)
	{
		// have the bot move to pick up discs
		// L.move_velocity(-300);
		// R.move_velocity(300);`
		belt.move_velocity(150);
		pros::vision_object_s_t color = vision.get_by_sig(0, 1);
		// vision_object_s_t rtn = vision.get_by_size(0);
		Vision = color.signature;
		pros::lcd::set_text(2, "Count: " + std::to_string(Disk_Count));
		dist.get();
		distread = dist.get();
		pros::delay(1);

		if (distread < 130 && Vision == 1)
		{
			Disk_Count++;
			pros::lcd::set_text(2, "Count: " + std::to_string(Disk_Count));
			pros::delay(2000);
		}
		else
		{

			pros::delay(1);
		}
	}
}

void CalcTurn()
{

	TLturn = (0.0972 * topleft.get_encoder_units()) + 497; // equation for calculating TL turn  old ->(0.103* topleft.get_encoder_units()) + 611
	TRturn = (-0.0779 * topright.get_encoder_units()) - 288;
	BLturn = (0.0924 * bottleft.get_encoder_units()) - 518;
	BRturn = (-0.0797 * bottright.get_encoder_units()) + 305;
}

void test()
{

	while (1)
	{
		pros::delay(1);
	}
}

void firepreloads()
{

	// shoot preloads
	bool auton = true;
	// Get motors ready to shoot

	int count = 0;

	while (auton)
	{

		topbelt.move_velocity(100);
		flywheel.move_velocity(flywheelV);

		velocity = flywheel.get_actual_velocity();
		pros::lcd::set_text(6, "Velocity:" + std::to_string(velocity));
		// pros::lcd::set_text(6,std::to_string(distread));
		pros::delay(1);

		if (velocity > 135 && velocity < 142) // range of speed for shooting , 141
		{

			piston.set_value(false);
			pros::delay(500);
			piston.set_value(true);
			count++;
			pros::delay(1000);
		}

		if (count == 3)
		{
			auton = false;
			break;
		}
	}
}
void waitUntil(pros::Motor motortype, int position)
{ // Delays until it reaches the target position (within a decent margin of eror)

	while (!(motortype.get_position() < (position + 5) && motortype.get_position() > (position - 5)))
	{
		pros::lcd::set_text(1, std::to_string(topleft.get_position()));
		pros::lcd::set_text(2, std::to_string(topright.get_position()));
		pros::lcd::set_text(3, std::to_string(bottleft.get_position()));
		pros::lcd::set_text(4, std::to_string(bottright.get_position()));
		pros::delay(2);
	}

	pros::delay(250); // gives motors a sec to chill
}

void autonMove(int TL, int TR, int BL, int BR, int velo)
{

	topleft.tare_position();
	topright.tare_position();
	bottleft.tare_position();
	bottright.tare_position();

	topleft.move_absolute(TL, velo);
	topright.move_absolute(TR, velo);
	bottleft.move_absolute(BL, velo);
	bottright.move_absolute(BR, velo);
	pros::delay(100);
}

void clear()
{
}

void autonomous()
{
	// clear
	topleft.tare_position();
	topright.tare_position();
	bottleft.tare_position();
	bottright.tare_position();

	autonMove(391, -222, -385, 250, 50);
	waitUntil(topleft, 391);
	// pick ip disk1
	topbelt.move_velocity(100);
	bottombelt.move_velocity(intakeV);
	autonMove(-1129, -860, 1172, 872, 50);
	waitUntil(bottleft, 1172);
	topbelt.move_velocity(0);
	bottombelt.move_velocity(0);

	// turn to roller1

	autonMove(-1068, 946, 1114, -979, 50);
	pros::delay(2500);

	// move to roller1

	autonMove(-1053, -1032, 1066, 1043, 80);
	pros::delay(1500);

	// turn (new Position)

	// roll while moving forward
	roller.move_velocity(100);
	topleft.move_velocity(-50);
	topright.move_velocity(-50);
	bottleft.move_velocity(50);
	bottright.move_velocity(50);
	pros::delay(1000);
	topleft.move_velocity(0);
	topright.move_velocity(0);
	bottleft.move_velocity(0);
	bottright.move_velocity(0);
	roller.move_velocity(0);
	pros::delay(1000);

	// 1776,1688,-1749,-1625

	// clear position
	topleft.tare_position();
	topright.tare_position();
	bottleft.tare_position();
	bottright.tare_position();

	topleft.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
	pros::lcd::set_text(2, std::to_string(topleft.get_position()));
	pros::lcd::set_text(3, std::to_string(topright.get_position()));
	pros::lcd::set_text(4, std::to_string(bottleft.get_position()));
	pros::lcd::set_text(5, std::to_string(bottright.get_position()));

	// 3535 encoder clicks to CENTER
	// Omni wheels are 4 inches in diameter: circumference is 12.566 inches, so 360 encoder clicks = 12.566 inches of movement (theoretically)

	// move straight
	topleft.move_absolute(1776, 100);
	topright.move_absolute(1688, 100);
	bottleft.move_absolute(-1749, 100);
	bottright.move_absolute(-1625, 100);
	pros::delay(2000);

	// clear
	topleft.tare_position();
	topright.tare_position();
	bottleft.tare_position();
	bottright.tare_position();

	// turn
	topleft.move_absolute(1159, 50);
	topright.move_absolute(-560, 50);
	bottleft.move_absolute(-1148, 50);
	bottright.move_absolute(575, 50);
	pros::delay(2000);

	// clear
	topleft.tare_position();
	topright.tare_position();
	bottleft.tare_position();
	bottright.tare_position();

	// move backwards towards roller
	topleft.move_absolute(-1703, 60);
	topright.move_absolute(-1544, 60);
	bottleft.move_absolute(1788, 60);
	bottright.move_absolute(1500, 60);
	pros::delay(2000);

	// roll while moving forward
	roller.move_velocity(100);
	topleft.move_velocity(-50);
	topright.move_velocity(-50);
	bottleft.move_velocity(50);
	bottright.move_velocity(50);
	pros::delay(500);
	topleft.move_velocity(0);
	topright.move_velocity(0);
	bottleft.move_velocity(0);
	bottright.move_velocity(0);
	roller.move_velocity(0);
	pros::delay(500);

	// clear
	topleft.tare_position();
	topright.tare_position();
	bottleft.tare_position();
	bottright.tare_position();

	// move to goal
	topleft.move_absolute(4809, 100);
	topright.move_absolute(4740, 100);
	bottleft.move_absolute(-4810, 100);
	bottright.move_absolute(-4792, 100);
	pros::delay(5000);

	// clear
	topleft.tare_position();
	topright.tare_position();
	bottleft.tare_position();
	bottright.tare_position();

	//-401,-445,411,452
	// move back
	topleft.move_absolute(-401, 100);
	topright.move_absolute(-445, 100);
	bottleft.move_absolute(411, 100);
	bottright.move_absolute(452, 100);
	pros::delay(1000);

	// clear
	topleft.tare_position();
	topright.tare_position();
	bottleft.tare_position();
	bottright.tare_position();

	// turn to shoot*
	topleft.move_absolute(123, 75);
	topright.move_absolute(-33, 75);
	bottleft.move_absolute(-107, 75);
	bottright.move_absolute(95, 75);
	pros::delay(2000);

	//-31,-142,36,168

	// clear
	topleft.tare_position();
	topright.tare_position();
	bottleft.tare_position();
	bottright.tare_position();

	// pros::delay (500);
	// end of robot moving

	firepreloads();

	//-1364,986,1388,-1025

	topbelt.move_velocity(100);
	pros::delay(500);

	// go forward to in
	topleft.move_absolute(-479, 75);
	topright.move_absolute(-699, 75);
	bottleft.move_absolute(500, 75);
	bottright.move_absolute(734, 75);
	pros::delay(1500);

	// clear
	topleft.tare_position();
	topright.tare_position();
	bottleft.tare_position();
	bottright.tare_position();

	// turn
	topleft.move_absolute(1200, 75);
	topright.move_absolute(-900, 75);
	bottleft.move_absolute(-900, 75);
	bottright.move_absolute(1025, 75);
	pros::delay(1000);

	// clear
	topleft.tare_position();
	topright.tare_position();
	bottleft.tare_position();
	bottright.tare_position();

	// straight towards disks
	topleft.move_absolute(-1106, 75);
	topright.move_absolute(-1095, 75);
	bottleft.move_absolute(1168, 75);
	bottright.move_absolute(1152, 75);
	pros::delay(2000);

	// clear
	topleft.tare_position();
	topright.tare_position();
	bottleft.tare_position();
	bottright.tare_position();

	Count();

	// turn
	topleft.move_absolute(-1200, 75);
	topright.move_absolute(900, 75);
	bottleft.move_absolute(900, 75);
	bottright.move_absolute(-1025, 75);
	pros::delay(1000);

	// clear
	topleft.tare_position();
	topright.tare_position();
	bottleft.tare_position();
	bottright.tare_position();

	firepreloads();
}

// Instantaneously turns the robot with the given "speed".
//
// @param turning_power An abstract value between -1.0 and 1.0.
//                      A negative value turns counterclockwise.
//                      A positive value turns clockwise.
//                      A zero value stops the turning.
void turn_chassis(double turning_power) {
	int voltage = turning_power * 127;

	topleft.move(voltage); // Swap negatives if you want the bot to drive in the other direction
	bottleft.move(-voltage);
	bottright.move(voltage);
	topright.move(-voltage);
}

// creating relationship between encoder clicks and robot movement for 90 degrees
void turn_90_degrees(bool turn_clockwise)
{
	
	double target_encoder_clicks = ENCODER_CLICKS_PER_DEGREE_LIBRARY_CARPET * 90.0;
	pros::Motor &drive_motor = topright;

	double starting_encoder_clicks = drive_motor.get_position();

	// If the robot turns clockwise, is the target_motor's encoder count going to increase or decrease?
	// Same question applies if the robot is turning counterclockwise.
	while((turn_clockwise  && (drive_motor.get_position() - starting_encoder_clicks) > -target_encoder_clicks) ||
	      (!turn_clockwise && (drive_motor.get_position() - starting_encoder_clicks) < target_encoder_clicks)) {
		turn_chassis(0.25);
	}
}

void opcontrol()
{
	// DEFINITIONS

	topleft.tare_position();
	topright.tare_position();
	bottleft.tare_position();
	bottright.tare_position();

	roller.tare_position();
	int ymotion;
	int xmotion;

	while (true)
	{

		pros::lcd::set_text(1, std::to_string(topleft.get_position()));
		pros::lcd::set_text(2, std::to_string(topright.get_position()));
		pros::lcd::set_text(3, std::to_string(bottleft.get_position()));
		pros::lcd::set_text(4, std::to_string(bottright.get_position()));

		pros::Controller master(pros::E_CONTROLLER_MASTER);
		// driving control code

		ymotion = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X); // ik this looks wrong, but it works
		xmotion = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);

		int right = -xmotion + ymotion; //-power + turn
		int left = xmotion + ymotion;	// power + turn

		topleft.move(left); // Swap negatives if you want the bot to drive in the other direction
		bottleft.move(-left);
		bottright.move(right);
		topright.move(-right);
		pros::delay(1);

		if (master.get_digital(DIGITAL_R1))
		{ // intake

			topbelt.move_velocity(100);
			bottombelt.move_velocity(intakeV);
			flywheel.get_actual_velocity();
		}
		else
		{
			topbelt.move_velocity(0);
			bottombelt.move_velocity(0);
		}

		if (master.get_digital(DIGITAL_R2))
		{ // runs flywheel while holding R2 down

			flywheel.move_velocity(flywheelV); // Changes flywheel velocity
			flywheelbott.move_velocity(flywheelV);
			pros::lcd::set_text(5, std::to_string(flywheel.get_actual_velocity()));
			pros::lcd::set_text(6, std::to_string(flywheelbott.get_actual_velocity()));
		}
		else
		{
			flywheel.move_velocity(0);
			flywheelbott.move_velocity(0);
		}

		if (master.get_digital(DIGITAL_A))
		{ // piston
			/*while (!(flywheel.get_actual_velocity() == flywheelV )) { //wait until flywheel is at the correct speed
				pros::delay(2);
			}	*/
			piston.set_value(false);
			pros::delay(500);
			piston.set_value(true);
		}

		// Slowdown feature (Cuts robots speed in half while holding down L1 on controller)
		if (master.get_digital(DIGITAL_L1))
		{

			int right = (-xmotion + ymotion) / 3; //-power + turn
			int left = (xmotion + ymotion) / 3;	  // power + turn

			topleft.move(left);
			bottleft.move(left);
			bottright.move(right);
			topright.move(right);

			intakeV = 100;
		}
		else
		{
			intakeV = 200;
		}

		if (master.get_digital(DIGITAL_UP))
		{
			pros::lcd::set_text(6, std::to_string(flywheelV));

			/*	if (flywheelV == 200) {

					flywheelV = 120;
					pros::delay(500);
					flywheelV = 60;
					//pros::delay(500);

				}
		*/

			if (flywheelV == midfly)
			{
				pros::delay(100);
				flywheelV = maxfly;
				master.print(0, 0, "far      ");
			}

			else if (flywheelV == smallfly)
			{
				pros::delay(100);
				flywheelV = midfly;
				master.print(0, 0, "medium");
			}
			pros::delay(100);
		}

		if (master.get_digital(DIGITAL_DOWN))
		{
			pros::lcd::set_text(6, std::to_string(flywheelV));

			if (flywheelV == maxfly)
			{
				pros::delay(100);
				flywheelV = midfly;
				master.print(0, 0, "medium");
			}

			else if (flywheelV == midfly)
			{
				pros::delay(100);
				flywheelV = smallfly;
				master.print(0, 0, "close   ");
			}
		}

		if (master.get_digital(DIGITAL_B))
		{

			roller.move_velocity(200);
		}
		else
		{
			roller.move_velocity(0);
		}

		if (master.get_digital(DIGITAL_LEFT)) // reverse intake
		{
			topbelt.move_velocity(reverseintakeV);
			bottombelt.move_velocity(reverseintakeV);
		}

		if (master.get_digital(DIGITAL_X)) // string shooter
		{
			// This will test rotating the robot 90 degrees instead of shooting string.
			//
			// This call will block until the rotation is done.
			turn_90_degrees(true);
		}
	}
}
