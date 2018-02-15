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
	};

	struct gate {
		virtual void send(char* data, size_t size) = 0;
		virtual void send_message(message* msg);
	};

//	struct posix_rdm_gate : public gate {
//		gxx::inet::socket  
//	};



	/*class router {
		virtual void retranslate_message(message* msg) = 0;
		virtual void retranslate_package(char* data, size_t size) = 0;
	};

	class naive_router : public router {
	public:
		gate* gt;
		void retranslate_message(message* msg);
		void retranslate_package(char* data, size_t size);
	};*/
}

#endif