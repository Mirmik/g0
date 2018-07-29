#include <g0/core.h>
#include <g0/services/test.h>
#include <g0/services/action.h>
#include <g1/tower.h>
#include <g1/indexes.h>
#include <gxx/print.h>
#include <gxx/syslock.h>

gxx::dlist<g0::service, &g0::service::lnk> g0::services;

void g0::__send(uint16_t sid, uint16_t rid, const uint8_t* raddr, size_t rsize, const char* data, size_t size, g1::QoS qos) {		
	g0::subheader sh;
	sh.sid = sid;
	sh.rid = rid;

	gxx::iovec iov[2] = {
		{ &sh, sizeof(sh) },
		{ data, size }
	};

	g1::send(raddr, rsize, iov, 2, G1_G0TYPE, qos);
}


void g0::incoming(g1::packet* pack) {
	auto sh = get_subheader(pack);
	for ( auto& srvs: g0::services ) {
		if (srvs.id == sh->rid) {
			srvs.incoming_packet(pack);
			return;
		}
	}
	gxx::println("g0: unresolved service. release packet");
	g1::release(pack);	
}

void g0::link_service(g0::service* srv, uint16_t id) {
	srv->id = id;
	g0::services.move_back(*srv);
}

g0::test_service* g0::create_test_service(int port) {
	auto tsrv = new g0::test_service();
	g0::link_service(tsrv, port);
	return tsrv;
}

g0::action_service* g0::create_action_service(int port, gxx::delegate<void, g1::packet*> dlg) {
	auto asrv = new g0::action_service(dlg);
	g0::link_service(asrv, port);
	return asrv;
}