#include <g0/network.h>

g0::id_t g0::node_id = g0::NAIVE_NODE;

void g0::set_node_id(id_t id) { node_id = id; }

void g0::naive_router::retranslate_package(char* data, size_t size) {

}

void g0::naive_router::export_message(message* msg) {
	gt->send_message(msg);
}

void g0::gate::send_message(message* msg) {
	size_t packsize = sizeof(msgpack_header) + msg->size;
	char buf[packsize];
	
	msgpack_header* header = (msgpack_header*) buf;

	header.snid = node_id;
	header.rnid = msg->rnid;
	header.sid  = msg->sid;
	header.rid  = msg->rid;

	memcpy((void*)(header + 1), msg.data, msg.size);
	send(buf, packsize);

	g0::utilize(msg);
}