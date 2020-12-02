#include <iostream>
#include <list>
#include <iterator>
#include <unistd.h>

using namespace std;

// Process class
class Process{
	private:
		int id = 0;
		int rTime = 0;
		int priority = 0;

	public:
		// Constructor
		Process(int v_id, int v_rTime, int v_priority){
			id = v_id;
			rTime = v_rTime;
			priority = v_priority;
		};

		// Decrement rTime by 1 second
		void decRTime(){
			rTime = rTime - 1;
		};

		// Return functions
		int getid(){
			return id;
		};

		int getrTime(){
			return rTime;
		};

		int getpriority(){
			return priority;
		};
};

int main(){
	// Process creation
	Process pro1(1000, 5, 4);
	Process pro2(1001, 4, 3);
	Process pro3(1002, 6, 4);
	Process pro4(1003, 2, 6);
	Process pro5(1004, 3, 1);

	// List creation
	list<Process> ready, executing, finished;

	// Populate ready list
	ready.push_back(pro1);
	ready.push_back(pro2);
	ready.push_back(pro3);
	ready.push_back(pro4);
	ready.push_back(pro5);

	// While a process is not executing
	while(executing.empty()){
		// End program if nothing is in ready list
		if (ready.empty())
		{
			break;
		}

		// Chooses what to execute based on priority
		list<Process>::iterator it,it2;
		int prior = 100000;
		for (it=ready.begin(); it!=ready.end(); it++){
			// If priority of current ready element is less than saved priority (high by default)
			// then set new saved priority, set it2 to it for erasing later, and push to executing
			// list after clearing (in the case of finding higher priority more than once)
			if (it->getpriority() < prior){
				prior = it->getpriority();
				it2 = it;
				executing.clear();
				executing.push_back(*it);
			}
		}
		ready.erase(it2);

		// "Execute" the process. Decrease time by 1 for every second passed.
		cout << "-- Beginning Execution of Process " << executing.front().getid() << " with Priority " << executing.front().getpriority() << " --" << endl;
		while(executing.front().getrTime() != 0){
			cout << "Process " << executing.front().getid() << " with Remaining Time of " << executing.front().getrTime() << endl;
			usleep(1000000);
			executing.front().decRTime();
		}
		cout << "Process " << executing.front().getid() << " finished and moved to Finished List" << endl << endl;

		// Move to finish list and clear executing list
		finished.push_back(executing.front());
		executing.clear();
	}
}