#include <g0/core.h>
#include <g1/tower.h>
#include <g1/indexes.h>
#include <g1/gates/udpgate.h>
#include <gxx/log/target2.h>

#include <thread>

g1::udpgate udpgate;

void udplisten();
void incoming_handler(g1::packet* pack);

struct brocker_service : public g0::service {
	void incoming_message(g0::message* msg) override {
		gxx::println("brocker incoming");
	}
} brocker;

gxx::log::colored_stdout_target console_target;

int main() {
	g1::logger.link(console_target, gxx::log::level::trace);
	g0::logger.link(console_target, gxx::log::level::trace);

	udpgate.open(10000);

	g1::link_gate(&udpgate, G1_UDPGATE);
	g0::link_service(&brocker, 25);

	g1::incoming_handler = incoming_handler;

	std::thread thr(udplisten);
	thr.join();
}

void udplisten() {
	while(1) udpgate.exec_syncrecv();
}

void incoming_handler(g1::packet* pack) {
	gxx::println("incoming handler");
	if (pack->block->type == G1_G0TYPE) {
		gxx::println("g1g0");
	} else {
		g1::release(pack);
	}
}