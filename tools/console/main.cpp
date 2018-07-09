#include <gxx/print.h>
#include <g0/core.h>
#include <g1/indexes.h>
#include <g1/tower.h>
#include <g1/gates/udpgate.h>

void receive(g0::message* msg) {
	gxx::writeln(msg->data, msg->size);
	g0::answer(msg, "Answer", 6);
	g0::utilize(msg);
}

void incoming_handler(g1::packet* pack) {
	if (pack->header.type == G1_G0TYPE) {
		g0::travell(pack);
	} else {
		g1::release(pack);
	}
}

int main() {
	g1::incoming_handler = incoming_handler;

	g1::make_udpgate(10005);
	g0::make_service(28, receive);

	g1::spin();
}

