#include <g0/core.h>
#include <g1/tower.h>
#include <g1/indexes.h>
#include <g1/address.h>
#include <g1/gates/udpgate.h>
#include <g1/gates/serial_gstuff.h>
#include <gxx/log/target2.h>
#include <gxx/serial/serial.h>
#include <gxx/io/file.h>

#include <thread>

gxx::log::colored_stdout_target console_target;

struct srvcls : public g0::service {
	void incoming_message(g0::message* msg) override {
		gxx::writeln(msg->data, msg->size);
		g0::utilize(msg);
	}
};

void incoming_handler(g1::packet* pack);

int main() {
	g1::logger.link(console_target, gxx::log::level::trace);
	g0::logger.link(console_target, gxx::log::level::trace);

	srvcls srv;
	g0::link_service(&srv, 0);

	g1::udpgate udpgate;

	serial::Serial ser("/dev/ttyUSB0", 115200);
	gxx::io::file fl(ser.fd());
	g1::serial_gstuff_gate serialgate(&fl);


	udpgate.open(10003);

	g1::link_gate(&udpgate, G1_UDPGATE);
	g1::link_gate(&serialgate, 0x42);

	const char msgdata[] = "g";

	//g0::service_address addr;
	//addr.id = 2;
	//addr.raddr = gxx::buffer("\x0C", 1);
	g1::incoming_handler = incoming_handler;

	g1::address addr;
	addr.pushu8(0x42);
	//addr.pushudp(G1_UDPGATE, "127.0.0.1", 10004);


	//sleep(2);

	sleep(1);
	ser.setDTR(false);
	usleep(10000);
	ser.setDTR(true);


	//g0::send(0, addr, msgdata, sizeof(msgdata));
	//g0::send(0, 2, addr.data(), addr.size(), "HelloWorld", 10, g1::QoS(0));

	g1::spin();
}

void incoming_handler(g1::packet* pack) {
	//gxx::println("main incoming handler");
	g1::print(pack);

	if (pack->header.type == G1_G0TYPE) {
		//gxx::println("retrans to g0");
		g0::travell(pack);
	} else {
		g1::release(pack);
	}
}