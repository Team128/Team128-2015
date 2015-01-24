#include "WPILib.h"

class Robot: public IterativeRobot
{
private:
	//initialize variables
		//driver station
	LiveWindow *lw;
	Joystick *m_leftStick;
	Joystick *m_rightStick;
	Joystick *m_gamepad;

	// motors
		//drivetrain
	CANTalon *m_leftFront;
	CANTalon *m_rightFront;
	CANTalon *m_leftBack;
	CANTalon *m_rightBack;
	RobotDrive *m_robotDrive;
	CANTalon *m_screw;

	//pneumatics
	//shifter
	Solenoid *m_shifterUp;
	Solenoid *m_shifterDown;

	//teleop variables
	Timer *lastShift;

	//ints and floats
	int shiftValue;
	float leftStickValue;
	float rightStickValue;


	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
		m_leftStick = new Joystick(0);
		m_rightStick = new Joystick(0);
		m_gamepad = new Joystick(0);
		m_leftFront = new CANTalon(0);
		m_rightFront = new CANTalon(0);
		m_leftBack = new CANTalon(0);
		m_rightBack = new CANTalon(0);
		m_robotDrive = new RobotDrive(m_leftFront, m_leftBack, m_rightFront, m_rightBack);
		m_screw = new CANTalon(0);
		m_shifterUp = new Solenoid(0);
		m_shifterDown = new Solenoid(0);
		lastShift = new Timer();

	}

	void AutonomousInit()
	{

	}

	void AutonomousPeriodic()
	{

	}

	void TeleopInit()
	{
		shiftValue = 0;
	}
	void TeleopPeriodic()
	{

	}

	void TestPeriodic()
	{
		lw->Run();
	}
	void TeleopDrive()
	{
		leftStickValue = m_leftStick->GetY();
		rightStickValue = m_rightStick->GetY();
		m_robotDrive->TankDrive(leftStickValue, rightStickValue);
	}

	void Shift()
	{
		if(m_leftStick->GetTrigger() == 1 && m_rightStick->GetTrigger() == 1 && shiftValue == 0 && lastShift->Get() > 0.3 )
		{
			m_shifterUp->Set(true);
			m_shifterDown->Set(false);
			shiftValue = 1;
			lastShift->Reset();
		}
		else if(m_leftStick->GetTrigger() == 1 && m_rightStick->GetTrigger() == 1 && shiftValue == 1 && lastShift->Get() > 0.3 )
		{
			m_shifterUp->Set(false);
			m_shifterDown->Set(true);
			shiftValue = 0;
			lastShift->Reset();
		}
	}
};

START_ROBOT_CLASS(Robot);
