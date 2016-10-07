
// Copyright (c) 2013-2016, niXman (i dotty nixman doggy gmail dotty com)
// All rights reserved.
//
// This file is part of YARMI(https://github.com/niXman/yarmi) project.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
//   Redistributions in binary form must reproduce the above copyright notice, this
//   list of conditions and the following disclaimer in the documentation and/or
//   other materials provided with the distribution.
//
//   Neither the name of the {organization} nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <yarmi/detail/throw/throw.hpp>
#include "checks.hpp"
#include "records.hpp"
#include "protoinfo.hpp"

#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/size_t.hpp>

#include <algorithm>
#include <set>

namespace yarmigen {

/***************************************************************************/

template<record_type RT>
struct get_records_by_type {
private:
	#define CAST(E) \
		boost::mpl::size_t<static_cast<std::size_t>(E)>

	using records_map = boost::mpl::map<
		 boost::mpl::pair<CAST(record_type::namespace_), record_namespace>
		,boost::mpl::pair<CAST(record_type::class_    ), record_class    >
		,boost::mpl::pair<CAST(record_type::enum_     ), record_enum     >
		,boost::mpl::pair<CAST(record_type::struct_   ), record_struct   >
		,boost::mpl::pair<CAST(record_type::using_    ), record_using    >
		,boost::mpl::pair<CAST(record_type::const_    ), record_const    >
		,boost::mpl::pair<CAST(record_type::var_      ), record_var      >
		,boost::mpl::pair<CAST(record_type::proto_    ), record_proc     >
	>;

	static_assert(
		 boost::mpl::has_key<records_map, CAST(RT)>::value
		,"T is not in records_map"
	);

public:
	using type = typename boost::mpl::at<records_map, CAST(RT)>::type;
	using cont = std::vector<type*>;

	static cont apply(const std::vector<proto_info> &info) {
		cont res;

		for ( auto ibeg = info.begin(); ibeg != info.end(); ++ibeg ) {
			for ( auto rbeg = ibeg->cl_records.begin(); rbeg != ibeg->cl_records.end(); ++rbeg ) {
				if ( rbeg->get()->type() == RT )
					res.push_back(static_cast<type*>(rbeg->get()));
			}
			for ( auto rbeg = ibeg->sr_records.begin(); rbeg != ibeg->sr_records.end(); ++rbeg ) {
				if ( rbeg->get()->type() == RT )
					res.push_back(static_cast<type*>(rbeg->get()));
			}
		}

		return res;
	}

	#undef CAST
};

/***************************************************************************/

void check_namespace_names(const std::vector<proto_info> &info) {
	using records = get_records_by_type<record_type::namespace_>;
	records::cont vec = records::apply(info);
	if ( vec.size() != 2 ) {
		YARMI_THROW("please choose namespaces in client and server section");
	}
}

/***************************************************************************/

void check_class_names(const std::vector<proto_info> &info) {
	using records = get_records_by_type<record_type::class_>;
	records::cont vec = records::apply(info);
	if ( vec.size() != 2 ) {
		YARMI_THROW("please choose class names in client and server section");
	}

	record_get_name_visitor gn1, gn2;
	vec[0]->accept(gn1);
	vec[1]->accept(gn2);
	if ( gn1.name == gn2.name ) {
		YARMI_THROW("client and server class names can't be the same");
	}
}

/***************************************************************************/

template<typename ResItem, typename Visitor, typename RT>
std::vector<ResItem> collect_props(const std::vector<RT*> &vec) {
	std::vector<ResItem> res;

	std::for_each(vec.begin(), vec.end(),
		[&res](RT *r) {
			Visitor vi;
			r->accept(vi);
			res.push_back(vi.name);
		}
	);

	return res;
}

/***************************************************************************/

template<typename Visitor, record_type eRecordType>
std::vector<std::string> find_duplicates(const std::vector<proto_info> &info) {
	using records = get_records_by_type<eRecordType>;
	typename records::cont vec = records::apply(info);
	std::vector<std::string>  names = collect_props<
		 std::string
		,record_get_name_visitor
	>(vec);
	std::sort(names.begin(), names.end());

	std::set<std::string> uset(names.begin(), names.end());

	std::vector<std::string> dup;
	std::set_difference(
		 names.begin(), names.end()
		,uset.begin(), uset.end()
		,std::back_inserter(dup)
	);

	return dup;
}

/***************************************************************************/

void check_enums_unique_names(const std::vector<proto_info> &info) {
	std::vector<std::string> dup = find_duplicates<
		 record_get_name_visitor
		,record_type::enum_
	>(info);

	if ( !dup.empty() )
		YARMI_THROW("enum \"%1%\" is declared twice", dup[0]);
}

void check_enums(const std::vector<proto_info> &info) {
	check_enums_unique_names(info);
}

/***************************************************************************/

void full_check(const std::vector<proto_info> &info) {
	check_namespace_names(info);
	check_class_names(info);
	check_enums(info);
}

/***************************************************************************/

} // ns yarmigen
