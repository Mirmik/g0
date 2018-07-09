#ifndef G0_SERVICE_H
#define G0_SERVICE_H

#include <gxx/event/delegate.h>
#include <gxx/datastruct/dlist_head.h>
#include <g0/message.h>

namespace g0 {

	struct basic_service {
		dlist_head lnk; ///< Линк для подключения к списку сервисов.
		uint16_t id; ///< Идентификатор сервиса.

		basic_service() { dlist_init(&lnk); }

		virtual void incoming_message(g0::message* msg) = 0;
	};

	/// Добавить сервис к ядру.
	void link_service(g0::basic_service* srvs, uint16_t id);

	struct service : public basic_service {
		gxx::delegate<void, g0::message*> dlg;
		service(gxx::delegate<void, g0::message*> dlg) : dlg(dlg) {}
		void incoming_message(g0::message* msg) override { dlg(msg); }
	};

	g0::service* make_service(int idx, gxx::delegate<void, g0::message*> dlg);

}

#endif