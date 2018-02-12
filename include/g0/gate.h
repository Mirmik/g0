#ifndef G0_GATE_H
#define G0_GATE_H

#include <g0/service.h>

namespace g0 {
	class gate;

	constexpr uint8_t CTRLPACK = 0x01;

	constexpr uint8_t CTRLFUNC_CREATE = 0xAA;

	struct package_header {
		uint8_t msgtype;
	};

	struct control_package_header {
		uint8_t msgtype;
		uint8_t func;
	};

	struct ctrlfunc_create_t {
		id_t remote_id;	
		id_t local_id;	
	};

	struct ctrlfunc_destroy_t {
		id_t remote_id;	
		id_t local_id;	
	};

	struct control_package {
		control_package_header header;
		union {
			ctrlfunc_create_t create;
			ctrlfunc_destroy_t destroy;
		};
	};

	class vservice : public service {
	public:
		dlist_head chlnk;
		id_t remote_id;
		id_t local_id;
		gate* gt;

		void on_input(message* msg) override;
	};

	class gate {
	public:
		dlist_head channels;
		gate() {
			dlist_init_list(&channels);
		}

		virtual void send_package(const char* data, size_t sz) = 0;
		virtual void send_package(iovec* vec);
		void on_recv_package(const char* data, size_t sz);
	
	private:
		void create_vservice(id_t remote) {
			vservice* vsrvs = new vservice;
			vsrvs->remote_id = remote;
			vsrvs->gt = this;
			dlist_add_next(&vsrvs->chlnk, &channels);
		} 

	public:
		void packsend_vchannel_control(int locid, int remid, uint8_t func) {
			constexpr uint8_t size = 
				sizeof(control_package_header) + sizeof(ctrlfunc_create_t);
			
			char pack[size];
			control_package_header* header = (control_package_header*) pack;
			ctrlfunc_create_t* args = (ctrlfunc_create_t*)(pack + sizeof(control_package_header));
			
			header->msgtype = CTRLPACK;
			header->func = CTRLFUNC_CREATE;
			args->local_id = locid;
			args->remote_id = remid;
			
			send_package(pack, size);
		}

	};
}

#endif