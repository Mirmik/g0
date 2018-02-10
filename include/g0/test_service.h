#ifndef G0_TEST_SERVICE_H
#define G0_TEST_SERVICE_H

#include <g0/service.h>

namespace g0 {
	class test_service : public g0::service {
	public:
		const char* name;
		test_service(const char* str) : name(str) {}

		void on_input(message* msg) override;
		void on_reply(message* msg) override;
	};

	class echo_service : public g0::service {
	public:
		const char* name;
		echo_service(const char* str) : name(str) {}

		void on_input(message* msg) override;
		void on_reply(message* msg) override;
	};	
}

#endif