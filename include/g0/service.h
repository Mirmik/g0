#ifndef G0_SERVICE_H
#define G0_SERVICE_H

#include <gxx/print.h>
#include <gxx/datastruct/dlist_head.h>
#include <gxx/datastruct/hlist_head.h>

namespace g0 {

	using id_t = uint16_t;

	struct message {
		dlist_head lnk;

		char* buffer;

		uint8_t addrlen;
		uint8_t* stage;

		char* raddr;
		char* saddr;

		char* data;		
		size_t datalen;
		size_t flen;

		size_t printTo(gxx::io::ostream& o) const {
			gxx::fprint_to(o, "(addrlen:{},stage:{},datalen:{})", addrlen, stage, datalen);
		}
	};

	void service_table_init();
	void message_init(g0::message* pkb, const char* raddr, uint8_t rlen, const char* data, size_t dlen);
	void message_parse(g0::message* pkb, char* data, size_t size);
	void transport(g0::message* pkb, id_t sid);

	class service {
	public:
		struct hlist_node hlnk;		//к таблице сервисов.
		id_t id;					//ид. хэш таблицы сервисов.
		
		virtual void on_input(message*) = 0;
		static service* find(id_t);
	};

	void registry_service(g0::service* srvs);
	void registry_service(g0::service* srvs, id_t id);

	void utilize(message* msg);
}

#endif