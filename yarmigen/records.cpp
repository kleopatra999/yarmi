
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

#include "records.hpp"
#include "tokens.hpp"
#include "tools.hpp"
#include "type_id.hpp"

#include <string>
#include <map>

namespace yarmigen {

/***************************************************************************/

#define PRINT_CURSOR(tag, c) \
	std::cout << "[tag:" << tag << "][pos:" << c.format() << "]: *it='" << *(c.it) << "'" << std::endl << std::flush

/***************************************************************************/

struct record_namespace::impl {
	std::string name;
}; // struct record_namespace::impl

record_namespace::record_namespace()
	:pimpl(new impl)
{}

record_namespace::~record_namespace()
{ delete pimpl; }

void record_namespace::parse(proto_info &, cursor &c) {
	pimpl->name = get_to_sep(c, ' ', namespace_close_dotcomma_char);
	check_next(c, namespace_close_dotcomma_char);
}

void record_namespace::write(std::ostream &) const {}
void record_namespace::dump(std::ostream &os) const {
	os
	<< "namespace" << std::endl
	<< "  name: " << pimpl->name << std::endl;
}

record_type record_namespace::type() const { return record_type::namespace_; }

void record_namespace::accept(record_get_name_visitor &v) const { v.name = pimpl->name; }

/***************************************************************************/

struct record_class::impl {
	std::string name;
}; // struct record_class::impl

record_class::record_class()
	:pimpl(new impl)
{}

record_class::~record_class()
{ delete pimpl; }

void record_class::parse(proto_info &, cursor &c) {
	pimpl->name = get_to_sep(c, ' ', class_close_dotcomma_char);
	check_next(c, class_close_dotcomma_char);
}

void record_class::write(std::ostream &) const {}
void record_class::dump(std::ostream &os) const {
	os
	<< "class" << std::endl
	<< "  name: " << pimpl->name << std::endl;
}

//const std::string& record_class::name() const { return pimpl->name; }
record_type record_class::type() const { return record_type::class_; }
void record_class::accept(record_get_name_visitor &v) const { v.name = pimpl->name; }

/***************************************************************************/

struct record_enum::impl {
	impl()
		:type(type_id::type_u64)
	{}

