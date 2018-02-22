#include <g0/service.h>
#include <stdlib.h>
#include <string.h>

static g0::id_t service_id_counter = 0;
//static g0::id_t message_id_counter = 0;

#ifndef G0_SERVICE_TABLE_SIZE
#define G0_SERVICE_TABLE_SIZE 20
#endif

struct hlist_head service_htable [G0_SERVICE_TABLE_SIZE];

static inline g0::id_t service_get_new_id() {
	return ++service_id_counter; 	
}

/*void g0::service_table_init() {
	for (hlist_head* it = service_htable; it != service_htable + G0_SERVICE_TABLE_SIZE; ++it) {
		hlist_init_head(it);
	}
}*/

/*static inline g0::id_t message_get_new_id() {
	return ++message_id_counter; 	
}

g0::message::message() {
	//stsbyte = 0;
	//qid = message_get_new_id();
}
*/
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
/*
void g0::send(id_t sender, id_t receiver, const char* data, size_t size) {
	message* msg = new message;
	
	void* msgdata = malloc(size);
	memcpy(msgdata, data, size);

	msg->sid = sender;
	msg->rid = receiver;
	msg->data = msgdata;
	msg->size = size;

	uint8_t sts = g0::transport(msg);
	//return msg->qid;
}
*/
//g0::id_t g0::message_read_next_id(g0::message* msg) {
//	return rid;
//}

void g0::transport(g0::message* msg, g0::id_t sid) {
	gxx::println("g0::transport");
	gxx::print("to: ");
	gxx::printhex(msg->raddr, msg->addrlen);
	gxx::println();
	g0::id_t rid = *(g0::id_t*)(msg->raddr + *msg->stage);
	*msg->stage += sizeof(g0::id_t);
	*(g0::id_t*)(msg->saddr + msg->addrlen - *msg->stage) = sid; 
	//gxx::println("rid:", rid);

	service* srvs = g0::service::find(rid);
	gxx::println(srvs);
	if (srvs == nullptr) {
		gxx::println("warn: g0::transport. wrong_adress. utilize.");
		g0::utilize(msg);
		return;
	}
	
	srvs->on_input(msg);
	//return 0;
}
/*
uint8_t g0::transport_reply(message* msg) {
	/*if (msg->noreply) {
		g0::utilize(msg);
		return 0;
	}*/
	
/*	auto tmp = msg->sid;
	msg->sid = msg->rid;
	msg->rid = tmp;	
	//msg->repled = true;

	return g0::transport(msg);
}*/

void g0::utilize(message* msg) {
	free(msg->buffer);
	delete msg;
}

void g0::message_init(g0::message* pkb, const char* raddr, uint8_t rlen, const char* data, size_t dlen) {
//	dlist_init_list(&pkb->lnk);
	pkb->datalen = dlen;
	pkb->addrlen = rlen;
	pkb->flen = 2 + rlen * 2 + dlen;
	pkb->buffer = (char*) malloc(pkb->flen);
	pkb->raddr = pkb->buffer + 2;
	pkb->saddr = pkb->buffer + 2 + rlen; 
	pkb->data =  pkb->buffer + 2 + rlen * 2;
	pkb->stage = (uint8_t*) (pkb->buffer + 1);
	* pkb->stage = 0;
	* pkb->buffer = rlen;
	memcpy(pkb->raddr, raddr, rlen);
	memcpy(pkb->data,  data, dlen);
}

void g0::message_parse(g0::message* pkb, char* data, size_t size) {
	pkb->addrlen = * (uint8_t*) data;
	pkb->buffer = data;
	pkb->stage = (uint8_t*) (pkb->buffer + 1);
	pkb->raddr = pkb->buffer + 2;
	pkb->saddr = pkb->buffer + 2 + pkb->addrlen; 
	pkb->data =  pkb->buffer + 2 + pkb->addrlen * 2;
	pkb->flen = size;
	pkb->datalen = size - pkb->addrlen * 2 - 2;
}
