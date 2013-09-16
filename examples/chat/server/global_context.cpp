
#include "user_context.hpp"
#include "global_context.hpp"

#include <yas/binary_iarchive.hpp>
#include <yas/binary_oarchive.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

#include <fstream>

/***************************************************************************/

enum {
	 registered = 0x02
	,activated = 0x04
};

struct session_wrapper {
	session_wrapper()
		:id(-1)
	{}

	std::int64_t id;
	std::string name;
	std::size_t state;

	struct by_id;
	struct by_name;
	struct by_state;

	template<typename Archive>
	void serialize(Archive &ar) {
		ar & id
			& name
			& state;
	}
};

/***************************************************************************/

template<>
struct global_context<user_context>::pimpl {
	typedef boost::multi_index::multi_index_container<
		 session_wrapper
		,boost::multi_index::indexed_by<
			boost::multi_index::ordered_unique<
				 boost::multi_index::tag<session_wrapper::by_id>
				,boost::multi_index::member<
					session_wrapper, std::int64_t, &session_wrapper::id
				>
			>
			,boost::multi_index::ordered_unique<
				 boost::multi_index::tag<session_wrapper::by_name>
				,boost::multi_index::member<
					session_wrapper, std::string, &session_wrapper::name
				>
			>
			,boost::multi_index::ordered_unique<
				 boost::multi_index::tag<session_wrapper::by_state>
				,boost::multi_index::member<
					session_wrapper, std::size_t, &session_wrapper::state
				>
			>
		>
	> users_db_type;

	static bool exists(const std::string &fname) {
		std::ifstream file(fname);
		return file;
	}

	void load() {
		if ( exists(users_db_name) ) {
			std::ifstream file(users_db_name, std::ios::binary);
			yas::binary_file_iarchive ia(file);
			while ( file.good() ) {
				session_wrapper sw;
				ia & sw;
				users_db.insert(sw);
			}
		}
		if ( exists(messages_db_name) ) {
			std::ifstream file(messages_db_name, std::ios::binary);
			yas::binary_file_iarchive ia(file);
//			while ( file.good() ) {
//			}
		}
	}
	void save() {
		{	std::ofstream file(users_db_name, std::ios::binary|std::ios::trunc);
			yas::binary_file_oarchive oa(file);
			for ( const auto &it: users_db ) {
				oa & it;
			}
		}
		{	std::ofstream file(messages_db_name, std::ios::binary|std::ios::trunc);
			yas::binary_file_oarchive oa(file);
		}
	}

	pimpl(const std::string &users_db_name, const std::string &messages_db_name)
		:users_db_name(users_db_name)
		,messages_db_name(messages_db_name)
	{
		load();
	}

	~pimpl() {
		save();
	}

	users_db_type users_db;

	std::string users_db_name;
	std::string messages_db_name;
};

/***************************************************************************/

template<>
global_context<user_context>::global_context(const std::string &users_db, const std::string &messages_db)
	:impl(new pimpl(users_db, messages_db))
{}

template<>
global_context<user_context>::~global_context()
{ delete impl; }

/***************************************************************************/

template<>
bool global_context<user_context>::user_registered(const std::string &username) const {

}

template<>
bool global_context<user_context>::user_activated(const std::string &username) const {

}

template<>
bool global_context<user_context>::user_logged_in(const std::string &username) const {

}

template<>
void global_context<user_context>::registration(const std::string &username) {

}

template<>
void global_context<user_context>::activation(const std::string &regkey, const std::string &username, const std::string &password) {

}

template<>
void global_context<user_context>::login(const std::string &username, const std::string &password) {

}

template<>
void global_context<user_context>::logout() {

}

template<>
void global_context<user_context>::users_online() {

}

/***************************************************************************/
