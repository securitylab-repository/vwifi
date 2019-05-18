#include <cstdio> //perror

#include <iostream> // cout

#include <sys/socket.h> //socket
#include <arpa/inet.h> // struct sockaddr_in & inet_ntoa & ntohs

#include "csocket.h"

using namespace std;

CSocket::CSocket()
{
	MasterSocket=0;
}

bool CSocket::Configure()
{
	//create a master socket
	if( (MasterSocket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
	{
		perror("socket failed");
		return ERROR_SOCKET;
	}

	return 0;
}

int CSocket::GetMasterSocket()
{
	return MasterSocket;
}

ssize_t CSocket::Send(Descriptor descriptor, const char* data, ssize_t sizeOfData)
{
	return send(descriptor, data, sizeOfData, 0);
}

ssize_t CSocket::Read(Descriptor descriptor, char* data, ssize_t sizeOfData)
{
	return recv(descriptor , data, sizeOfData, 0);
}

void CSocket::ShowInfo(Descriptor descriptor)
{
	struct sockaddr_in address;
	int addrlen = sizeof(address);

	getpeername(descriptor , (struct sockaddr*)&address , (socklen_t*)&addrlen);
	cout<<"ip "<<inet_ntoa(address.sin_addr)<<" , port "<<ntohs(address.sin_port);
}


