#include <g0/gate/udp.h>

g0::udp_gate_address ugaddr[] = {
	{17, "127.0.0.1", 11363},
	{0, "0.0.0.0", 0}
};

g0::udp_gate ugate(11363, ugaddr);
g0::gate* gates[] = { &ugate, nullptr };

int main() {
	g0::set_gates(gates);
	g0::set_node_id(g0::NAIVE_NODE);

	char* buf = (char*) malloc(128);
	const char* msgstr = "HelloGate";

	g0::package_header* pack = (g0::package_header*) buf;
	pack->snid = 0;
	pack->rnid = 17;
	pack->packtype = g0::PACKTYPE_TEST;
	char* packdata = (char*)(pack+1);
	strcpy(packdata, msgstr);

	buf = (char*) realloc(buf, sizeof(g0::package_header) + strlen(msgstr));
	g0::pkb pkb(buf, sizeof(g0::package_header) + strlen(msgstr));

	g0::retranslate_package(pkb, true);


}