# ptpdATFH-Kiel
This programs I wrote for my Advance C++ course in FH-Kiel. Task was to create a simple PTPD daemon for LAN.
I've used boost, C++ networking (sock, getifaddrs, bind etc), and read/write file functions to make it.

Program is very small and very easy to understand. Offset calclation done as 
" (T1_p_time - T1_time - sending_client_microsecond + T2_time)/2"

On test, it able to synchronize upto 3 digit in millisecond.
