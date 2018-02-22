#include <g0/service.h> 
#include <g0/test_service.h>

#include <g0/gate/udp.h>

g0::test_service test0("test");
g0::udp_gate ugate(11002);

int main() {
	g0::registry_service(&test0, 1);
	g0::registry_service(&ugate, 2);

	ugate.add("127.0.0.1", 11001);

	g0::message* msg = new g0::message;
	g0::message_init(msg, "\x02\x00\x00\x00\x01\x00", 6, "helloworld", 10);

	g0::transport(msg, test0.id);
}