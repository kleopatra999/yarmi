
// Copyright (c) 2013,2014, niXman (i dotty nixman doggy gmail dotty com)
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

#ifndef _yarmigen__records_hpp
#define _yarmigen__records_hpp

#include "visitors.hpp"

#include <memory>
#include <iosfwd>

namespace yarmigen {

/***************************************************************************/

enum class record_type {
	 namespace_
	,class_
	,enum_
	,struct_
	,using_
	,const_
	,var_
	,proto_
};

/***************************************************************************/

struct proto_info;
struct cursor;

struct record_base {
	virtual void parse(proto_info &info, cursor &c) = 0;
	virtual void write(std::ostream &os) const = 0;
	virtual void dump(std::ostream &os) const = 0;
	virtual record_type type() const = 0;

	virtual ~record_base() {}
}; // struct record_base

/***************************************************************************/

struct record_namespace: record_base {
	record_namespace();
	~record_namespace();

	void parse(proto_info &info, cursor &c);
	void write(std::ostream &os) const;
	void dump(std::ostream &os) const;
	record_type type() const;

	void accept(record_get_name_visitor &v) const;

private:
	struct impl;
	impl *pimpl;
}; // struct record_namespace

/***************************************************************************/

struct record_class: record_base {
	record_class();
	~record_class();

	void parse(proto_info &info, cursor &c);
	void write(std::ostream &os) const;
	void dump(std::ostream &os) const;
	record_type type() const;

	void accept(record_get_name_visitor &v) const;

private:
	struct impl;
	impl *pimpl;
}; // struct record_class

/***************************************************************************/

struct record_enum: record_base {
	record_enum();
	~record_enum();

	void parse(proto_info &info, cursor &c);
	void write(std::ostream &os) const;
	void dump(std::ostream &os) const;
	record_type type() const;

	void accept(record_get_name_visitor &v) const;

private:
	struct impl;
	impl *pimpl;
}; // struct record_enum

/***************************************************************************/

struct record_using: record_base {
	record_using();
	~record_using();

	void parse(proto_info &info, cursor &c);
	void write(std::ostream &os) const;
	void dump(std::ostream &os) const;
	record_type type() const;

	void accept(record_get_name_visitor &v) const;

private:
	struct impl;
	impl *pimpl;
}; // struct record_using

/***************************************************************************/

struct record_const: record_base {
	record_const();
	~record_const();

	void parse(proto_info &info, cursor &c);
	void write(std::ostream &os) const;
	void dump(std::ostream &os) const;
	record_type type() const;

	void accept(record_get_name_visitor &v) const;

private:
	struct impl;
	impl *pimpl;
}; // struct record_const

/***************************************************************************/

struct record_proc: record_base {
	record_proc();
	~record_proc();

	void parse(proto_info &info, cursor &c);
	void write(std::ostream &os) const;
	void dump(std::ostream &os) const;
	record_type type() const;

private:
	struct impl;
	impl *pimpl;
}; // struct record_proc

/***************************************************************************/

struct record_var: record_base {
	record_var();
	~record_var();

	void parse(proto_info &info, cursor &c);
	void write(std::ostream &os) const;
	void dump(std::ostream &os) const;
	record_type type() const;

	void accept(record_get_name_visitor &v) const;

private:
	struct impl;
	impl *pimpl;
}; // struct record_var

/***************************************************************************/

struct record_struct: record_base {
	record_struct();
	~record_struct();

	void parse(proto_info &info, cursor &c);
	void write(std::ostream &os) const;
	void dump(std::ostream &os) const;
	record_type type() const;

	void accept(record_get_name_visitor &v) const;

private:
	struct impl;
	impl *pimpl;
}; // struct record_struct

/***************************************************************************/

using record_ptr = std::unique_ptr<record_base>;

record_ptr record_factory(const std::string &kword);

/***************************************************************************/

} // ns yarmigen

#endif // _yarmigen__records_hpp
