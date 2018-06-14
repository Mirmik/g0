#include <g0/core.h>
#include <g1/tower.h>
#include <g1/indexes.h>
#include <g1/gates/udpgate.h>
#include <gxx/log/target2.h>

#include <thread>

g1::udpgate udpgate;

void udplisten();
void g1execute();
void incoming_handler(g1::packet* pack);

struct brocker_service : public g0::service {
	void incoming_message(g0::message* msg) override {
		gxx::fprintln("brocker incoming, data:{}", gxx::buffer(msg->data, msg->size));
		g0::utilize(msg);
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
	std::thread thrg1(g1execute);
	thr.join();
	thrg1.join();
}

void udplisten() {
	while(1) udpgate.exec_syncrecv();
}

void incoming_handler(g1::packet* pack) {
	gxx::println("incoming handler");
	if (pack->block->type == G1_G0TYPE) {
		gxx::println("g1g0");
		g0::travell(pack);
	} else {
		g1::release(pack);
	}
}

void g1execute() {
	while(1) g1::one_thread_execute();
}