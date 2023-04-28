#pragma config(Sensor, in1,    armEncoder,     sensorPotentiometer)
#pragma config(Sensor, dgtl1,  leftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  rightEncoder,   sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  sonar,          sensorSONAR_inch)
#pragma config(Sensor, dgtl12, jumper,         sensorDigitalIn)
#pragma config(Motor,  port1,           leftMotor,     tmotorVex393_HBridge, openLoop, reversed, driveLeft, encoderPort, dgtl1)
#pragma config(Motor,  port2,           armMotor,      tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           clawMotor,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          rightMotor,    tmotorVex393_HBridge, openLoop, driveRight, encoderPort, dgtl3)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*----------------------------------------------------------------------------------------------------*\
|*                                - Clawbot Single Joystick Control -                                 *|
|*                                      ROBOTC on VEX 2.0 Cortex                                      *|
|*    MOTORS & SENSORS:                                                                               *|
|*    [I/O Port]        [Name]          [Type]                               [Description]            *|
|*    Motor - Port 2   rightMotor    VEX 393 Motor                         Right drive motor          *|
|*    Motor - Port 6   clawMotor     VEX 393 Motor w/ Motor Controler 29   Claw motor                 *|
|*    Motor - Port 7   armMotor      VEX 393 Motor w/ Motor Controler 29   Arm motor                  *|
|*    Motor - Port 10  leftMotor     VEX 393 Motor                         Left drive motor           *|
\*----------------------------------------------------------------------------------------------------*/

// Global Variables

float armTargetPosition = -1;

// Helper Methods
short max(short a, short b) {
	if (a > b) {
		return a;
	}
	return b;
}

// Arcade Drive Method: takes in a speed and rotation
void arcadeDrive(short drive, short rotate) {
	short maximum = max(abs(drive), abs(rotate));
	short total = drive + rotate;
	short difference = drive - rotate;

	if (drive >= 0) {
		if (rotate >= 0) {
			motor[leftMotor] = maximum;
			motor[rightMotor] = difference;
		} else {
			motor[leftMotor] = total;
			motor[rightMotor] = maximum;
		}
	}
	else {
		if (rotate >= 0) {
			motor[leftMotor] = total;
			motor[rightMotor] = -maximum;
		} else {
			motor[leftMotor] = -maximum;
			motor[rightMotor] = difference;
		}
	}
}

void toPosition(float position) {
	armTargetPosition = position;
}

// PID + Feedforward Values

float armP = 0.0;
float armI = 0.0;
float armD = 0.0;
float armS = 0.0;
float armG = 0.0;

float dt = 0.05;

float p = 0;
float i = 0;

float lastError = 0;

float armCalculate(float setpoint, float measurement) {
	float error = measurement - setpoint;
	
	float ff = armS + cos(measurement) * armG;
	
	p = armP * error;
	i += armI * error * dt;
	float derivative = (error - lastError) / dt;
	d = armD * derivative;
	
	lastError = error;
	
	return output;
}

void runArm() {
	if (armTargetPosition != -1) {

	} else { // arm manual control
		if(vexRT[Btn5U] == 1)       	//If button 5U is pressed...
		{
			motor[armMotor] = 127;    	//...raise the arm.
		}
		else if(vexRT[Btn5D] == 1)  	//Else, if button 5D is pressed...
		{
			motor[armMotor] = -127;   	//...lower the arm.
		}
		else                      		//Else (neither button is pressed)...
		{
			motor[armMotor] = 0;      	//...stop the arm.
		}
	}
}

void runClaw() {
	if(vexRT[Btn6U] == 1)       	//If Button 6U is pressed...
		{
			motor[clawMotor] = 127;  		//...close the gripper.
		}
		else if(vexRT[Btn6D] == 1)  	//Else, if button 6D is pressed...
		{
			motor[clawMotor] = -127; 		//...open the gripper.
		}
		else                      		//Else (neither button is pressed)...
		{
			motor[clawMotor] = 0;    		//...stop the gripper.
		}
}

task main () {

	while(true) {

		// motor[leftMotor]  = (vexRT[Ch2] + vexRT[Ch1]);  // (y + x)/2
		// motor[rightMotor] = (vexRT[Ch2] - vexRT[Ch1]);  // (y - x)/2
	
		arcadeDrive(vexRT[Ch3], vexRT[Ch1]);
		runArm();
		runClaw();
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
