#include <g0/network.h>
#include <gxx/print.h>

#include <string.h>

/*g0::id_t g0::node_id = g0::NAIVE_NODE;
void g0::set_node_id(id_t id) { node_id = id; }*/

/*void g0::naive_router::retranslate_package(char* data, size_t size) {

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
*/
static g0::gate** __gates;

void g0::set_gates(g0::gate** ptr) {
	__gates = ptr;
}
/*
bool g0::receive_package(g0::pkb pack) {
	switch (pack.pack->packtype) {
		case PACKTYPE_TEST: 
			gxx::println("test type. utilize pkb");
			free(pack.buffer);
			break;
		case PACKTYPE_MESSAGE:
			gxx::println("message type");
			free(pack.buffer);
			break;
	} 
}*/

g0::pkb::pkb(char* data, size_t size) {
	addrlen = * (uint8_t*) data;
	stage = (uint8_t*) (data + 1);
	raddr = data + 2;
	saddr = raddr + addrlen;
	data = saddr + addrlen; 
	datalen = size - 2 - (addrlen << 1);
}

void g0::form_package_header(char* pack, const char* raddr, uint8_t addrlen) {
	* (uint8_t*) pack = addrlen;
	* (uint8_t*) (pack + 1) = 0;
	memcpy(pack + 2, raddr, addrlen);
	memset(pack + 2 + addrlen, 0xFF, addrlen);
}

bool g0::receive_package(g0::pkb* pkb) {
	gxx::println("g0::retranslate_package");
	return true;
}

bool g0::retranslate_package(g0::pkb* pkb) {
	gxx::println("g0::retranslate_package");
	if (pkb->addrlen == pkb->stage) {
		g0::receive_package(pkb);
	} else {
		g0::gate** gt = __gates; 
		
		if (gt == nullptr) {
			gxx::println("warn: gate table isn't setted");
			return false;
		}

		bool ret;
		while (*gt != nullptr) {
			ret = gt->translate(pkb);
			if (ret) return true;
		}

		return false;
	}
}

/*bool g0::retranslate_package(g0::pkb pack, bool external) {
	GXX_PRINT(pack.pack->snid);
	GXX_PRINT(pack.pack->rnid);
	GXX_PRINT(g0::node_id);
	
	if (g0::node_id == pack.pack->rnid || (external && g0::node_id == NAIVE_NODE)) {
		gxx::fprintln("i am {} receiver", g0::node_id == NAIVE_NODE ? "naive" : "");
		return g0::receive_package(pack);
	} else {
		gxx::println("i am not receiver. need retrans.");
		//g0::route_pkb(pack); 
		return false;
	}
}

bool g0::translate_package(g0::pkb pack) {
	gxx::println("g0::translate_package");
}*/