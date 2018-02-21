#ifndef G0_SERVICE_H
#define G0_SERVICE_H

#include <gxx/print.h>
#include <gxx/datastruct/dlist_head.h>
#include <gxx/datastruct/hlist_head.h>

namespace g0 {

	struct message {
		dlist_head lnk;

		char* buffer;

		uint8_t addrlen;
		uint8_t* stage;

		char* raddr;
		char* saddr;

		char* data;		
		size_t datalen;

		size_t printTo(gxx::io::ostream& o) const {
			gxx::fprint_to(o, "(addrlen:{},stage:{},datalen:{})", addrlen, stage, datalen);
		}
	};

	void message_init(g0::message* pkb, const char* raddr, uint8_t rlen, const char* data, size_t dlen);
	void transport(g0::message* pkb);

	using id_t = uint16_t;
	id_t message_read_next_id(g0::message* msg);

	
/*	class message {
	public:
		message();
		struct dlist_head qlnk; //к листу входящих сервиса.
		
		id_t sid; 			//ид отправителя.
		id_t snid; 			//ид отправителя.
		
		id_t rid;			//ид получателя.
		id_t rnid; 			//ид получателя.
		
		void* data;
		size_t size;
	};
*/
	class service {
	public:
		struct hlist_node hlnk;		//к таблице сервисов.
		id_t id;					//ид. хэш таблицы сервисов.
		
		virtual void on_input(message*) = 0;
		static service* find(id_t);
	};

	void registry_service(g0::service* srvs);
	void registry_service(g0::service* srvs, id_t id);

/*	struct iovec {
		void* data;
		size_t size;
	};

	void send(id_t sender, id_t receiver, const char* data, size_t size);
	void send(id_t sender, id_t receiver, const iovec* vec);

	uint8_t transport(message* msg);
	uint8_t transport_reply(message* msg);
*/
	void utilize(message* msg);
}

#endif