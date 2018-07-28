#ifndef G0_DGRAMM_SPAMER_H
#define G0_DGRAMM_SPAMER_H

#include <gxx/io/ostream.h>
#include <g0/service.h>
#include <gxx/util/hexer.h>

namespace g0 {
	struct dgramm_spammer : public g0::basic_service, public gxx::io::ostream {
		int rid;
		uint8_t* addr;
		size_t size;

		dgramm_spammer(int rid, uint8_t* addr, size_t size) : rid(rid), addr(addr), size(size) {}
		dgramm_spammer(int rid, const char* hexaddr, uint8_t* addr, size_t size) : rid(rid), addr(addr), size(size) {
			hexer(addr, size, hexaddr, strlen(hexaddr));
		}

		int writeData(const char* dat, size_t sz) {
			g0::send(addr, size, dat, sz, 0, g1::QoS(0));
		}

		void incoming_message(g0::message*) override {}
	};
}

#endif