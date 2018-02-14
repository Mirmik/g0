#include <g0/service.h>
#include <stdlib.h>
#include <string.h>

static g0::id_t service_id_counter = 0;
static g0::id_t message_id_counter = 0;

#ifndef G0_SERVICE_TABLE_SIZE
#define G0_SERVICE_TABLE_SIZE 20
#endif

struct hlist_head service_htable [G0_SERVICE_TABLE_SIZE];

static inline g0::id_t service_get_new_id() {
	return ++service_id_counter; 	
}

static inline g0::id_t message_get_new_id() {
	return ++message_id_counter; 	
}

g0::message::message() {
	//stsbyte = 0;
	//qid = message_get_new_id();
}

g0::service::service() {
	id = service_get_new_id();
	hlist_add_next(&hlnk, &service_htable[id % G0_SERVICE_TABLE_SIZE].first);
}

g0::service* g0::service::find(id_t id) {
	struct hlist_node* it;
	struct g0::service* entry;
	size_t cell = id % G0_SERVICE_TABLE_SIZE;
	hlist_for_each(it, &service_htable[cell]) {
		entry = hlist_entry(it, g0::service, hlnk);
		if (id == entry->id) return entry;
	}
	return nullptr;
}

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

uint8_t g0::transport(message* msg) {
	service* srvs = g0::service::find(msg->rid);
	if (srvs == nullptr) {
		g0::utilize(msg);
		return -1;
	}
	srvs->on_input(msg);
	return 0;
}

uint8_t g0::transport_reply(message* msg) {
	/*if (msg->noreply) {
		g0::utilize(msg);
		return 0;
	}*/
	
	auto tmp = msg->sid;
	msg->sid = msg->rid;
	msg->rid = tmp;	
	//msg->repled = true;

	return g0::transport(msg);
}

void g0::utilize(message* msg) {
	free(msg->data);
	delete msg;
} 