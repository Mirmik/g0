#include <g0/service.h>
#include <g0/test_service.h>

#include <g0/gate/udp.h>

g0::test_service test0("test");

g0::udp_gate_address uaddr[] = {
	{4, "127.0.0.1", 11001},
	{0,"0.0.0.0",0},
};

g0::udp_gate ugate(11002, uaddr);

int main() {
	g0::registry_service(&test0, 1);
	g0::registry_service(&ugate, 2);

	g0::message* msg = new g0::message;
	g0::message_init(msg, "\002\000\004\000\001\000", 6, "helloworld", 10);

	g0::transport(msg);
}