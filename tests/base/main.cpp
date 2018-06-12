#include <g0/service.h>
#include <g0/message.h>
#include <g0/core.h>
#include <g0/services/echo.h>
#include <g0/services/test.h>

#include <g1/tower.h>
#include <g1/indexes.h>
#include <g1/gates/udpgate.h>

#include <gxx/log/target2.h>

gxx::log::colored_stdout_target console_target;

g0::echo_service echosrvs;
g0::test_service testsrvs;

g1::udpgate udpgate;

int main() {
	g1::link_gate(&udpgate, G1_UDPGATE);
	g0::link_service(&echosrvs, 1);
	g0::link_service(&testsrvs, 0);

	g0::logger.link(console_target, gxx::log::level::trace);
	g1::logger.link(console_target, gxx::log::level::trace);

	g0::service_address addr;
	addr.id = 1;
	addr.qos = 0;
	g1::pushgate(addr.g1addr, G1_UDPGATE);
	g1::pushudp(addr.g1addr, "127.0.0.1", 10003);
	//addr.g1addr.pushudp("127.0.0.1", 10003);

	g0::send(0, 1, "HelloWorld", 10);
	g0::send(0, addr, "HelloWorld", 10);
}