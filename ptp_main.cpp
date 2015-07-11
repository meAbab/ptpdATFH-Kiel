#include "ptph.h"

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

int main(int argc, char *argv[])
{

	if (argc == 2)
		{	
			client_udp(argc, argv);
			
		}else if(argc == 1) 
		{
			//read_time();
			cout << " Program is running as Server mode ... ";
			time_server();
		}
		else {
			cout << "You gave too many argument. Either give the IP or just do NOTHING." << endl;
		}
	return 0;
}