/* 
	UDP client  file. First generating a connection to server, on port 4221 [predefined]. Server IP address need to given as argument.
	Ask to your partner for IP address, which comes from time_server.cpp file.
	
	Four different time, T1, T1', T2 and T2' has been calculated here to figure the offset time. 
	
	Client time is set twice - first in line 59 by calling ptp_client.cpp and then again after calculate the offset time.
*/
#include "ptph.h"

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

int client_udp(int argc, char *argv[])
{
   int sock, n;
   unsigned int length;
   struct sockaddr_in server, from;
   struct hostent *ip;
   char buffer[256];
   double T1_p_time, T1_time, T2_time;
   struct timeval sync_serv_client;
         
   sock= socket(AF_INET, SOCK_DGRAM, 0);
   
   if (sock < 0) cerr <<"error connecting server";

   server.sin_family = AF_INET;
   server.sin_port = htons(4221);

   if(argc !=2 )
   {
	   cout << "No server found:...";
	   exit(1);
   }
   
   server.sin_addr.s_addr = inet_addr(argv[1]);
   
   length=sizeof(struct sockaddr_in);
   
   // as datagram client generate first connection request so to trace
   
   sendto(sock,buffer, strlen(buffer),0,(const struct sockaddr *)&server,length);
   
   bzero(buffer,256);
   
   recvfrom(sock,buffer,200,0,(struct sockaddr *)&from, &length); //T1 time
   T1_time = static_cast<double>(atol(buffer));
   
   cout << "First time in buffer: " << fixed << T1_time <<'\n';
      
   ofstream outfile;
   outfile.open("my_time");
   outfile << buffer;
   outfile.close();
   
   // ptp_client - to set client time according the file got from server. e.g. my_file
   ptp_client();
   
   recvfrom(sock,buffer,100,0,(struct sockaddr *)&from, &length); // T1' time
   T1_p_time = static_cast<double>(atol(buffer));
   
   cout << " Secon Time: " << fixed << T1_p_time <<'\n';
   
   
   // taking client current time and sending to server
   
   	ptime sending_client_time = microsec_clock::local_time();
	ptime time_t_epoch(date(1970,1,1));
	time_duration td = sending_client_time - time_t_epoch;
	long sending_client_microsecond = td.total_microseconds();
	ostringstream stoser;

	stoser << sending_client_microsecond;
   
    sendto(sock, stoser.str().c_str(), 100, 0, (struct sockaddr*)&server, length); //T2' time
	cout << " Third Time: " << sending_client_microsecond <<'\n';
	
    recvfrom(sock,buffer,100,0,(struct sockaddr *)&from, &length); //T2 Time
	T2_time = static_cast<double>(atol(buffer));
	
	cout <<" Forth Time: " << fixed << T2_time << '\n';
   
	
	// +++++ Calculation and updating Client time ++++
		
	double offset = (T1_p_time - T1_time - sending_client_microsecond + T2_time)/2;  // calculating offset time
	cout << "\nOffset time: " << offset;  
	
	ptime current_client_time = microsec_clock::local_time();
	ptime time_t_epoch_client(date(1970,1,1));
	time_duration client_td = current_client_time - time_t_epoch_client;
		
	long current_client_microsecond = client_td.total_microseconds() + offset;  // adding offset time to sync to server
	cout << "\n\nCurrent Client Time plus Offset: " << current_client_microsecond;
	
	sync_serv_client.tv_usec = current_client_microsecond%1000000;
				
	sync_serv_client.tv_sec = (current_client_microsecond - sync_serv_client.tv_usec)/1000000;
			
	cout << " \n Client update Seconds: " << sync_serv_client.tv_sec;
	cout << " \n Client update Microseconds: " << sync_serv_client.tv_usec;
	
	//applying the calculated time to machine
	
	int cu = settimeofday(&sync_serv_client, NULL);
	if(cu==0)
		{
			cout << "\nSystem Time Updated.";
		}
		else{
			cerr << "\n\n ... Failed update system Time. \n\n";
			exit(1);
		}
		
    close(sock);
    return 0;
}
