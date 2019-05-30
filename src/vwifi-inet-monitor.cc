#include "cwirelessdevice.h"
#include "cmonwirelessdevice.h"
#include <stdio.h>
#include <iostream>

#include <unistd.h>
int main (int agc, char ** argv){


	struct ether_addr mac { 0xff, 0x00, 0x00,0x00, 0xfe, 0x00 };
	struct ether_addr mac3 { 0xaa, 0xbb, 0x00,0x00, 0xfe, 0x00 };

	WirelessDevice wdevice("wlan0",4,5,mac);
	std::cout << wdevice << std::endl ;

	WirelessDevice wdevice2 ;
	wdevice2 = wdevice;
	
	wdevice.setMacaddr(mac3);
	
	std::cout << wdevice2 << std::endl ;
	std::cout << wdevice << std::endl ;

	MonitorWirelessDevice monitor ;

	monitor.start();

	sleep(4);

	std::cout << "end" << std::endl ;
	monitor.stop();

	return 0 ;



}
