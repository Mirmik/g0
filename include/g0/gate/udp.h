#ifndef G0_UDP_GATE_H
#define G0_UDP_GATE_H

#include <g0/network.h>
#include <gxx/inet/dgramm.h>

namespace g0 {

	struct udp_gate_address {
		gxx::hostaddr addr;
		uint16_t port;
		udp_gate_address(gxx::hostaddr& str, int port) : addr(str), port(port) {}
	};

	class udp_gate : public g0::service {
	public:
		gxx::inet::udp_socket sock;

		std::vector<udp_gate_address> table;

		//udp_gate_address* table;
		udp_gate(unsigned int port) : sock("0.0.0.0", port) {}

		void add(gxx::hostaddr addr, int port) {
			table.emplace_back(addr, port);
		}

		void on_input(g0::message*) override;

		void read_handler();
		int get_fd() { return sock.fd; }
	};
}

#endif