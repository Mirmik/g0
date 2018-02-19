#include <g0/gate/udp.h>

g0::udp_gate_address ugaddr[] = {
	{17, "127.0.0.1", 11363},
	{0, "0.0.0.0", 0}
};

g0::udp_gate ugate(11363, ugaddr);
g0::gate* gates[] = { &ugate, nullptr };

int main() {
	g0::set_gates(gates);
}