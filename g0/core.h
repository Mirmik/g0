#ifndef G0_CORE_H
#define G0_CORE_H

#include <g0/service.h>
#include <gxx/log/logger2.h>
#include <gxx/container/dlist.h>

namespace g1 { struct packet; }
namespace g0 {

	extern gxx::log::logger logger;
	extern gxx::dlist<g0::service, &g0::service::lnk> services;

	struct service_address {
		uint16_t id;
		uint8_t qos = 0;
		std::string g1addr; 
	};

	///Для построения сообщения.
	struct iovec { const void* data; size_t size; };

	/// Добавить сервис к ядру.
	void link_service(g0::service* srvs, uint16_t id);

	/// Обработка пакета, поступившего в ядро через систему g1.
	void travell(g1::packet* pack);

	void transport(g0::message* msg);
	void utilize(g0::message* msg);

	void send(uint16_t sid, uint16_t rid, const char* raddr, size_t rlen, iovec* vec);
	void send(uint16_t sid, uint16_t rid, iovec* vec);
	void send(uint16_t sid, uint16_t rid, const char* data, size_t size);
	void send(uint16_t sid, g0::service_address& raddr, const char* data, size_t size);
}

#endif