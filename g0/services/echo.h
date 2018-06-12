#ifndef G0_ECHO_H
#define G0_ECHO_H

namespace g0 {

	struct echo_service : public service {
		void incoming_message(g0::message* msg) override {
			gxx::fprintln("echo service incoming_message {0}", gxx::buffer(msg->data, msg->size));
			std::swap(msg->rid, msg->sid);
			g0::transport(msg);
		}
	};

}

#endif