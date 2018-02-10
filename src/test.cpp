#include <g0/test_service.h>
#include <g0/test_gate.h>
#include <stdlib.h>
#include <stdio.h>

void g0::test_service::on_input(g0::message* msg) {
	printf("%s input message: \n\tqid: %d\n\tsid: %d\n\trid: %d\n\ttxt: %*s\n\trepl: %d\n", name, msg->qid, msg->sid, msg->rid, (int)msg->size, (char*)msg->data, (int)msg->repled);
	g0::utilize(msg);
}

void g0::echo_service::on_input(g0::message* msg) {
	printf("%s replied message: \n\tqid: %d\n\tsid: %d\n\trid: %d\n\ttxt: %*s\n\trepl: %d\n", name, msg->qid, msg->sid, msg->rid, (int)msg->size, (char*)msg->data, (int)msg->repled);
	g0::transport_reply(msg);
}

void g0::test_gate::send_package(const char* data, size_t sz) {
	printf("send_package\r\n");
	target->on_recv_package(data, sz);
}