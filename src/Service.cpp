#include "ros_control_interface/Service.h"

namespace roscontrol {

Service::Service(void) {
	this->mutex_.lock();
	this->run_ = false;
	this->mutex_.unlock();
}

Service::~Service(void) {
	if(this->IsRunning() == true) {
		this->Stop();
		this->Join();
	}
}

void Service::Start(void) {

	if(this->IsRunning() == true) {
		this->Stop();
		this->Join();
	}

	printf("[%s] Service is requested to start\n", this->name().c_str());
	this->mutex_.lock();
	this->thread_ = std::thread(&Service::Run, this);
	this->run_ = true;
	this->mutex_.unlock();
	
}

void Service::Stop(void) {
	printf("[%s] Service is requested to stop\n", this->name().c_str());
	this->mutex_.lock();
	this->run_ = false;
	this->mutex_.unlock();
}

bool Service::IsRunning(void) {
	bool ret;
	this->mutex_.lock();
	ret = this->run_;
	this->mutex_.unlock();
	return ret;
}

void Service::Join(void) {
	printf("[%s] Service is requested to join\n", this->name().c_str());
	if(this->thread_.joinable() == true)
		this->thread_.join();
}

void Service::SleepFor(int ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));

}

std::string Service::name(void) {
	std::string name;
	this->mutex_.lock();
	name = this->name_;
	this->mutex_.unlock();
	return name;
}



}
