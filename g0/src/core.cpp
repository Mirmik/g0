#include <g0/core.h>
#include <g1/packet.h>
#include <g1/tower.h>
#include <g1/indexes.h>
#include <gxx/print.h>
#include <gxx/syslock.h>

gxx::log::logger g0::logger("g0");
gxx::dlist<g0::basic_service, &g0::basic_service::lnk> g0::services;

g0::message* g0::create_message() {
	gxx::system_lock();
	auto ret = (g0::message*) malloc(sizeof(g0::message));
	gxx::system_unlock();
	return ret;
}

void g0::send(uint16_t sid, uint16_t rid, const char* data, size_t size) {
	g0::message* msg = g0::create_message();
	dlist_init(&msg->lnk);
	msg -> sid = sid;
	msg -> rid = rid;
	msg -> pack = nullptr;
	msg -> data = (char*) malloc(size);
	memcpy(msg->data, data, size);
	msg -> size = size;

	g0::transport(msg);
}

void g0::send(uint16_t sid, uint16_t rid, gxx::iovec* beg, gxx::iovec* end) {
	size_t sz = 0;
	for (auto it = beg; it != end; ++it) sz += it->size;

	g0::message* msg = g0::create_message();
	

	dlist_init(&msg->lnk);
	msg -> sid = sid;
	msg -> rid = rid;
	msg -> pack = nullptr;
	gxx::system_lock();
	msg -> data = (char*) malloc(sz);
	gxx::system_unlock();

	char* dataiter = msg->data;
	for (auto it = beg; it != end; ++it) 
		dataiter = (char*)memcpy(dataiter, it->data, it->size) + it->size;
	
	msg -> size = sz;

	g0::transport(msg);
}

void g0::send(uint16_t sid, const g0::service_address& raddr, const char* data, size_t size, g1::QoS qos) {
	if (raddr.raddr.size() == 0) return g0::send(sid, raddr.id, data, size);
	auto pack = g1::create_packet(nullptr, raddr.raddr.size(), size + 2);
	*pack->dataptr() = sid;
	*(pack->dataptr() + 1) = raddr.id;
	pack->header.qos = qos;
	pack->header.type = G1_G0TYPE;
	memcpy(pack->dataptr() + 2, data, size);
	memcpy(pack->addrptr(), raddr.raddr.data(), raddr.raddr.size());
	g1::transport(pack);
}

void g0::send(uint16_t sid, uint16_t rid, const uint8_t* addr, size_t asize, const char* data, size_t dsize, g1::QoS qos, uint16_t ackquant) {
	auto pack = g1::create_packet(nullptr, asize, dsize + 2);
	*pack->dataptr() = sid;
	*(pack->dataptr() + 1) = rid;
	pack->header.qos = qos;
	pack->header.type = G1_G0TYPE;
	pack->header.ackquant = ackquant;
	memcpy(pack->dataptr() + 2, data, dsize);
	memcpy(pack->addrptr(), addr, asize);
	g1::transport(pack);
}

void g0::send(uint16_t sid, const g0::service_address& raddr, gxx::iovec* beg, gxx::iovec* end, g1::QoS qos) {
	if (raddr.raddr.size() == 0) return g0::send(sid, raddr.id, beg, end);
	
	size_t sz = 0;
	for (auto it = beg; it != end; ++it) sz += it->size;

	auto pack = g1::create_packet(nullptr, raddr.raddr.size(), sz + 2);
	*pack->dataptr() = sid;
	*(pack->dataptr() + 1) = raddr.id;
	pack->header.qos = qos;
	pack->header.type = G1_G0TYPE;
	
	char* dataiter = pack->dataptr() + 2;
	for (auto it = beg; it != end; ++it) 
		dataiter = (char*)memcpy(dataiter, it->data, it->size) + it->size;
	gxx::print_dump(pack->dataptr(), sz + 2);

	memcpy(pack->addrptr(), raddr.raddr.data(), raddr.raddr.size());
	g1::transport(pack);
}

void g0::travell(g1::packet* pack) {
	gxx::system_lock();
	g0::message* msg = g0::create_message();
	gxx::system_unlock();
	dlist_init(&msg->lnk);
	msg -> pack = pack;
	msg -> sid = pack->dataptr()[0];
	msg -> rid = pack->dataptr()[1];
	msg -> data = pack->dataptr() + 2;
	msg -> size = pack->datasize() - 2;

	g0::transport(msg);
}

void g0::utilize(g0::message* msg) {
	gxx::system_lock();
	dlist_del(&msg->lnk);
	if (!msg->pack) free(msg->data); 
	else g1::release(msg->pack);
	free(msg);	
	gxx::system_unlock();
}

void g0::transport(g0::message* msg) {
	/*if (msg->pack != nullptr) {
		msg->pack->header.stg = 0;
		msg->pack->dataptr()[0] = msg->sid;
		msg->pack->dataptr()[1] = msg->rid;
		g1::travel(msg->pack);
		msg->pack = nullptr;
		g0::utilize(msg);
	}*/

	for ( auto& srvs: g0::services ) {
		if (srvs.id == msg->rid) {
			//g0::logger.debug("to {} rid", srvs.id);
			srvs.incoming_message(msg);
			return;
		}
	}
	//g0::logger.debug("unresolved basic_service. utilize message");
	g0::utilize(msg);
}

/*void g0::retrans(g0::message* msg) {
	if (msg->pack != nullptr) {
		msg->pack->dataptr()[0] = msg->sid;
		msg->pack->dataptr()[1] = msg->rid;
		g1::transport(msg->pack);
		msg->pack = nullptr;
		g0::utilize(msg);
	}
	else {
		transport(msg);
	}
}*/

void g0::link_service(g0::basic_service* srvs, uint16_t id) {
	srvs->id = id;
	g0::services.move_back(*srvs);
}


g0::service* g0::make_service(int idx, gxx::delegate<void, g0::message*> dlg) {
	g0::service* srvs = new g0::service(dlg);
	g0::link_service(srvs, idx);
}

void g0::answer(g0::message* msg, const char* data, size_t sz) {
	if (msg->pack == nullptr) g0::send(msg->rid, msg->sid, data, sz);
	else g0::send(msg->rid, msg->sid, msg->pack->addrptr(), msg->pack->header.alen, data, sz, g1::QoS(2));		
}