#ifndef G0_BROCKER_THEME_H
#define G0_BROCKER_THEME_H

#include <map>
#include <set>
#include <unordered_map>
#include <g0/service.h>
#include <gxx/util/string.h>
#include <gxx/datastruct/iovec.h>

struct Subscriber;

struct Theme {
	std::string name;
	std::set<Subscriber*> subs;

	Theme(const std::string& name) : name(name) {}

	void notify(const char* data, size_t size) const;

	void notify(const std::string& data) const { notify(data.data(), data.size()); }
};

struct Subscriber {
	g0::service_address addr;
	uint32_t last_connect;
	std::set<Theme*> thms;

	void notify(const Theme* theme, const char* data, size_t size) const {
		std::string str0 = gxx::serialstr8(theme->name);
		std::string str1 = gxx::serialstr8(data, size);


		gxx::print_dump(str0.data(), str0.size());
		gxx::print_dump(str1.data(), str1.size());

		gxx::iovec vec[] = {
			{str0.data(), str0.size()},
			{str1.data(), str1.size()},
		};
		g0::send(0, addr, std::begin(vec), std::end(vec));
	}

	Subscriber(g0::service_address addr) : addr(addr) {}

	void link(Theme* thm) {
		thms.insert(thm);
		thm->subs.insert(this);
	}
};

struct Brocker {
	std::unordered_map<g0::service_address, Subscriber> subscribers;
	std::unordered_map<std::string, Theme> themes;

	void add_subscriber(const g0::service_address& addr) {
		subscribers.emplace(addr, addr);
	}

	void add_theme(const std::string& name) {
		themes.emplace(name, name);
	}

	void notify(const std::string& theme, const std::string& data) const {
		try {
			const auto& thm = themes.at(theme);
			thm.notify(data);
		} catch (...) {
			gxx::println("unregistred theme");
		}
	}

	void subscribe(const std::string& thm, g0::message* msg) {
		auto addr = g0::remoteaddr(msg);

		Subscriber* sub;
		try {
			sub = &subscribers.at(addr);
		} catch (...) {
			add_subscriber(addr);
			sub = &subscribers.at(addr);	
		}

		Theme* theme;
		try {
			theme = &themes.at(thm);
		} catch (...) {
			add_theme(thm);
			theme = &themes.at(thm);	
		}
		
		sub->link(theme);
	}

	void unsubscribe(const std::string& thm, g0::message* msg) {
		PANIC_TRACED();
	}
};


void Theme::notify(const char* data, size_t size) const {
	for (auto& sub: subs) {
		sub->notify(this, data, size);
	}
}


//void subthm_link(Subscriber& sub, std::string thmname);
//void subthm_unlink(Subscriber& sub, std::string thmname);

#endif