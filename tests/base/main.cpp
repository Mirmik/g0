#include <g0/core.h>
#include <g0/services/test.h>
#include <g0/services/action.h>

#include <g1/tower.h>
#include <g1/indexes.h>
#include <g1/gates/udpgate.h>

void incoming(g1::packet* pack) {
	gxx::println("incoming");

	switch(pack->header.type) {
		case G1_G0TYPE:
			g0::incoming(pack);
			break;

		default:
			g1::release(pack);
	}
}

void hello(g1::packet* pack) {
	gxx::println("HelloWorld!!!!!");
}

int main() {
	g0::create_test_service(10);
	g0::create_action_service(11, hello);
	
	g1::incoming_handler = incoming;
	g0::__send(0,11,nullptr,0,"HelloWorld",10,g1::QoS(0));
	
	g1::spin();
}