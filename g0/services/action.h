#ifndef G0_ACTION_H
#define G0_ACTION_H

#include <gxx/event/delegate.h>
#include <g1/tower.h>

namespace g0 {
	struct action_service : public service {
		gxx::delegate<void, g1::packet*> dlg;

		action_service(gxx::delegate<void, g1::packet*> dlg) : dlg(dlg) {}

		void incoming_packet(g1::packet* pack) override {
			dlg(pack);
		}
	};

	g0::action_service* create_action_service(int i, gxx::delegate<void, g1::packet*> dlg);
}

#endif