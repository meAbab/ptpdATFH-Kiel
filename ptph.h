#ifndef ptph_H
#define ptph_H

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cstring>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <time.h>
#include <cstdlib>

extern double T1_p_time, T1_time, T2_time;
extern long  sending_client_microsecond;

int read_time();

int time_server();

int client_udp(int argc,char *argv[]);

int ptp_client();

#endif
