#ifndef G0_TEST_H
#define G0_TEST_H

#include <gxx/util/string.h>
#include <gxx/print.h>
#include <gxx/print/stdprint.h>
#include <g1/tower.h>

namespace g0 {

	struct test_service : public service {
		void incoming_packet(g1::packet* pack) override {
			auto sh = g0::get_subheader(pack);
			auto data = g0::get_datasect(pack);
			g1::println(pack);
			gxx::fprintln("subheader: sid={}, rid={}", sh->sid, sh->rid);
			gxx::fprintln("datasect: {}", gxx::dstring(data));
			g1::release(pack);
		}
	};

	g0::test_service* create_test_service(int i);
}

#endif