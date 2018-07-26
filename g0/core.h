#ifndef G0_CORE_H
#define G0_CORE_H

#include <gxx/container/dlist.h>
#include <gxx/datastruct/iovec.h>
#include <g1/packet.h>

namespace g0 {
	struct service {
		dlist_head lnk;
		uint16_t id;
		virtual void incoming_packet(g1::packet* pack) = 0;
		service() { dlist_init(&lnk); }
	};

	struct subheader {
		uint16_t sid;
		uint16_t rid;		
	} G1_PACKED;

	static inline subheader* get_subheader(g1::packet* pack) {
		return (subheader*) pack->dataptr();
	}

	static inline gxx::buffer get_datasect(g1::packet* pack) {
		return gxx::buffer(pack->dataptr() + sizeof(subheader), pack->datasize() - sizeof(subheader));
	}	

	extern gxx::dlist<g0::service, &g0::service::lnk> services;

	/// Добавить сервис к ядру.
	void link_service(g0::service* srvs, uint16_t id);
	void incoming(g1::packet* pack);

	void __send(uint16_t sid, uint16_t rid, const uint8_t* raddr, size_t rlen, const char* data, size_t size, g1::QoS qos);
}

#endif