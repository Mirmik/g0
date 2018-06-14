#include <g0/core.h>
#include <g1/packet.h>
#include <g1/tower.h>
#include <g1/indexes.h>
#include <gxx/print.h>

gxx::log::logger g0::logger("g0");
gxx::dlist<g0::service, &g0::service::lnk> g0::services;

void g0::send(uint16_t sid, uint16_t rid, const char* data, size_t size) {
	g0::logger.debug("local message constructor");

	g0::message* msg = (g0::message*) malloc(sizeof(g0::message));
	dlist_init(&msg->lnk);
	msg -> sid = sid;
	msg -> rid = rid;
	msg -> pack = nullptr;
	msg -> data = (char*) malloc(size);
	memcpy(msg->data, data, size);
	msg -> size = size;

	g0::transport(msg);
}

void g0::send(uint16_t sid, g0::service_address& raddr, const char* data, size_t size) {
	if (raddr.g1addr.size() == 0) return g0::send(sid, raddr.id, data, size);
	auto block = g1::create_block(raddr.g1addr.size(), size + 2);
	auto pack = g1::create_packet(nullptr, block);
	*pack->dataptr() = sid;
	*(pack->dataptr() + 1) = raddr.id;
	pack->block->qos = (g1::QoS)raddr.qos;
	pack->block->type = G1_G0TYPE;
	memcpy(pack->dataptr() + 2, data, size);
	memcpy(pack->addrptr(), raddr.g1addr.data(), raddr.g1addr.size());
	g1::transport(pack);
}

void g0::travell(g1::packet* pack) {
	g0::message* msg = (g0::message*) malloc(sizeof(g0::message));
	dlist_init(&msg->lnk);
	msg -> pack = pack;
	msg -> sid = pack->datasect()[0];
	msg -> rid = pack->datasect()[1];
	msg -> data = pack->dataptr() + 2;
	msg -> size = pack->datasize() - 2;
	gxx::println(msg->sid, msg->rid, msg->size);
	g0::transport(msg);
}

void g0::utilize(g0::message* msg) {
	dlist_del(&msg->lnk);
	if (!msg->pack) free(msg->data); 
	else g1::release(msg->pack);
	free(msg);	
}

void g0::transport(g0::message* msg) {
	//if (msg->pack == nullptr) {
	//	g0::logger.debug("transport local message sid:{}, rid:{}, data:{}", msg->sid, msg->rid, msg->datasect());
		for ( auto& srvs: g0::services ) {
			if (srvs.id == msg->rid) {
				g0::logger.debug("to {} rid", srvs.id);
				srvs.incoming_message(msg);
				return;
			}
		}
		g0::logger.debug("unresolved service. utilize message");
		g0::utilize(msg);
	//}
}

void g0::link_service(g0::service* srvs, uint16_t id) {
	srvs->id = id;
	g0::services.move_back(*srvs);
}