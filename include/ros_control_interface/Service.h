#ifndef ROS_CONTROL_INTERFACE_SERVICE_H_
#define ROS_CONTROL_INTERFACE_SERVICE_H_

#include <chrono>
#include <mutex>
#include <thread>

namespace roscontrol {

class Service {

	public:
		Service(void);
		~Service(void);

		virtual void Run(void) = 0;
		void Start(void);
		void Stop(void);
		void Join(void);
		bool IsRunning(void);
		std::string name(void);
		void SleepFor(int ms);

	protected:
		std::string name_;
	private:
		bool run_;
		std::thread thread_;
		std::mutex mutex_;

};

}


#endif
