#include <gxx/print.h> 
#include <gxx/inet/dgramm.h>

int main() {
	gxx::inet::udp_socket usock("127.0.0.1", 11001);

	char buf[128];
	int sz = usock.recvfrom(buf, 128, nullptr);

	gxx::print_dump((void*)buf, sz);
}