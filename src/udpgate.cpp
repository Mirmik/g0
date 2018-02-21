#include <g0/gate/udp.h>

void g0::udp_gate::on_input(g0::message* msg) {
	gxx::println("here");
	id_t node = message_read_next_id(msg);

	gxx::println("node", node);

	udp_gate_address* it;
	it = table;
	while (it->port != 0) {
		if (it->node == node) {
			sock.sendto(it->addr, it->port, msg->buffer, msg->flen);
			g0::utilize(msg);
			return;
		}
		++it;
	} 
	g0::utilize(msg);
	return;
}


/*bool g0::udp_gate::send(id_t node, const char* data, size_t size) {
	
}

bool g0::udp_gate::send(id_t node, const char* data1, size_t size1, const char* data2, size_t size2) {

}*/
