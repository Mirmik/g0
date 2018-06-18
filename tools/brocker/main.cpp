#include <g0/core.h>
#include <g1/tower.h>
#include <g1/indexes.h>
#include <g1/gates/udpgate.h>
#include <gxx/log/target2.h>

#include <thread>

#include <brocker.h>

g1::udpgate udpgate;

void udplisten();
void g1execute();
void incoming_handler(g1::packet* pack);
void print_diagnostic();

Brocker brocker;

/**
	Структура сообщения. Дата секция.
	1 байт - тип пакета.

	Для типа сообщения. 
	1 байт размер имени.
	N байт - имя.
	2 байта - размер данных.
	M байт - данные.

	Для подписки / отписки.
	1 байт размер имени.
	N байт имя.	  
*/

#define BROCKER_DATA_MESSAGE 0

struct brocker_service : public g0::service {
	void incoming_message(g0::message* msg) override {
		gxx::fprintln("brocker incoming, data:{}", gxx::buffer(msg->data, msg->size));
		
		auto datasect = msg->datasect();
		gxx::print_dump(datasect.data(), datasect.size());

		switch (datasect[0]) {
			case 0: {
					gxx::println("publish");
					uint8_t thmsize = datasect[1];
					std::string thm = std::string(&datasect[2], thmsize);
					GXX_PRINT(thm.c_str());

					uint8_t datasize = datasect[2+thmsize];
					std::string data = std::string(&datasect[2 + 1 + thmsize], datasize);
					GXX_PRINT(data.c_str());

					brocker.notify(thm, data);
				}
				break;
			case 1: {
					gxx::println("subscribe");
					uint8_t thmsize = datasect[1];
					std::string thm = std::string(&datasect[2], thmsize);
					GXX_PRINT(thm.c_str());
	
					brocker.subscribe(thm, msg);
				}
				break;
			case 2: {
					gxx::println("unsubscribe");
					uint8_t thmsize = datasect[1];
					std::string thm = std::string(&datasect[2], thmsize);
					GXX_PRINT(thm.c_str());
	
					brocker.unsubscribe(thm, msg);
				}
				break;
			default:
				gxx::println("unresolved type");
				break;
		}
		g0::utilize(msg);
		print_diagnostic();
	}
} brocker_service;

gxx::log::colored_stdout_target console_target;

int main() {
	g1::logger.link(console_target, gxx::log::level::trace);
	g0::logger.link(console_target, gxx::log::level::trace);

	udpgate.open(10000);

	g1::link_gate(&udpgate, G1_UDPGATE);
	g0::link_service(&brocker_service, 0);

	g1::incoming_handler = incoming_handler;

	std::thread thr(udplisten);
	std::thread thrg1(g1execute);
	thr.join();
	thrg1.join();
}

void udplisten() {
	while(1) udpgate.exec_syncrecv();
}

void incoming_handler(g1::packet* pack) {
	gxx::println("incoming handler");
	if (pack->block->type == G1_G0TYPE) {
		g0::travell(pack);
	} else {
		g1::release(pack);
	}
}

void g1execute() {
	while(1) {
		g1::one_thread_execute();
	}
}

void print_diagnostic() {
	GXX_PRINT(brocker.themes.size());
	GXX_PRINT(brocker.subscribers.size());
}