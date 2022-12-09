#include "ros_control_interface/ServiceKeyboard.h"


namespace roscontrol {

ServiceKeyboard::ServiceKeyboard(void) {
	this->name_ = "keyboard_service";
	this->is_new_ = false;

	int flags = fcntl(0, F_GETFL, 0);
	fcntl(0, F_SETFL, flags | O_NONBLOCK); 
}

ServiceKeyboard::~ServiceKeyboard(void) {
	int flags = fcntl(0, F_GETFL, 0);
	fcntl(0, F_SETFL, flags | ~O_NONBLOCK); 
}

void ServiceKeyboard::Run(void) {

	char key;
	bool isrunning = true;
	
	printf("[%s] Service is up\n", this->name().c_str());
	
	this->kmutex_.lock();
	this->enable_raw_mode();
	this->kmutex_.unlock();
	
	while(isrunning == true) {

		this->kmutex_.lock();
		if(this->kbhit()) {
			this->key_ = this->getch();
			this->is_new_ = true;
		}
		this->kmutex_.unlock();
		
		
		isrunning = this->IsRunning();
		
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	this->kmutex_.lock();
	this->disable_raw_mode();
	this->kmutex_.unlock();
	tcflush(0, TCIFLUSH);
	tcflush(0, TCOFLUSH);
	
	printf("[%s] Service is down\n", this->name().c_str());

}

bool ServiceKeyboard::HasHit(void) {

	bool retcod = false;
	this->kmutex_.lock();
	retcod = this->is_new_;
	this->kmutex_.unlock();

	return retcod;
}

int ServiceKeyboard::GetKey(void) {

	int key = 0;

	this->kmutex_.lock();
	if(this->is_new_) {	
		key = this->key_;
		this->is_new_ = false;
	}
	this->kmutex_.unlock();

	return key;
	
}

int ServiceKeyboard::kbhit(void) {
	
	unsigned char ch;
    int nread;
	
	if (peek_character != -1) {
		return 1;
	}

    nread = read(0,&ch,1);

    if (nread == 1){
        peek_character = ch;
        return 1;
    }
    return 0;
}

int ServiceKeyboard::getch(){
    char ch;
	int nread;

    if (peek_character != -1){
        ch = peek_character;
        peek_character = -1;
	}

    return ch;
}

void ServiceKeyboard::enable_raw_mode() {
    termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ICANON | ECHO); // Disable echo as well
    tcsetattr(0, TCSANOW, &term);
}

void ServiceKeyboard::disable_raw_mode() {
    termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, &term);
}



}

