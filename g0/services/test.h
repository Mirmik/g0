#ifndef G0_TEST_H
#define G0_TEST_H

#include <gxx/util/string.h>
#include <gxx/print.h>
#include <gxx/print/stdprint.h>
#include <g1/tower.h>

namespace g0 {

<<<<<<< HEAD
	struct test_service : public service {
		void incoming_packet(g1::packet* pack) override {
			auto sh = g0::get_subheader(pack);
			auto data = g0::get_datasect(pack);
			g1::println(pack);
			gxx::fprintln("subheader: sid={}, rid={}", sh->sid, sh->rid);
			gxx::fprintln("datasect: {}", gxx::dstring(data));
			g1::release(pack);
=======
	struct test_service : public basic_service {
		void incoming_message(g0::message* msg) override {
			gxx::fprintln("test service incoming_message from:{} {}", msg->rid, gxx::buffer(msg->data, msg->size));
>>>>>>> 08a606d12e8d590f6fb295ab9b8b036946e70521
		}
	};

	g0::test_service* create_test_service(int i);
}

#endif