#include <g0/service.h>
#include <g0/test_service.h>

g0::test_service test0("test");

int main() {
	g0::registry_service(&test0, 1);

	g0::message* msg = new g0::message;
	g0::message_init(msg, "\001", 1, "helloworld", 10);

	g0::transport(msg);
}