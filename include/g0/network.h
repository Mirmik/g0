#ifndef G0_NETWORK_H
#define G0_NETWORK_H

#include <g0/service.h>

namespace g0 {
	constexpr id_t LOCAL_NODE = 0xFFFF;
	constexpr id_t NAIVE_NODE = 0xFFFF;

	extern id_t node_id;

	struct pack_header {
		id_t snid;
		id_t rnid;
		uint8_t packtype;
	};

	struct msgpack_header : public pack_header {
		id_t sid;
		id_t rid;
		uint8_t stsbyte;
	};

	struct gate {
		virtual void send(char* data, size_t size) = 0;
		virtual void send_message(message* msg);
	};

	class router {
		virtual void retranslate(message* msg) = 0;
	};

	class naive_router {
	public:
		gate* gt;
		void export_message(message* msg);
		void retranslate_package(char* data, size_t size);
	};
}

#endif