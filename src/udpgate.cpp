#include <g0/gate/udp.h>

void g0::udp_gate::on_input(g0::message* msg) {
	id_t node = *(g0::id_t*)(msg->raddr + *msg->stage);

	udp_gate_address* it;
	sock.sendto(table[node].addr, table[node].port, msg->buffer, msg->flen);
	
	g0::utilize(msg);
	return;
}

void g0::udp_gate::read_handler() {
	char* pack = (char*) malloc(1024);

	gxx::inet::netaddr naddr;
	int ret = sock.recvfrom(pack, 1024, &naddr);
	pack = (char*) realloc(pack, ret);

	g0::message* msg = new g0::message;
	g0::message_parse(msg, pack, ret);

	id_t node = 0xFFFF;

	for(int i = 0; i < table.size(); ++i) {
		if (table[i].addr == naddr.addr && table[i].port == naddr.port) {
			node = i;
		}
	}

	*(g0::id_t*)(msg->saddr + msg->addrlen - *msg->stage - 1) = node;
	*msg->stage += sizeof(g0::id_t);	 

	g0::transport(msg, id); 
}
