#include <Servo.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>

#define LEFT_PIN 6
#define RIGHT_PIN 7

#define wheel_track 0.65
#define wheel_speed_ratio 1.0   //need calibrate

#define MAX_SPEED_M_PER_S   2.0
#define NETRUAL_POINT   1500
#define LOW_DEADZONE    1200    //need calibrate
#define HIGH_DEADZONE   1800    //need calibrate

ros::NodeHandle nh;

Servo motor_left;
Servo motor_right;

unsigned long lastCmdTime;


void velCallback(const geometry_msgs::Twist& vel)
{
    lastCmdTime = millis();

    float x = vel.linear.x;    
    float za = vel.angular.z;

    movement(x, za);
}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", velCallback);

void setup()
{
    //servo as motor control init
    motor_left.attach(LEFT_PIN);
    motor_right.attach(RIGHT_PIN);

    //set default netrual point
    setMotorStop();

    nh.initNode();
    nh.subscribe(sub);
}

void loop()
{
    nh.spinOnce();
    delay(10);

    //auto stop protect every 1s
    if((millis() - lastCmdTime) >= 1000)
    {
        lastCmdTime = millis();
        setMotorStop();
    }
}

void movement(float x_vel, float z_ang)
{
    float left = 0.0;
    float right = 0.0;
    //turn in place
    if(x_vel == 0)
    {
        right = z_ang * wheel_track / 2;
        left = -right;
    }
    else if(z_ang == 0)
    {
        left = right = x_vel;
    }
    else
    {
        left = x_vel - z_ang * wheel_track / 2;
        right = x_vel + z_ang * wheel_track / 2;
    }

    //due to different surface of the ground
    //need to calibrate the wheel_speed_ratio
    left = left * wheel_speed_ratio;
    right = right * wheel_speed_ratio;

    //MAX speed limit
    left = limitSpeed(left);
    right = limitSpeed(right);

    setMotorSpeed(left,right);
}

float limitSpeed(float spd)
{
    if(spd > MAX_SPEED_M_PER_S)
    {
        return MAX_SPEED_M_PER_S;
    }
    else if(spd < (-MAX_SPEED_M_PER_S))
    {
        return -MAX_SPEED_M_PER_S;
    }
    else
    {
        return spd;
    }
}

void setMotorSpeed(float ls, float rs)
{
    int lp = map(ls, -MAX_SPEED_M_PER_S, MAX_SPEED_M_PER_S, 1000, 2000);
    int rp = map(rs, -MAX_SPEED_M_PER_S, MAX_SPEED_M_PER_S, 1000, 2000);

    //remove dead zone of motor and config the output
    motor_left.writeMicroseconds(removeDeadZone(lp));
    motor_right.writeMicroseconds(removeDeadZone(rp));
}

int removeDeadZone(int pulse)
{
    if((pulse < NETRUAL_POINT) && (pulse > LOW_DEADZONE))
    {
        return LOW_DEADZONE;
    }
    else if((pulse > NETRUAL_POINT) && (pulse < HIGH_DEADZONE))
    {
        return HIGH_DEADZONE;
    }
    else
    {
        return pulse;
    }
}

void setMotorStop()
{
    motor_left.writeMicroseconds(NETRUAL_POINT);
    motor_right.writeMicroseconds(NETRUAL_POINT);
}