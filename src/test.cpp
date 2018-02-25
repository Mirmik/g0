#include <g0/test_service.h>
//#include <g0/test_gate.h>
#include <stdlib.h>
#include <stdio.h>

void g0::test_service::on_input(g0::message* msg) {
	printf("%s input message: len: %d txt: %*s\n", name, (int)msg->datalen, (int)msg->datalen, (char*)msg->data);
	g0::utilize(msg);
}

void g0::echo_service::on_input(g0::message* msg) {
	gxx::fprintln("{}: {}", name, gxx::buffer(msg->data, msg->datalen));
		
	memcpy(msg->raddr, msg->saddr, msg->addrlen);
	*msg->stage = 0;

	g0::transport(msg, id);	
}