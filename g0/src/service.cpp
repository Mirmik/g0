#include <g0/service.h>
#include <g0/kernel.h>
#include <stdlib.h>
#include <string.h>

static g0::id_t service_id_counter = 0;

#ifndef G0_SERVICE_TABLE_SIZE
#define G0_SERVICE_TABLE_SIZE 20
#endif

struct hlist_head service_htable [G0_SERVICE_TABLE_SIZE];

static inline g0::id_t service_get_new_id() {
	return ++service_id_counter; 	
}

void g0::registry_service(g0::service* srvs) {
	srvs->id = service_get_new_id();
	hlist_add_next(&srvs->hlnk, &service_htable[srvs->id % G0_SERVICE_TABLE_SIZE].first);
}

void g0::registry_service(g0::service* srvs, g0::id_t id) {
	srvs->id = id;
	hlist_add_next(&srvs->hlnk, &service_htable[srvs->id % G0_SERVICE_TABLE_SIZE].first);
}

g0::service* g0::service::find(g0::id_t id) {
	struct hlist_node* it;
	struct g0::service* entry;
	size_t cell = id % G0_SERVICE_TABLE_SIZE;
	hlist_for_each(it, &service_htable[cell]) {
		entry = hlist_entry(it, g0::service, hlnk);
		if (id == entry->id) return entry;
	}
	return nullptr;
}

void g0::transport(g0::message* msg, g0::id_t sid) {
	gxx::println("g0::transport, sid:", sid);
	g0::id_t rid= g0::read_stage(msg);

	service* srvs = g0::service::find(rid);
	if (srvs == nullptr) {
		if (msg->header->buserr_notify) {
			g0::buserror(msg, sid);
			return;
		}
		g0::utilize(msg);
		return;
	}
	
	g0::mark_stage(msg, sid);
	srvs->on_input(msg);
}

g0::id_t g0::read_stage(g0::message* msg) {
	return *(g0::id_t*)(msg->raddr + msg->header->stage);
}

void g0::mark_stage(g0::message* msg, g0::id_t id) {
	msg->header->stage += sizeof(g0::id_t);
	*(g0::id_t*)(msg->saddr + msg->header->addrlen - msg->header->stage) = id;

	//gxx::println(msg->header->stage);
	//gxx::printhex(msg->saddr, msg->header->addrlen);
}

void g0::utilize(message* msg) {
	free(msg->buffer);
	delete msg;
}

void g0::message_init(g0::message* pkb, const g0::id_t* raddr, uint8_t rlen, const char* data, size_t dlen) {
	pkb->flen = sizeof(message_header) + rlen * 2 + dlen;
	pkb->buffer = (char*) malloc(pkb->flen);

	pkb->header->datalen = dlen;
	pkb->header->addrlen = rlen;
	pkb->header->stage = 0;
	
	pkb->raddr = (char*)(pkb->header + 1);
	pkb->saddr = pkb->raddr + rlen; 
	pkb->data =  pkb->saddr + rlen;

	pkb->header->buserr_notify = true;
	
	memcpy(pkb->raddr, raddr, rlen);
	memcpy(pkb->data,  data, dlen);
}

//void g0::message_init(g0::message* pkb, std::initializer_list<g0::id_t> rvec, uint8_t rlen, const char* data, size_t dlen) {
//	id_t buf[rvec.size() * sizeof(g0::id_t)];
//	std::copy(rvec.begin(), rvec.end(), buf);
//}

void g0::message_parse(g0::message* pkb, char* data, size_t size) {
	pkb->buffer = data;
	pkb->raddr = (char*)(pkb->header + 1);
	pkb->saddr = pkb->raddr + pkb->header->addrlen; 
	pkb->data =  pkb->saddr + pkb->header->addrlen;
	pkb->flen = size;
}

void g0::buserror(g0::message* msg, g0::id_t sid) {
	gxx::println("warn: buserror. utilize message");
	g0::utilize(msg);

/*	gxx::println("g0::buserror, sid:", sid);
	if (msg->header->buserr_notify == false) {
		gxx::println("buserror_notify prevent. utilize message.");	
		g0::utilize(msg);
		return;
	} 

	msg->header->type = G0_ERROR_BUSERR;
	msg->header->errarg8_0 = msg->header->stage;
	msg->header->stage = msg->header->addrlen - msg->header->stage;
	msg->header->buserr_notify = false;

	g0::transport(msg, sid);*/
}