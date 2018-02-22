#include <g0/gate/udp.h>

void g0::udp_gate::on_input(g0::message* msg) {
	gxx::println("here");
	id_t node = *(g0::id_t*)(msg->raddr + *msg->stage);

	gxx::println("node", node);

	udp_gate_address* it;
	//it = table;
	sock.sendto(table[node].addr, table[node].port, msg->buffer, msg->flen);
	gxx::print("saddr: "); 
	gxx::printhex(msg->saddr, msg->addrlen); 
	gxx::println();
	
	g0::utilize(msg);
	return;
}

void g0::udp_gate::read_handler() {
	char* pack = (char*) malloc(1024);

	gxx::inet::netaddr naddr;
	int ret = sock.recvfrom(pack, 1024, &naddr);
	gxx::println(ret);
	pack = (char*) realloc(pack, ret);
	gxx::print_dump((void*)pack, ret);

	g0::message* msg = new g0::message;
	g0::message_parse(msg, pack, ret);

	id_t node = 0xFFFF;

	gxx::println(naddr.addr);
	gxx::println(naddr.port);
	gxx::println(table[0].addr);
	gxx::println(table[0].port);

	for(int i = 0; i < table.size(); ++i) {
		if (table[i].addr == naddr.addr && table[i].port == naddr.port) {
			gxx::println("here");
			node = i;
		}
	}

	*(g0::id_t*)(msg->saddr + msg->addrlen - *msg->stage - 1) = node;
	*msg->stage += sizeof(g0::id_t);	 

	g0::transport(msg, id); 
}


/*bool g0::udp_gate::send(id_t node, const char* data, size_t size) {
	
}

bool g0::udp_gate::send(id_t node, const char* data1, size_t size1, const char* data2, size_t size2) {

}*/
