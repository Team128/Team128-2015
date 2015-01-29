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
	SmartDashboard *smart;

	// motors
		//drivetrain
	CANTalon *m_leftFront;
	CANTalon *m_rightFront;
	CANTalon *m_leftBack;
	CANTalon *m_rightBack;
	RobotDrive *m_robotDrive;
		//lift
	CANTalon *m_screw;

	//pneumatics
		//shifter
	Solenoid *m_shifterUp;
	Solenoid *m_shifterDown;

	//timers
	Timer *lastShift;
	Timer *liftToBoxHeight;
	Timer *autonLiftTimer;
	Timer *autonTurnTimer;
	Timer *autonDriveTimer;

	//sensors
	DigitalInput *liftLimitSwitch;

	//ints, floats, and bools
	int shiftValue;
	int autonSwitch;
	float leftStickValue;
	float rightStickValue;
	bool bikiniBottom;									//Lift is at the bottom


	void RobotInit()
	{
		//driverstation
		lw = LiveWindow::GetInstance();
		m_leftStick = new Joystick(0);
		m_rightStick = new Joystick(0);
		m_gamepad = new Joystick(0);
		smart = new SmartDashboard();

		//drivetrain
		m_leftFront = new CANTalon(0);
		m_rightFront = new CANTalon(0);
		m_leftBack = new CANTalon(0);
		m_rightBack = new CANTalon(0);
		m_shifterUp = new Solenoid(0);
		m_shifterDown = new Solenoid(0);
		m_robotDrive = new RobotDrive(m_leftFront,m_rightFront,m_leftBack,m_rightBack);

		//lift
		m_screw = new CANTalon(0);

		//timers
		lastShift = new Timer();
		liftToBoxHeight = new Timer();
		autonLiftTimer = new Timer();
		autonTurnTimer = new Timer();
		autonDriveTimer = new Timer();

		//sensors
		liftLimitSwitch = new DigitalInput(0);
		SmartDashboard::init();
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
		TeleopDrive();
		Shift();
		Lift();
	}

	void TestPeriodic()
	{
		lw->Run();
		TeleopDrive();
		Shift();
		Lift();
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

	void Lift()
	{
		m_screw->Set(m_gamepad->GetZ());		//"Set" may be changed for CAN

		if(m_gamepad->GetRawButton(12))
		{
			while(liftLimitSwitch->Get())
			{
				m_screw->Set(-0.7);
			}
			bikiniBottom = true;
			m_screw->Set(0.0);
			smart->PutString("Lift status", "At bottom");
		}
		if(m_gamepad->GetRawButton(0))
		{
			liftToBoxHeight->Start();

			while(liftToBoxHeight->Get() < 1.05)
			{
				m_screw->Set(0.7);
			}
			m_screw->Set(0.0);
			liftToBoxHeight->Reset();
			smart->PutString("Lift status", "At box height");
		}
	}

	void Autonomous()
	{
		switch(autonSwitch)
		{
			case 0:
				m_armClose->Set(true);
				autonSwitch = 1;
				break;

			case 1:
				autonLiftTimer->Start();
				while(autonLiftTimer < 0.5)					//time is subject to change
				{
					m_screw->Set(0.7);
				}
				autonLiftTimer->Reset();
				m_screw->Set(0.0);
				autonSwitch = 2;
				break;

			case 2:
				autonTurnTimer->Start();
				while(autonTurnTimer < 0.5)					//time is subject to change
				{
					m_robotDrive->TankDrive(0.0 , 0.7);
				}
				autonTurnTimer->Reset();
				m_robotDrive->TankDrive(0.0 , 0.0);
				autonSwitch = ;
				break;

			case 3:											//robot goes over the step
				autonDriveTimer->Start();
				while(autonDriveTimer < 3.5)				//time is subject to change
				{
					m_robotDrive(0.7 , 0.7);
				}
				autonDriveTimer->Reset();
				m_robotDrive(0 , 0);
				autonSwitch = 5;
				break;

			case 4:											//robot stays off the step
				autonDriveTimer->Start();
				while(autonDriveTimer < 3.0)				//time is subject to change
				{
					m_robotDrive(0.7 , 0.7);
				}
				autonDriveTimer->Reset();
				m_robotDrive(0 , 0);
				autonSwitch = 5;
				break;

			case 5:
				while(liftLimitSwitch->Get())
				{
					m_screw->Set(-0.7);
				}
				m_screw->Set(0.0);
				autonSwitch = 6;
				break;

			case 6:
				m_armOpen->Set(true);
				autonSwitch = 7;
				break;

			case 7:
				autonDriveTimer->Start();
				while(autonDriveTimer < 0.4)
				{
					m_robotDrive->TankDrive(0.7 , 0.7);
				}
				autonDriveTimer->Reset();
				m_robotDrive->TankDrive(0.0, 0.0);
		}
	}
};

START_ROBOT_CLASS(Robot);
