#include <g0/service.h>
#include <g0/test_service.h>

#include <stdio.h>
#include <string.h>

int main() {
	g0::test_service test1("test1");
	g0::echo_service echo1("echo1");

	const char* msg = "HelloWorld";
	g0::send(test1.id, echo1.id, msg, strlen(msg));
}