#ifndef G0_MESSAGE_H
#define G0_MESSAGE_H

#include <gxx/buffer.h>

namespace g1 { struct packet; }
namespace g0 {

	struct message {
		dlist_head lnk;

		uint16_t sid;
		uint16_t rid;

		g1::packet* pack;

		char* data;
		size_t size;

		gxx::buffer datasect() {
			return gxx::buffer(data, size);
		}
	};

	

}

#endif