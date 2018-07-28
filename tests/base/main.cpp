#include <g0/core.h>
#include <g0/services/test.h>
#include <g0/services/action.h>

#include <g1/tower.h>
#include <g1/address.h>
#include <g1/indexes.h>
#include <g1/gates/udpgate.h>

<<<<<<< HEAD
void incoming(g1::packet* pack) {
	gxx::println("incoming");

	switch(pack->header.type) {
		case G1_G0TYPE:
			g0::incoming(pack);
			break;
=======
//#include <gxx/log/target2.h>

//gxx::log::colored_stdout_target console_target;
>>>>>>> 08a606d12e8d590f6fb295ab9b8b036946e70521

		default:
			g1::release(pack);
	}
}

void hello(g1::packet* pack) {
	gxx::println("HelloWorld!!!!!");
}

int main() {
<<<<<<< HEAD
	g0::create_test_service(10);
	g0::create_action_service(11, hello);
	
	g1::incoming_handler = incoming;
	g0::__send(0,11,nullptr,0,"HelloWorld",10,g1::QoS(0));
	
	g1::spin();
=======
	g1::link_gate(&udpgate, G1_UDPGATE);
	g0::link_service(&echosrvs, 1);
	g0::link_service(&testsrvs, 0);

	//g0::logger.link(console_target, gxx::log::level::trace);
	//g1::logger.link(console_target, gxx::log::level::trace);

	g0::service_address addr;
	addr.id = 1;
	
	g1::address raddr;
	raddr.pushudp(G1_UDPGATE, "127.0.0.1", 10004);

	addr.raddr = raddr;

	g0::send(0, 1, "HelloWorld", 10);
	g0::send(0, addr, "HelloWorld", 10);
>>>>>>> 08a606d12e8d590f6fb295ab9b8b036946e70521
}