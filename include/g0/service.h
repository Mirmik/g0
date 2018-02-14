#ifndef G0_SERVICE_H
#define G0_SERVICE_H

#include <gxx/datastruct/dlist_head.h>
#include <gxx/datastruct/hlist_head.h>

namespace g0 {
	using id_t = uint16_t;

	class message {
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

	class service {
	public:
		service();
		struct hlist_node hlnk;		//к таблице сервисов.
		id_t id;					//ид. хэш таблицы сервисов.
		
		virtual void on_input(message*) = 0;
		static service* find(id_t);
	};

	struct iovec {
		void* data;
		size_t size;
	};

	void send(id_t sender, id_t receiver, const char* data, size_t size);
	void send(id_t sender, id_t receiver, const iovec* vec);

	uint8_t transport(message* msg);
	uint8_t transport_reply(message* msg);

	void utilize(message* msg); 
}

#endif