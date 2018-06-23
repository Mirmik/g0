#ifndef G0_CORE_H
#define G0_CORE_H

#include <g0/service.h>
#include <gxx/log/logger2.h>
#include <gxx/container/dlist.h>
#include <gxx/datastruct/iovec.h>
#include <g1/packet.h>

namespace g1 { struct packet; }
namespace g0 {

	extern gxx::log::logger logger;
	extern gxx::dlist<g0::service, &g0::service::lnk> services;

	struct service_address {
		uint16_t id;
		gxx::buffer raddr;
	
		bool operator!=(const g0::service_address& oth) const { return id != oth.id || raddr != oth.raddr; }
		bool operator==(const g0::service_address& oth) const { return id == oth.id && raddr == oth.raddr; }
	};

	///Для построения сообщения.
	struct iovec { const void* data; size_t size; };

	/// Добавить сервис к ядру.
	void link_service(g0::service* srvs, uint16_t id);

	/// Обработка пакета, поступившего в ядро через систему g1.
	void travell(g1::packet* pack);

	void transport(g0::message* msg);
	//void retrans(g0::message* msg);
	void utilize(g0::message* msg);
	g0::message* create_message();

	void send(uint16_t sid, uint16_t rid, const char* raddr, size_t rlen, iovec* vec);
	void send(uint16_t sid, uint16_t rid, gxx::iovec* vec, gxx::iovec* evec);
	void send(uint16_t sid, uint16_t rid, const char* data, size_t size);
	void send(uint16_t sid, uint16_t rid, const char* addr, size_t asize, const char* data, size_t dsize, g1::QoS qos);
	void send(uint16_t sid, const g0::service_address& raddr, const char* data, size_t size, g1::QoS qos = (g1::QoS)0);
	void send(uint16_t sid, const g0::service_address& raddr, gxx::iovec* vec, gxx::iovec* evec, g1::QoS qos = (g1::QoS)0);

	/*static inline service_address remoteaddr(const g0::message* msg) {
		service_address addr;
		addr.id = msg->rid;
		addr.g1addr = std::string(msg->pack->addrptr(), msg->pack->block->alen);
		return addr;
	}*/
}

/*namespace std {
	template <> struct hash<g0::service_address> {
		std::size_t operator()(const g0::service_address& k) const {
			return (std::hash<uint16_t>()(k.id) << 1) ^ std::hash<std::string>()(k.g1addr);
		}
	};
}*/

#endif