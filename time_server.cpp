/* 	
	This is UDP server side file. it will create and open a connection on port 4221 [pre-defined / given at line 28]
	and accept connection from anywhere.
	This server program is running on wlan0 interface, if want to run the program on other interface change strcpy wlan0 to 
	your desired interface at line 43.
	Four different time tracked here as, T1, T1', T2' and T2 time. The calculation of these time is given in client_udp.cpp file.
	
	
*/
#include <ifaddrs.h>
#include <netdb.h>
#include "ptph.h"

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

int time_server(void)
{
	struct sockaddr_in serv_addr, client_addr;
	struct ifaddrs *ifaddr, *ifa;
	int sock_d, i,family, s;
	ptime sending_time = microsec_clock::local_time();
	char buffer[100];
	string buf_time;
	char cl_buf[100];
	char host[NI_MAXHOST];
	
	sock_d = socket(AF_INET, SOCK_DGRAM, 0);
	
	socklen_t slen=sizeof(client_addr);
	bzero(&serv_addr,sizeof(serv_addr));
		
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(4221);
	
	// search IP on wlan0 interface
	
	getifaddrs(&ifaddr);
		
	for(ifa = ifaddr; ifa!=NULL; ifa = ifa->ifa_next)
		{
			if (ifa->ifa_addr == NULL)
				continue;
			
			s = getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
	
			if((strcmp(ifa->ifa_name,"wlan0")==0)&&(ifa->ifa_addr->sa_family==AF_INET))
			{
			serv_addr.sin_addr.s_addr = inet_addr(host);
			}
		}
			
	bind(sock_d, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	while(1)
	{
		
		i = recvfrom(sock_d, cl_buf, 100,0,(struct sockaddr*)&client_addr,&slen);
		
		write(1,"Received a datagram: ",21);
		//write(1,buffer,i);
		
		ifstream infile;
		
		read_time();
		
		infile.open("my_time");
				
		while (getline(infile,buf_time))
		{
			infile >> buf_time;
		}
	
		strcpy(buffer, buf_time.c_str());
		
		cout << "\nServer time is :" << buffer;
		
		ptime sending_time = microsec_clock::local_time();
		ptime time_t_epoch(date(1970,1,1));
		time_duration td = sending_time - time_t_epoch;
		long sending_microsecond = td.total_microseconds();
		ostringstream stocl;
		char sending_to_client[25];
		stocl << sending_microsecond;
				
		sendto(sock_d, buffer, 100, 0, (struct sockaddr*)&client_addr, slen); // T1 time
		sendto(sock_d, stocl.str().c_str(), 100, 0, (struct sockaddr*)&client_addr, slen); // T1' time
		long client_time_microsecond = recvfrom(sock_d, cl_buf, 100,0,(struct sockaddr*)&client_addr,&slen); //T2' time
		
		ptime sending_T2_serv_time = microsec_clock::local_time();
		ptime time_t_epoch2(date(1970,1,1));
		time_duration td_T2 = sending_T2_serv_time - time_t_epoch2;
		long T2_serv_microsecond = td_T2.total_microseconds();
		ostringstream stoclT2;
		stoclT2 << T2_serv_microsecond;
		sendto(sock_d, stoclT2.str().c_str(), 100, 0, (struct sockaddr*)&client_addr, slen); //T2 time
		
		
	}
	
	
}