#ifndef G0_UDP_GATE_H
#define G0_UDP_GATE_H

#include <g0/network.h>
#include <gxx/inet/dgramm.h>

namespace g0 {

	struct udp_gate_address {
		id_t node;
		gxx::hostaddr addr;
		uint16_t port;
	};

	class udp_gate : public g0::gate {
	public:
		gxx::inet::udp_socket sock;

		udp_gate_address* table;
		udp_gate(unsigned int port, udp_gate_address* table) : table(table), sock("0.0.0.0", port) {}

		bool send(id_t node, const char* data, size_t size) override;
		bool send(id_t node, const char* data1, size_t size1, const char* data2, size_t size2) override;
	};

}

#endif