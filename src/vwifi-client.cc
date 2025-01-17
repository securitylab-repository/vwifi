#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <string.h> // strcmp

#include <memory>

#include "config.h" // DEFAULT_WIFI_CLIENT_PORT_VHOST / DEFAULT_WIFI_CLIENT_PORT_INET

#include "cwificlient.h"
#ifdef ENABLE_VHOST
#include "csocketclientvtcp.h"
#endif
#include "csocketclientitcp.h"

enum STATE {

	STARTED=1,
	STOPPED ,
	SUSPENDED
};

CKernelWifi* wifiClient;

enum STATE  _state = STOPPED ;


void  signal_handler(int signal_num)
{

	switch(signal_num)
	{
		case SIGINT :
		case SIGTERM :
		case SIGQUIT :

			std::cout << signal_num << std::endl ;
			wifiClient->stop() ;
			_state = STOPPED ;
			break ;

		case SIGTSTP:

			std::cout << "This signal is ignored" << std::endl ;
			break ;


		default :
			std::cerr << "Signal not handled" << std::endl ;

	}

	std::cout << "OUT SWITCH" << std::endl ;
}

void help()
{
	std::cout<<"Usage: vwifi-client [-h] [-v] [-s] [IP_ADDR] [-p PORT]"<<std::endl;
	std::cout<<"                    [--help] [--version] [--spy] [IP_ADDR] [--port PORT]"<<std::endl;
	std::cout<<"         By default : client mode : TCP : IP_ADDR="<<DEFAULT_ADDRESS_IP <<" PORT="<< DEFAULT_WIFI_CLIENT_PORT_INET << std::endl;
	std::cout<<"                      client mode : VHOST : PORT="<< DEFAULT_WIFI_CLIENT_PORT_VHOST << std::endl;
	std::cout<<"                      spy mode (--spy) : IP_ADDR="<< DEFAULT_ADDRESS_IP <<" PORT="<< DEFAULT_WIFI_SPY_PORT << std::endl;
}

int main (int argc , char ** argv){

	bool spy = false;

	std::string ip_addr;
	TPort port_number = 0;

	int arg_idx = 1;
	while (arg_idx < argc)
	{
		if( ! strcmp("-v", argv[arg_idx]) || ! strcmp("--version", argv[arg_idx]) )
		{
			std::cout<<"Version : "<<VERSION<<std::endl;
			return 0;
		}
		if( ! strcmp("-h", argv[arg_idx]) || ! strcmp("--help", argv[arg_idx]) )
		{
			help();
			return 0;
		}
		if( ( ! strcmp("-p", argv[arg_idx]) || ! strcmp("--port", argv[arg_idx]) ) && (arg_idx + 1) < argc)
		{
			port_number = std::stoi(argv[arg_idx+1]);
			arg_idx++;
		}
		else if( ! strcmp("-s", argv[arg_idx]) || ! strcmp("--spy", argv[arg_idx]) )
		{
			spy=true;
		}
		else
		{
			if( ip_addr.empty() )
				ip_addr = std::string(argv[arg_idx]);
			else
			{
				std::cerr<<"Error : unknown parameter : "<< argv[arg_idx] <<std::endl;
				help();
				return 1;
			}
		}

		arg_idx++;
	}

	if( getuid() )
	{
		std::cerr<<"Error : This program must be run as root!!"<<std::endl;
		return 2;
	}

	/* Handle signals */
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGQUIT, signal_handler);
	signal(SIGHUP, SIG_IGN);
	signal(SIGTSTP, signal_handler);
	//signal(SIGCONT, signal_handler);

	if( spy )
	{ // mode TCP

		if( ip_addr.empty() )
			ip_addr = std::string(DEFAULT_ADDRESS_IP);

		if( ! port_number )
			port_number = DEFAULT_WIFI_SPY_PORT;

		std::cout<<"Type : AF_INET"<<std::endl;
		wifiClient=new CWifiClient<CSocketClientITCP>;
		static_cast<CWifiClient<CSocketClientITCP>*>(wifiClient)->Init(ip_addr.c_str(), port_number);
	}
	else
	{
		if( ip_addr.empty() )
		{ // IP not set -> mode VHOST
#ifdef ENABLE_VHOST
			if( ! port_number )
				port_number = DEFAULT_WIFI_CLIENT_PORT_VHOST;

			std::cout<<"Type : AF_VSOCK"<<std::endl;
			wifiClient=new CWifiClient<CSocketClientVTCP>;
			static_cast<CWifiClient<CSocketClientVTCP>*>(wifiClient)->Init(port_number);
#else
			std::cerr<<"Error : This program is not build with VHOST!!"<<std::endl;
			return 3;
#endif
		}
		else
		{ // mode TCP
			if( ! port_number )
				port_number = DEFAULT_WIFI_CLIENT_PORT_INET;

			std::cout<<"Type : AF_INET"<<std::endl;
			wifiClient=new CWifiClient<CSocketClientITCP>;
			static_cast<CWifiClient<CSocketClientITCP>*>(wifiClient)->Init(ip_addr.c_str(), port_number);
		}
	}

	if(!wifiClient->start())
		std::cout << "Starting process aborted" << std::endl ;



	std::cout << "Good Bye (:-)" << std::endl ;

	_exit(EXIT_SUCCESS);

}
