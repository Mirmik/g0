#ifndef G0_SERVICE_H
#define G0_SERVICE_H

#include <gxx/datastruct/dlist_head.h>
#include <g0/message.h>

namespace g0 {

	struct service {
		dlist_head lnk;
		uint16_t id;

		virtual void incoming_message(g0::message* msg) = 0;
		service() {
			dlist_init(&lnk);
		}
	};

}

#endif