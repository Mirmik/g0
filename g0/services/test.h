#ifndef G0_TEST_H
#define G0_TEST_H

namespace g0 {

	struct test_service : public basic_service {
		void incoming_message(g0::message* msg) override {
			gxx::fprintln("test service incoming_message from:{} {}", msg->rid, gxx::buffer(msg->data, msg->size));
		}
	};

}

#endif