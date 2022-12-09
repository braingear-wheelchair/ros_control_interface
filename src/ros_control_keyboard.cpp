#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/UInt8MultiArray.h>
#include "ros_control_interface/ServiceKeyboard.h"

constexpr char KEY_FORWARD		= 'w';
constexpr char KEY_BACKWARD		= 's';
constexpr char KEY_LEFT			= 'a';
constexpr char KEY_RIGHT		= 'd';
constexpr char KEY_TURN_LEFT	= 'q';
constexpr char KEY_TURN_RIGHT	= 'e';
constexpr char KEY_STOP			= 'x';
constexpr char KEY_BUTTON_A		= 'u';
constexpr char KEY_BUTTON_B		= 'i';
constexpr char KEY_BUTTON_C		= 'o';
constexpr char KEY_BUTTON_D		= 'p';
constexpr char KEY_QUIT			=  27;	// ESC

constexpr float VELOCITY_MAX 				=  1.0f;
constexpr float VELOCITY_MIN 				= -1.0f;
constexpr float DEFAULT_VELOCITY_INCREMENT 	= 0.05f;
constexpr int   DEFAULT_NUM_BUTTONS 		= 4;

float normalize_velocity(float x) {

	if(x > VELOCITY_MAX)
		x = VELOCITY_MAX;
	
	if(x < VELOCITY_MIN)
		x = VELOCITY_MIN;

	return x;
}

geometry_msgs::Twist set_analog_message(float vx, float vy, float vz) {

	geometry_msgs::Twist msg;
	
	msg.linear.x  = vx;
	msg.linear.y  = vy;
	msg.linear.z  = 0.0f;
	msg.angular.x = 0.0f;
	msg.angular.y = 0.0f;
	msg.angular.z = vz;

	return msg;
}

std_msgs::UInt8MultiArray set_discrete_message(int button_id) {

	std_msgs::UInt8MultiArray msg;
	msg.data.assign(DEFAULT_NUM_BUTTONS, 0);
	std_msgs::MultiArrayDimension dim;
	dim.size   = 1;
	dim.stride = 0;

	for(auto i = 0; i<DEFAULT_NUM_BUTTONS; i++) {
		dim.label  = "button " + std::to_string(i);
		msg.layout.dim.push_back(dim);
	}

	if (button_id >= 0 & button_id < DEFAULT_NUM_BUTTONS)
		msg.data[button_id] = true;

	return msg;
}

int main(int argc, char* argv[]) {


	ros::init(argc, argv, "ros_control_keyboard");
	ros::NodeHandle nh;
	ros::Publisher pub_analog   = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
	ros::Publisher pub_discrete = nh.advertise<std_msgs::UInt8MultiArray>("/button", 1);

	roscontrol::ServiceKeyboard keyboard;

	keyboard.Start();

	ros::Rate r(16);
	geometry_msgs::Twist 	 analog;
	std_msgs::UInt8MultiArray discrete;

	int key;
	float VELOCITY_INCREMENT;
	float vx = 0.0f, vy = 0.0f, vz = 0.0f;
	int button_id = -1;
	bool run 			= true;
	bool has_hit 		= false;
	bool first_no_input = true;
	bool is_analog 		= true;
	bool is_discrete 	= false;
	bool reset_input;


	/*** Gathering parameters ***/
	ros::param::param<float>("~increment", VELOCITY_INCREMENT, DEFAULT_VELOCITY_INCREMENT);
	ros::param::param<bool>("~reset_input", reset_input, false);
	

	while(ros::ok() && run == true) {

		/*** Getting current press key ***/
		has_hit = keyboard.HasHit();
		if(has_hit == true) {
			key = keyboard.GetKey();
		} else {
			key = 0;
		}

		/*** Select the vx, vy, vz velocities accordingly ***/
		switch((char)key) {
			case KEY_FORWARD:
				vx = normalize_velocity(vx + VELOCITY_INCREMENT);
				is_analog 	= true;
				break;
			case KEY_BACKWARD:
				vx = normalize_velocity(vx - VELOCITY_INCREMENT);
				is_analog 	= true;
				break;
			case KEY_RIGHT:
				vy = normalize_velocity(vy + VELOCITY_INCREMENT);
				is_analog 	= true;
				break;
			case KEY_LEFT:
				vy = normalize_velocity(vy - VELOCITY_INCREMENT);
				is_analog 	= true;
				break;
			case KEY_TURN_LEFT:
				vz = normalize_velocity(vz + VELOCITY_INCREMENT);
				is_analog 	= true;
				break;
			case KEY_TURN_RIGHT:
				vz = normalize_velocity(vz - VELOCITY_INCREMENT);
				is_analog 	= true;
				break;
			case KEY_BUTTON_A:
				button_id = 0;
				is_analog   = false;
				break;
			case KEY_BUTTON_B:
				button_id = 1;
				is_analog   = false;
				break;
			case KEY_BUTTON_C:
				button_id = 2;
				is_analog   = false;
				break;
			case KEY_BUTTON_D:
				button_id = 3;
				is_analog   = false;
				break;
			case KEY_QUIT:
				run = false;
				is_analog   = false;
				break;
			case KEY_STOP:
				vx = 0.0f;
				vy = 0.0f;
				vz = 0.0f;
				is_analog   = true;
			default:
				has_hit = false;
				break;
		}


		if(has_hit == true) {
			if(is_analog == true) {
				analog = set_analog_message(vx, vy, vz);
				pub_analog.publish(analog);
			} else { 
				discrete = set_discrete_message(button_id);
				pub_discrete.publish(discrete);
				button_id = -1;
			}

			first_no_input = true;
		} else if (first_no_input == true && reset_input == true) {
			vx = 0; vy = 0; vz = 0;

			if(is_analog == true) {
				analog = set_analog_message(vx, vy, vz);
				pub_analog.publish(analog);
			}
			
			first_no_input = false;
		}
		
		ros::spinOnce();
		r.sleep();
	}


	return 0;



}
