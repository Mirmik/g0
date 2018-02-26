#ifndef G0_UDP_GATE_H
#define G0_UDP_GATE_H

#include <g0/service.h>
#include <gxx/inet/dgramm.h>

namespace g0 {

	struct udp_gate_address {
		gxx::hostaddr addr;
		uint16_t port;
		bool temporary;
		udp_gate_address(gxx::hostaddr& str, int port, bool temporary) : addr(str), port(port), temporary(temporary) {}
	};

	class udp_gate : public g0::service {
	private:
		gxx::inet::udp_socket sock;
		std::list<udp_gate_address> table;

	public:	
		udp_gate() {}
		udp_gate(unsigned int port) : sock("0.0.0.0", port) {}
		void init(unsigned int port) { sock.bind("0.0.0.0", port); }

		void add(gxx::hostaddr addr, int port) { table.emplace_back(addr, port, false); }
		void on_input(g0::message*) override;
		void read_handler();
		int get_fd() { return sock.fd; }

		void handshake_temporaries();
	};
}

#endif