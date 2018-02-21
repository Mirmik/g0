#include <g0/gate/udp.h>

void g0::udp_gate::on_input(g0::message* msg) {
	gxx::println("here");
}


/*bool g0::udp_gate::send(id_t node, const char* data, size_t size) {
	
}

bool g0::udp_gate::send(id_t node, const char* data1, size_t size1, const char* data2, size_t size2) {

}*/
