#include <g0/test_service.h>
//#include <g0/test_gate.h>
#include <stdlib.h>
#include <stdio.h>

void g0::test_service::on_input(g0::message* msg) {
	printf("%s input message: len: %d txt: %*s\n", name, (int)msg->header->datalen, (int)msg->header->datalen, (char*)msg->data);
	g0::utilize(msg);
}

void g0::echo_service::on_input(g0::message* msg) {
	//gxx::fprintln("{}: {}", name, gxx::buffer(msg->data, msg->header->datalen));
	gxx::fprintln("{}: message received", name);
	gxx::println("\taddrlen:", msg->header->addrlen);
	gxx::println("\tdatalen:", msg->header->datalen);
	gxx::print("\tsender: "); gxx::printhex(msg->saddr, msg->header->addrlen);
	gxx::println();
	gxx::print("\tdata(hex): "); gxx::printhex(msg->data, msg->header->datalen);
	gxx::println();
	gxx::print("\tdata(ascii): "); gxx::write(msg->data, msg->header->datalen);
	gxx::println();

	memcpy(msg->raddr, msg->saddr, msg->header->addrlen);
	msg->header->stage = 0;

	g0::transport(msg, id);	
}