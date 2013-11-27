#include "request.h"

#include <sstream>
#include <winsock.h>

std::string request(std::string host, std::string url, std::string* session) {
	struct sockaddr_in server;
	struct hostent *host_info;
	int sock;
	
	WSAData data;
	if( WSAStartup(MAKEWORD(2,0), &data) != 0 )
		throw std::runtime_error("WSAStartUp failed.");

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( sock == INVALID_SOCKET )
		throw std::runtime_error("Socket Creation failed.");

	host_info = gethostbyname( host.c_str() );
	if( host_info == NULL )
		throw std::runtime_error("Could not resolve hostname.");

	server.sin_family = AF_INET;
	server.sin_port = htons(80);
	memcpy(&server.sin_addr, host_info->h_addr, host_info->h_length);

	if( connect(sock, (const sockaddr*)&server, sizeof(server)) < 0 ) {
		std::stringstream error;
		error << "Could not connect to server: " << WSAGetLastError();
		throw std::runtime_error(error.str().c_str());
	}

	std::stringstream request;
	request << "GET " << url << " HTTP/1.0\r\n";
	request << "Host: " << host << "\r\n";
	
	if( session )
		request << "Cookie: " << "PHPSESSID=" << *session << "\r\n";
		
	request << "\r\n";

	send(sock, request.str().c_str(), request.str().length(), 0);
	char buffer[1024];
	size_t count;
	std::stringstream response;
	do {
		count = recv(sock, buffer, sizeof(buffer) - 1, 0);
		buffer[count] = '\0';
		response << buffer;
	} while( count > 0 );

	closesocket(sock);

	return response.str();
}