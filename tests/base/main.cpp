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
	g0::connection con;
	g0.open(test1, echo1);
	g0.close();
	

	//con.close();

	/*const char* msg = "HelloWorld";
	g0::send(test1.id, echo1.id, msg, strlen(msg));

	g0::test_gate t2;
	t2.target = &t1;

	char buf[256];

	int locid = 33;
	int remid = 35;

	t2.packsend_vchannel_control(locid, remid, g0::CTRLFUNC_CREATE);*/
}