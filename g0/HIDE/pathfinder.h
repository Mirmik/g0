#ifndef G0_PATHFINDER_H
#define G0_PATHFINDER_H

#include <g0/service.h>

namespace g0 {

	class pathfinder : public service {
		void on_input(g0::message* msg) override;
	};

}

#endif