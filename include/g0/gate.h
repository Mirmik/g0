#ifndef G0_GATE_H
#define G0_GATE_H

#include <g0/service.h>

namespace g0 {
	class gate;

	struct package_header {
		uint8_t msgtype;
	};

	struct control_package {
		package_header header;
		uint8_t func;
		union {
			id_t remote_id;
		};
	};

	class vservice : public service {
	public:
		dlist_head chlnk;
		id_t remote_id;
		gate* gt;

		void on_input(message* msg) override;
	};

	class gate {
	public:
		dlist_head channels;

		virtual void send_package(const char* data, size_t sz) = 0;
		virtual void send_package(iovec* vec);
		void on_recv_package(const char* data, size_t sz);
	
	private:
		void create_vservice(id_t remote) {
			vservice* vsrvs = new vservice;
			vsrvs->remote_id = remote;
			vsrvs->gt = this;
		} 
	};
}

#endif