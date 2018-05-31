#ifndef G0_KERNEL_H
#define G0_KERNEL_H

namespace g0 {
	//Инициализация таблицы регистрации сервисов. 
	//Функция должна вызываться до начала работы с системой сообщений. 
	void service_table_init();

	void message_init(g0::message* pkb, const g0::id_t* raddr, uint8_t rlen, const char* data, size_t dlen);
	void message_parse(g0::message* pkb, char* data, size_t size);
	
	id_t read_stage(g0::message* msg);
	void mark_stage(g0::message* pkb, id_t id);
	void transport(g0::message* pkb, id_t sid);
	void buserror(g0::message* pkb, id_t sid);
}

#endif