	type_id type;
	std::string name;
	std::vector<std::pair<std::string, std::string>> elems;
}; // struct record_enum::impl

record_enum::record_enum()
	:pimpl(new impl)
{}

record_enum::~record_enum()
{ delete pimpl; }

void record_enum::parse(proto_info &, cursor &c) {
	pimpl->name = get_to_sep(c, ' ', enum_body_open_char, enum_type_separator);

	// if enum has specified underlying type
	skipws(c);
	if ( curch(c) == enum_type_separator ) {
		check_next(c, enum_type_separator);
		cursor c1 = c;
		const std::string type = get_to_sep(c, ' ', enum_body_open_char);
		pimpl->type = type_id_by_name(type);
		if ( pimpl->type == type_id::type_unknown )
			YARMI_THROW(
				"enum underlying type error: '%s' in %s"
				,type
				,c1.format()
			);
	}

	check_next(c, enum_body_open_char);

	for ( ;; ) {
		skipws(c);
		// if enum don't have any members
		char cc = curch(c);
		if ( cc == enum_body_close_char ) {
			check_next(c, enum_body_close_char);
			check_next(c, enum_body_close_dotcomma_char);
			return;
		}

		const std::string n = get_to_sep(c, ',', ' ', enum_assign_char, enum_body_close_char);

		skipws(c);
		cc = curch(c);
		// without init value
		if ( cc == ',' ) {
			nextch(c);
			pimpl->elems.push_back({n, ""});
		// with init value
		} else if ( cc == enum_assign_char ) {
			nextch(c);
			const std::string v = get_to_sep(c, ' ', ',', enum_body_close_char);
			pimpl->elems.push_back({n, v});
		// error
		} else {
			YARMI_THROW(
				"syntax error: '%c' in %s"
				,cc
				,c.format()
			);
		}
	}
}

void record_enum::write(std::ostream &) const {}

void record_enum::dump(std::ostream &os) const {
	os
	<< "enum" << std::endl
	<< "  name: " << pimpl->name << std::endl
	<< "  type: " << type_name_by_id(pimpl->type) << std::endl
	<< "  {" << std::endl;
	for ( auto it = pimpl->elems.begin(); it != pimpl->elems.end(); ++it ) {
		const char sep = (it == pimpl->elems.begin() ? ' ' : ',');
		if ( it->second.empty() ) {
			os << "    " << sep << it->first << std::endl;
		} else {
			os << "    " << sep << it->first << '=' << it->second << std::endl;
		}
	}
	os << "  };" << std::endl;
}

//const std::string& record_enum::name() const { return pimpl->name; }
record_type record_enum::type() const { return record_type::enum_; }

void record_enum::accept(record_get_name_visitor &v) const { v.name = pimpl->name; }

/***************************************************************************/

struct record_using::impl {
	std::string name;
	std::string type;
}; // struct record_using::impl

record_using::record_using()
	:pimpl(new impl)
{}

record_using::~record_using()
{ delete pimpl; }

void record_using::parse(proto_info &, cursor &c) {
	pimpl->name = get_to_sep(c, ' ', using_assign_char);
	check_next(c, using_assign_char);
	pimpl->type = get_to_sep(c, using_close_dotcomma_char);
	check_next(c, using_close_dotcomma_char);
}

void record_using::write(std::ostream &) const {}

void record_using::dump(std::ostream &os) const {
	os
	<< "using" << std::endl
	<< "  name: " << pimpl->name << std::endl
	<< "  type: " << pimpl->type << std::endl;
}

record_type record_using::type() const { return record_type::using_; }

void record_using::accept(record_get_name_visitor &v) const { v.name = pimpl->name; }

/***************************************************************************/

struct record_const::impl {
	std::string type;
	std::string name;
	std::string val;
}; // struct record_const::impl

record_const::record_const()
	:pimpl(new impl)
{}

record_const::~record_const()
{ delete pimpl; }

void record_const::parse(proto_info &, cursor &c) {
	pimpl->type = get_to_sep(c, ' ');
	pimpl->name = get_to_sep(c, ' ', const_assign_char);
	check_next(c, const_assign_char);
	pimpl->val = get_to_sep(c, const_close_dotcomma_char);
	check_next(c, const_close_dotcomma_char);
}

void record_const::write(std::ostream &) const {}

void record_const::dump(std::ostream &os) const {
	os
	<< "const" << std::endl
	<< "  type: " << pimpl->type << std::endl
	<< "  name: " << pimpl->name << std::endl
	<< "  val : " << pimpl->val << std::endl;
}

record_type record_const::type() const { return record_type::const_; }

void record_const::accept(record_get_name_visitor &v) const { v.name = pimpl->name; }

/***************************************************************************/

struct record_proc::impl {
	std::string name, req_name, handler_name, sig;
}; // struct record_proc::impl

record_proc::record_proc()
	:pimpl(new impl)
{}

record_proc::~record_proc()
{ delete pimpl; }

void record_proc::parse(proto_info &, cursor &c) {
	pimpl->req_name = get_to_sep(c, ' ', '-');
	check_next(c, '-');
	check_next(c, '>');
	pimpl->handler_name = get_to_sep(c, ' ', ':');
	check_next(c, ':');
	check_next(c, proc_sig_open_char);
	pimpl->sig = get_to_sep(c, proc_sig_close_char);
	check_next(c, proc_sig_close_char);
	check_next(c, proc_sig_close_dotcomma_char);
}

void record_proc::write(std::ostream &) const {}

void record_proc::dump(std::ostream &os) const {
	os
	<< "proc" << std::endl
	<< "  request: " << pimpl->req_name << std::endl
	<< "  handler: " << pimpl->handler_name << std::endl
	<< "  sig    : " << pimpl->sig << std::endl;
}

record_type record_proc::type() const { return record_type::proto_; }

/***************************************************************************/

struct record_var::impl {
	std::string type;
	std::string name;
}; // struct record_var::impl

record_var::record_var()
	:pimpl(new impl)
{}

record_var::~record_var()
{ delete pimpl; }

void record_var::parse(proto_info &, cursor &c) {
	const std::string str = get_to_sep(c, ';');
	check_next(c, ';');
	auto pos = str.rfind(' ');
	pimpl->type = str.substr(0, pos);
	pimpl->name = str.substr(pos+1);
}

void record_var::write(std::ostream &) const {}

void record_var::dump(std::ostream &os) const {
	os
	<< "var" << std::endl
	<< "  type: " << pimpl->type << std::endl
	<< "  name: " << pimpl->name << std::endl;
}

//const std::string& record_var::name() const { return pimpl->name; }
record_type record_var::type() const { return record_type::var_; }

void record_var::accept(record_get_name_visitor &v) const { v.name = pimpl->name; }

/***************************************************************************/

struct record_struct::impl {
	std::string name;
	std::vector<std::string> base;
	std::vector<record_ptr> members;
}; // struct record_struct::impl

record_struct::record_struct()
	:pimpl(new impl)
{}

record_struct::~record_struct()
{ delete pimpl; }

void record_struct::parse(proto_info &info, cursor &c) {
	pimpl->name = get_to_sep(c, ' ', struct_base_classes_separator, struct_body_open_char);

	skipws(c);
	// process list of base structs
	if ( curch(c) == struct_base_classes_separator ) {
		nextch(c);
		skipws(c);

		cursor tc = c;
		std::string base;
		bool flag = false;

		while ( ! flag ) {
			char ch = nextch(c);
			switch ( ch ) {
				case '/': {
					prevch(c);
					skipws(c);
					continue;
				}
				case ' ':
				case ',': {
					if ( ch == ',' && base.empty() )
						YARMI_THROW(
							"bad char '%c', expected name of base struct in %s"
							,ch
							,c.format()
						);

					if ( !struct_already_declared(info, base) )
						YARMI_THROW(
							 "base struct '%s'(%s) is not declared"
							,base
							,tc.format()
						);

					pimpl->base.push_back(base);
					base.clear();
					skipws(c);
					if ( curch(c) == ',' ) {
						nextch(c);
						skipws(c);
					}
					tc = c;
					continue;
				}
				case struct_body_open_char: {
					flag = true;
					prevch(c);
					break;
				}
				default: {
					base.push_back(ch);
				}
			}
		}
	}

	check_next(c, struct_body_open_char);
	skipws(c);

	for ( ;; ) {
		const iterator it = c.it;
		const std::string kword = get_to_sep(c, ' ');
		const bool is_type = is_type_name(kword);
		c.it = (is_type ? it : c.it);

		record_ptr o = record_factory((is_type ? "var" : kword));
		if ( !o )
			YARMI_THROW(
				 "bad keyword \"%s\" in %s"
				,kword
				,c.format()
			);

		o->parse(info, c);
		pimpl->members.push_back(std::move(o));

		skipws(c);
		const char ch = curch(c);
		if ( ch == struct_body_close_char )
			break;
	}
	check_next(c, struct_body_close_char);
	check_next(c, struct_body_close_dotcomma_char);
}

void record_struct::write(std::ostream &) const {}

void record_struct::dump(std::ostream &os) const {
	os
	<< "struct" << std::endl
	<< "  name: " << pimpl->name << std::endl
	<< "  base: ";
	for ( auto it = pimpl->base.begin(); it != pimpl->base.end(); ++it ) {
		const char *sep = (it+1 != pimpl->base.end() ? ", " : "");
		os << *it << sep;
	}
	os
	<< std::endl
	<< "  members:" << std::endl;
	for ( const auto &it: pimpl->members )
		it->dump(os);

}

record_type record_struct::type() const { return record_type::struct_; }

void record_struct::accept(record_get_name_visitor &v) const { v.name = pimpl->name; }

/***************************************************************************/

record_ptr record_factory(const std::string &kword) {
	static const std::map<std::string, record_ptr(*)()> map = {
		 {"namespace", [](){return record_ptr(new record_namespace);}	}
		,{"class"    , [](){return record_ptr(new record_class);}		}
		,{"enum"     , [](){return record_ptr(new record_enum);}			}
		,{"struct"   , [](){return record_ptr(new record_struct);}		}
		,{"using"    , [](){return record_ptr(new record_using);}		}
		,{"const"    , [](){return record_ptr(new record_const);}		}
		,{"var"      , [](){return record_ptr(new record_var);}			}
		,{"proc"     , [](){return record_ptr(new record_proc);}			}
	};

	auto it = map.find(kword);
	if ( it == map.end() )
		return record_ptr();

	return it->second();
}

/***************************************************************************/

} // ns yarmigen
