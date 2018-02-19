#ifndef G0_NETWORK_H
#define G0_NETWORK_H

#include <g0/service.h>

namespace g0 {
	constexpr id_t LOCAL_NODE = 0xFFFF;
	constexpr id_t NAIVE_NODE = 0xFFFF;
	constexpr uint8_t PACKTYPE_MESSAGE = 0x02;

	extern id_t node_id;

	struct address {
		id_t node;
		id_t serv;
	};

	struct package_header {
		id_t snid;
		id_t rnid;
		uint8_t packtype;
	};

	struct message_header : public package_header {
		id_t sid;
		id_t rid;
	};

	struct gate {
		virtual bool send(id_t to, const char* data, size_t size) = 0;
		virtual bool send(id_t to, const char* data1, size_t size1, const char* data2, size_t size2);
		
		void on_package_receive(char* data, size_t sz);
	};

	void set_gates(gate**);

	/*struct inet_gate : public gate {
		void send(char* data, size_t size) override;
	};*/

//	struct posix_rdm_gate : public gate {
//		gxx::inet::socket  
//	};

	/*class router {
		void retranslate_message(message* msg) = 0;
		void retranslate_package(char* data, size_t size) = 0;
	};*/

	/*class naive_router : public router {
	public:
		gate* gt;
		void retranslate_message(message* msg);
		void retranslate_package(char* data, size_t size);
	};*/
}

#endif