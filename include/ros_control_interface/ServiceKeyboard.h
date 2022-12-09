#ifndef ROS_CONTROL_INTERFACE_KEYBOARD_H_
#define ROS_CONTROL_INTERFACE_KEYBOARD_H_

#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <cstdio>
#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <mutex>
#include <fcntl.h>
#include "Service.h"

namespace roscontrol {

class ServiceKeyboard : public Service {
	public:
		ServiceKeyboard(void);
		~ServiceKeyboard(void);

		bool HasHit(void);
		int GetKey(void);
		void Run(void);

	private:
		int kbhit(void);
		int getch(void);
		void enable_raw_mode(void);
		void disable_raw_mode(void);

	private:
		bool run_;
		int peek_character;
		int key_;
		termios term_;
		std::mutex kmutex_;
		bool is_new_;

};


}
#endif
