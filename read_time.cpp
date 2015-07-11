/* creating a file according server time */

#include "ptph.h"

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

int read_time()
{
	ofstream writeTimetoFile;
	ptime current_date_microseconds = microsec_clock::local_time();
	ptime time_t_epoch(date(1970,1,1));
	
	time_duration td = current_date_microseconds - time_t_epoch;
		
	long total_microseconds = td.total_microseconds();
	cout <<"1. Server time in MicroSeconds : " << total_microseconds <<'\n';
		
	writeTimetoFile.open("my_time");
		
	writeTimetoFile << fixed << total_microseconds << '\n';
	//writeTimetoFile << micro_sec;
	
	return 0;
}