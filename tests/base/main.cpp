#include <g0/service.h>
#include <g0/gate.h>
#include <g0/test_service.h>
#include <g0/test_gate.h>

#include <stdio.h>
#include <string.h>

g0::test_service test1("test1");
g0::echo_service echo1("echo1");

g0::test_gate t1; 

int main() {
	const char* msg = "HelloWorld";
	g0::send(test1.id, echo1.id, msg, strlen(msg));

	g0::test_gate t2;
	t2.target = &t1;

	char buf[256];

	buf[0] = 0;
	buf[1] = 0;
	buf[2] = 0;
	buf[3] = 0;
	buf[4] = 0;
	buf[5] = 0;
	buf[6] = 0;

	t2.send_package(buf, 7);
}