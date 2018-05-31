#include <g0/gate/udp.h>

void g0::udp_gate::on_input(g0::message* msg) {
	gxx::println("g0::on_input");
	id_t node = *(g0::id_t*)(msg->raddr + msg->header->stage);

	auto it = idmap.find(node);
	if (it == idmap.end()) { 
		gxx::println("g0::udp_gate. buserror");
		g0::buserror(msg, id);
		return;
	} 

	gxx::fprintln("sock.sendto {} {}", it->second->addr, it->second->port);
	sock.sendto(it->second->addr, it->second->port, msg->buffer, msg->flen);
	
	g0::utilize(msg);
	return;
}

void g0::udp_gate::read_handler() {
	gxx::println("g0::udp_gate::read_handler");
	char* pack = (char*) malloc(1024);

	gxx::inet::netaddr naddr;
	int ret = sock.recvfrom(pack, 1024, &naddr);
	pack = (char*) realloc(pack, ret);

	//Обработка пинга адреса.
	if (ret == 1 && pack[0] == G0_HANDSHAKE) {
		sock.sendto(naddr.addr, naddr.port, pack, 1);
		free(pack);
		return;
	} 

	g0::message* msg = new g0::message;
	g0::message_parse(msg, pack, ret);

	id_t node;

	auto it = addrmap.find(naddr);
	if (it == addrmap.end()) { 
		gxx::println("info: unknown address, add temporary node");
		node = add(naddr.addr, naddr.port, true);
	} else {
		node = it->second->node;
	}
	
	g0::mark_stage(msg, node);
	g0::transport(msg, id);
}


static int idcounter = 33; 
g0::id_t g0::udp_gate::add(gxx::hostaddr addr, int port, bool temporary) {
	udp_gate_address* uaddr = new udp_gate_address(addr, port);
	uaddr->node = idcounter++;
	uaddr->temporary = false;
	addrmap[gxx::inet::netaddr(addr, port)] = uaddr;
	idmap[uaddr->node] = uaddr;
	return uaddr->node;
}

