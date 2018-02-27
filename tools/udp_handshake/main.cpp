#include <gxx/print.h> 
#include <gxx/inet/dgramm.h>
#include <gxx/osutil/signal.h>
#include <chrono>

using namespace std::chrono_literals;

bool success = false;

void usock_signal_handler(int signal) {
	success = true;
}

int main(int argc, char* argv[]) {
	if (argc < 3) {
		gxx::println("Usage: g0_udp_handshake ip port");
		exit(0);
	}

	std::string addr = argv[1];
	std::string port = argv[2];

	gxx::inet::udp_socket usock;

	gxx::osutil::setsig(usock.fd, SIGUSR1);
	gxx::osutil::signal(SIGUSR1, usock_signal_handler);

	usock.sendto(addr.c_str(), std::stoi(port), "\x02", 1);

	auto start = std::chrono::system_clock::now();
	while( !success && std::chrono::system_clock::now() - start < 1000ms);

	if ( success ) {
		gxx::fprintln("correct handshake with {}:{}", addr, port);	
	} else {
		gxx::fprintln("timeout for handshake with {}:{}", addr, port);	
	}

	usock.close();
}