#ifndef G0_NETWORK_H
#define G0_NETWORK_H

#include <g0/service.h>
#include <gxx/print.h>

namespace g0 {
	constexpr id_t LOCAL_NODE = 0xFFFF;
	constexpr id_t NAIVE_NODE = 0xFFFF;
	constexpr uint8_t PACKTYPE_MESSAGE = 0x02;
	constexpr uint8_t PACKTYPE_TEST = 0x03;

	extern id_t node_id;

	struct pkb {
		char* buffer;
		char* raddr;
		char* saddr;
		char* data;
		
		uint8_t addrlen;
		uint8_t* stage;

		size_t datalen;

		pkb(char* data, size_t size);

		size_t printTo(gxx::io::ostream& o) const {
			gxx::fprint_to(o, "(addrlen:{},stage:{},datalen:{})", addrlen, stage, datalen);
		}
	};

	void form_package_header(char* pack, const char* raddr, uint8_t addrlen);

	struct gate {
		virtual bool translate->

	/*	virtual bool send(id_t to, const char* data, size_t size) = 0;
		virtual bool send(id_t to, const char* data1, size_t size1, const char* data2, size_t size2) = 0;

		void on_package_receive(char* data, size_t sz);
	*/};
/*
	void set_gates(gate**);
	void set_node_id(id_t n);
*/
	bool retranslate_package(g0::pkb* pack);
	bool receive_package(g0::pkb* pack);
/*	bool translate_package(g0::pkb pack);
	bool receive_package(g0::pkb pack);
	*/

	/*struct inet_gate : public gate {
		void send(char* data, size_t size) override;
	};*/

//	struct posix_rdm_gate : public ge {
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
