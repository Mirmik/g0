#ifndef G0_SERVICE_H
#define G0_SERVICE_H

#include <gxx/print.h>
#include <gxx/datastruct/dlist_head.h>
#include <gxx/datastruct/hlist_head.h>

namespace g0 {

	using id_t = uint16_t;

	constexpr uint8_t G0_MESSAGE = 0x00;
	//constexpr uint8_t G0_ERROR = 0x01;
	constexpr uint8_t G0_HANDSHAKE = 0x02;

	constexpr uint8_t G0_POLICY_NORELIABLE = 0x00;
	constexpr uint8_t G0_POLICY_RELIABLE = 0x01;
	constexpr uint8_t G0_POLICY_RELIABLE_ONCE = 0x02;

	constexpr uint8_t G0_ERROR_BUSERR = 0x20;

	struct message_header {
		uint8_t type;
		uint8_t addrlen;
		uint8_t stage;

		union {
			uint16_t datalen;
			uint16_t errarg16;
			struct {
				uint8_t errarg8_0;
				uint8_t errarg8_1;
			};
		};
		
		union {
			uint8_t flags;
			struct {
				uint8_t buserr_notify : 1;
				uint8_t policy : 2;
			};
		};
	};

	struct message {
		dlist_head lnk;

		union {
			char* buffer;
			message_header* header;
		};

		char* raddr;
		char* saddr;
		char* data;	

		size_t flen;

		size_t printTo(gxx::io::ostream& o) const {
			gxx::fprint_to(o, "(addrlen:{},stage:{},datalen:{})", header->addrlen, header->stage, header->datalen);
		}
	};

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