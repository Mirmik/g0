#include <gxx/print.h> 
#include <gxx/inet/dgramm.h>

int main() {
	gxx::inet::udp_socket usock("0.0.0.0", 12233);
	usock.sendto("127.0.0.1", 11001, "\x02", 1);

	char buf[1];
	usock.recvfrom(buf, 1, nullptr);
	gxx::println("correct handshake");	
}