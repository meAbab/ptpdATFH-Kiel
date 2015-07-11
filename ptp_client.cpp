/* client to update time

#include<iostream>
#include <fstream>
#include <sys/time.h>
#include <cstdlib>
*/
#include "ptph.h"

using namespace std;
int ptp_client()
{
	ifstream read_time_file;
	long long i;
	struct timeval now;
	
	read_time_file.open("my_time");
	
	while(read_time_file >> i)
	{
			now.tv_usec = i%1000000;
				
			now.tv_sec = (i - now.tv_usec)/1000000;
			
			// I didn't get any answer of why it need to subtract 7200 sec. But exactly that amount of error is coming.
			
			now.tv_sec = now.tv_sec - 7200;
	}
	
	cout << " Seconds: " << now.tv_sec <<'\n';
	cout << "MicroSec: " << now.tv_usec<< '\n';
	int rc = settimeofday(&now, NULL);
	if(rc==0)
	{
		cout << "Taken first Server Time."<< '\n';
	}
	
	else{
		cerr << "\n\n ... Failed update system Time. \n\n";
		exit(1);
	}
	return 0;
}