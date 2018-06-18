#include <g0/core.h>
#include <g1/tower.h>
#include <g1/indexes.h>
#include <g1/gates/udpgate.h>
#include <gxx/log/target2.h>

#include <thread>

g1::udpgate udpgate;
gxx::log::colored_stdout_target console_target;

int main() {
	g1::logger.link(console_target, gxx::log::level::trace);
	g0::logger.link(console_target, gxx::log::level::trace);

	udpgate.open(10001);

	g1::link_gate(&udpgate, G1_UDPGATE);

	g0::service_address addr;
	addr.id = 0;
	g1::pushgate(addr.g1addr, G1_UDPGATE);
	g1::pushudp(addr.g1addr, "127.0.0.1", 10000);

	const char msgdata[] = "\000\010atom.log\012HelloWorld";

	g0::send(0, addr, msgdata, sizeof(msgdata));

	g1::one_thread_execute();
}