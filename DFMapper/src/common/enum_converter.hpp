#pragma once

#include "enum_definition.hpp"
#include "../util/util.hpp"
#include <stdexcept>
#include <cstdio>

namespace DFMpr
{
	class NodeTypeConverter
	{
	public:
		static auto toString(NodeType type_)->string
		{
			if (type_ == NodeType::pe)
				return "pe";
			if (type_ == NodeType::fg)
				return "fg";
			if (type_ == NodeType::ls)
				return "ls";
			if (type_ == NodeType::lv)
				return "lv";
			if (type_ == NodeType::lc)
				return "lc";
			if (type_ == NodeType::bus)
				return "bus";
			if (type_ == NodeType::bus0)
				return "bus0";
			if (type_ == NodeType::bus1)
				return "bus1";
			if (type_ == NodeType::fifo)
				return "fifo";
			if (type_ == NodeType::router)
				return "router";
			if (type_ == NodeType::rt)
				return "rt";
			if (type_ == NodeType::null)
				return "null";
			if (type_ == NodeType::begin)
				return "begin";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toEnum(string s_)->NodeType
		{
			if (s_ == "pe")
				return NodeType::pe;
			if (s_ == "fg")
				return NodeType::fg;
			if (s_ == "ls")
				return NodeType::ls;
			if (s_ == "bus")
				return NodeType::bus;
			if (s_ == "bus0")
				return NodeType::bus0;
			if (s_ == "bus1")
				return NodeType::bus1;
			if (s_ == "lv")
				return NodeType::lv;
			if (s_ == "lc")
				return NodeType::lc;
			if (s_ == "fifo")
				return NodeType::fifo;
			if (s_ == "router")
				return NodeType::router;
			if (s_ == "rt")
				return NodeType::rt;
			if (s_ == "null")
				return NodeType::null;
			if (s_ == "begin")
				return NodeType::begin;
			throw std::runtime_error("your configuration is boom(nodetype)");
			DEBUG_ASSERT(false);
			return NodeType::null;
		}

		static auto isRealNode(NodeType type_) -> bool
		{
			return !(type_ == NodeType::begin || type_ == NodeType::null);
		}
	};

	using BlockTypeConverter = NodeTypeConverter;
	class InputBufferBypassConverter 
	{
	public:
		static auto toString(InputBufferBypass type_)->string
		{
			if (type_ == InputBufferBypass::inbuffer)
				return "inbuffer";
			if (type_ == InputBufferBypass::bypass)
				return "bypass";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toBit(InputBufferBypass type_)->string
		{
			if (type_ == InputBufferBypass::inbuffer)
				return "0";
			if (type_ == InputBufferBypass::bypass)
				return "1";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toEnum(string s_)->InputBufferBypass
		{
			if (s_ == "inbuffer" || s_ == "0")
				return InputBufferBypass::inbuffer;
			if (s_ == "bypass" || s_ == "1")
				return InputBufferBypass::bypass;
			throw std::runtime_error("your configuration is boom(input_buffer_bypass)");
			DEBUG_ASSERT(false);
			return InputBufferBypass::inbuffer;
		}
	};
	class InBufferFromConverter {
	public:
		static auto toString(InBufferFrom type_)->string
		{
			if (type_ == InBufferFrom::alu)
				return "alu";
			if (type_ == InBufferFrom::flr)
				return "null";
			if (type_ == InBufferFrom::in0)
				return "in0";
			if (type_ == InBufferFrom::in1)
				return "in1";
			if (type_ == InBufferFrom::in2)
				return "in2";
			if (type_ == InBufferFrom::aluin1)
				return "aluin1";
			if (type_ == InBufferFrom::in0in1)
				return "in0in1";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toBit(InBufferFrom type_)->string
		{
			if (type_ == InBufferFrom::flr)
				return "0";
			if (type_ == InBufferFrom::in0)
				return "1";
			if (type_ == InBufferFrom::in2)
				return "1";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toBit2(InBufferFrom type_)->string
		{
			if (type_ == InBufferFrom::flr)
				return "00";
			if (type_ == InBufferFrom::in1)
				return "01";
			if (type_ == InBufferFrom::aluin1)
				return "10";
			if (type_ == InBufferFrom::in0in1)
				return "11";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toEnum(string s_)->InBufferFrom
		{
			if (s_ == "alu")
				return InBufferFrom::alu;
			if (s_ == "null" || s_ == "0" || s_ == "00")
				return InBufferFrom::flr;
			if (s_ == "in0" || s_ == "1")
				return InBufferFrom::in0;
			if (s_ == "in1" || s_ == "01")
				return InBufferFrom::in1;
			if (s_ == "in2" || s_ == "1")
				return InBufferFrom::in2;
			if (s_ == "aluin1" || s_ == "10")
				return InBufferFrom::aluin1;
			if (s_ == "in0in1" || s_ == "11")
				return InBufferFrom::in0in1;
			throw std::runtime_error("your configuration is boom(inbufferfrom)");
			DEBUG_ASSERT(false);
			return InBufferFrom::flr;
		}
	};

	class CondConverter {
	public:
		static auto toString(CondMode type_)->string
		{
			if (type_ == CondMode::cb)
				return "cb";
			if (type_ == CondMode::cinvb)
				return "cinvb";
			if (type_ == CondMode::cond)
				return "cond";
			if (type_ == CondMode::in2data)
				return "in2data";
			DEBUG_ASSERT(false);
		}
		static auto toEnum(string s_)->CondMode
		{
			if (s_ == "cb")
				return CondMode::cb;
			if (s_ == "cinvb")
				return CondMode::cinvb;
			if (s_ == "cond")
				return CondMode::cond;
			if (s_ == "in2data")
				return CondMode::in2data;
			throw std::runtime_error("your configuration is boom(condmode)");
			DEBUG_ASSERT(false);
		}
	};
	class BufferModeConverter {
	public:
		static auto toString(BufferMode type_)->string
		{
			if (type_ == BufferMode::buffer)
				return "buffer";
			if (type_ == BufferMode::keep)
				return "keep";
			if (type_ == BufferMode::lr_out)
				return "lr_out";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toBit(BufferMode type_)->string
		{
			if (type_ == BufferMode::buffer)
				return "0";
			if (type_ == BufferMode::keep)
				return "1";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toEnum(string s_)->BufferMode
		{
			if (s_ == "buffer" || s_ == "0")
				return BufferMode::buffer;
			if (s_ == "keep" || s_ == "1")
				return BufferMode::keep;
			if (s_ == "lr")
				return BufferMode::keep;
			if (s_ == "lr_out")
				return BufferMode::lr_out;
			if (s_ == "lrffer")
				return BufferMode::lrffer;
			throw std::runtime_error("your configuration is boom(buffer_mode)");
			DEBUG_ASSERT(false);
			return BufferMode::buffer;
		}
	};
	class FGModeConverter
	{
	public:
		static auto toString(FGMode type_)->string
		{
			if (type_ == FGMode::lbegin)
				return "lbegin";
			if (type_ == FGMode::null)
				return "null";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toEnum(string s_)->FGMode
		{
			if (s_ == "null")
				return FGMode::null;
			if (s_ == "lbegin")
				return FGMode::lbegin;
			throw std::runtime_error("your configuration is boom(fgmode)");
			DEBUG_ASSERT(false);
			return FGMode::null;
		}
	};
	class VecmodeConverter 
	{
	public:
		static auto toString(VecMode type_)->string
		{
			if (type_ == VecMode::null)
				return "null";
			if (type_ == VecMode::vect)
				return "vect";
			if (type_ == VecMode::vecr)
				return "vecr";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toEnum(string s_)->VecMode
		{
			if (s_ == "null")
				return VecMode::null;
			if (s_ == "vect")
				return VecMode::vect;
			if (s_ == "vecr")
				return VecMode::vecr;
			throw std::runtime_error("your configuration is boom(vecmode)");
			DEBUG_ASSERT(false);
			return VecMode::null;
		}
	};
	class PortValidConverter
	{
	public:
		static auto toString(PortValid type_)->string
		{
			if (type_ == PortValid::PEA)
				return "PEA";
			if (type_ == PortValid::BUS)
				return "BUS";
			DEBUG_ASSERT(false);
			return "";
		}
		static auto toEnum(string s_)->PortValid
		{
			if (s_ == "PEA")
				return PortValid::PEA;
			if (s_ == "BUS")
				return PortValid::BUS;
		}
	};
	class FunctionOpConverter
	{
	public:
		static auto toString(FunctionOp type_)->string
		{
			if (type_ == FunctionOp::gatherBreak)
				return "gatherBreak";
			if (type_ == FunctionOp::gatherEnd)
				return "gatherEnd";
			if (type_ == FunctionOp::gatherLast)
				return "gatherLast";
		}
		static auto toEnum(string s_)->FunctionOp
		{
			if (s_ == "gatherBreak")
				return FunctionOp::gatherBreak;
			if (s_ == "gatherEnd")
				return FunctionOp::gatherEnd;
			if (s_ == "gatherLast")
				return FunctionOp::gatherLast;
			throw std::runtime_error("your configuration is boom(busopcode)");
			DEBUG_ASSERT(false);
		}
	};
	class PEOpcodeConverter
	{
	public:
		static auto toString(PEOpcode type_)->string
		{
			if (type_ == PEOpcode::null)
				return "nop";
			if (type_ == PEOpcode::sadd)
				return "sadd";
			if (type_ == PEOpcode::hadd)
				return "hadd";
			if (type_ == PEOpcode::hradd)
				return "hradd";
			if (type_ == PEOpcode::hrsf)
				return "hrsf";
			if (type_ == PEOpcode::hrneq)
				return "hrneq";
			if (type_ == PEOpcode::hlt)
				return "hlt";
			if (type_ == PEOpcode::add)
				return "add";
			if (type_ == PEOpcode::mul)
				return "mul";
			if (type_ == PEOpcode::sub)
				return "sub";
			if (type_ == PEOpcode::div)
				return "div";
			if (type_ == PEOpcode::mod)
				return "mod";
			if (type_ == PEOpcode::lt)
				return "lt";
			if (type_ == PEOpcode::eq)
				return "eq";
			if (type_ == PEOpcode::mux)
				return "mux";
			if (type_ == PEOpcode::mac)
				return "mac";
			if (type_ == PEOpcode::add1_comp)
				return "add1_comp";
			if (type_ == PEOpcode::add1)
				return "add1";
			if (type_ == PEOpcode::leshift)
				return "leshift";
			if (type_ == PEOpcode::rishift)
				return "rishift";
			if (type_ == PEOpcode::bit_and)
				return "and";
			if (type_ == PEOpcode::bit_or)
				return "or";
			if (type_ == PEOpcode::cos)
				return "cos";
			if (type_ == PEOpcode::sin)
				return "sin";
			if (type_ == PEOpcode::bit_xor)
				return "xor";
			if (type_ == PEOpcode::smux)
				return "smux";
			if (type_ == PEOpcode::neq)
				return "neq";
			if (type_ == PEOpcode::bit_not)
				return "not";
			if (type_ == PEOpcode::merge)
				return "merge";
			if (type_ == PEOpcode::lte)
				return "lte";
			if (type_ == PEOpcode::hrlt)
				return "hrlt";
			if (type_ == PEOpcode::hgt)
				return "hgt";
			if (type_ == PEOpcode::hgte)
				return "hgte";
			if (type_ == PEOpcode::hrgt)
				return "hrgt";
			if (type_ == PEOpcode::logic_and)
				return "logic_and";
			if (type_ == PEOpcode::logic_or)
				return "logic_or";
			if (type_ == PEOpcode::logic_not)
				return "logic_not";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toBit(PEOpcode type_)->string
		{
			if (type_ == PEOpcode::null)
				return "00000";
			if (type_ == PEOpcode::sadd)
				return "00001";
			if (type_ == PEOpcode::mul)
				return "00010";
			if (type_ == PEOpcode::add || type_ == PEOpcode::hadd)
				return "00011";
			if (type_ == PEOpcode::sub)
				return "00100";
			if (type_ == PEOpcode::lt || type_ == PEOpcode::hlt)
				return "00101";
			if (type_ == PEOpcode::lte)
				return "00110";
			if (type_ == PEOpcode::eq)
				return "00111";
			if (type_ == PEOpcode::mac)
				return "01000";
			if (type_ == PEOpcode::bit_and)
				return "01001";
			if (type_ == PEOpcode::bit_or)
				return "01010";
			if (type_ == PEOpcode::bit_not)
				return "01011";	
			if (type_ == PEOpcode::bit_xor)
				return "01100";
			if (type_ == PEOpcode::mux)
				return "01101";
			if (type_ == PEOpcode::leshift)
				return "01110";
			if (type_ == PEOpcode::rishift)
				return "01111";
			if (type_ == PEOpcode::logic_and)
				return "10000";
			if (type_ == PEOpcode::logic_or)
				return "10001";
			if (type_ == PEOpcode::logic_not)
				return "10010";
			if (type_ == PEOpcode::hgt)
				return "10011";
			if (type_ == PEOpcode::hgte)
				return "10100";
			if (type_ == PEOpcode::neq)
				return "10101";
			if (type_ == PEOpcode::div)
				return "10110";
			else
				std::cout << toString(type_);
			DEBUG_ASSERT(false);
			return 0;
		}

		static auto toEnum(string s_)->PEOpcode
		{
			if (s_ == "nop" || s_ == "00000")
				return PEOpcode::null;
			if (s_ == "sadd" || s_ == "00001")
				return PEOpcode::sadd;
			if (s_ == "add" || s_ == "00011")
				return PEOpcode::add;
			if (s_ == "mul" || s_ == "00010")
				return PEOpcode::mul;
			if (s_ == "sub" || s_ == "00100")
				return PEOpcode::sub;
			if (s_ == "lt" || s_ == "00101")
				return PEOpcode::lt;
			if (s_ == "lte" || s_ == "00110")
				return PEOpcode::lte;
			if (s_ == "eq" || s_ == "00111")
				return PEOpcode::eq;
			if (s_ == "mac" || s_ == "01000")
				return PEOpcode::mac;
			if (s_ == "and" || s_ == "01001")
				return PEOpcode::bit_and;
			if (s_ == "or" || s_ == "01010")
				return PEOpcode::bit_or;
			if (s_ == "not" || s_ == "01011")
				return PEOpcode::bit_not;
			if (s_ == "xor" || s_ == "01100")
				return PEOpcode::bit_xor;
			if (s_ == "mux" || s_ == "01101")
				return PEOpcode::mux;
			if (s_ == "leshift" || s_ == "01110")
				return PEOpcode::leshift;
			if (s_ == "rishift" || s_ == "01111")
				return PEOpcode::rishift;
			if (s_ == "logic_and" || s_ == "10000")
				return PEOpcode::logic_and;
			if (s_ == "logic_or" || s_ == "10001")
				return PEOpcode::logic_or;
			if (s_ == "logic_not" || s_ == "10010")
				return PEOpcode::logic_not;
			if (s_ == "hgt" || s_ == "10011")
				return PEOpcode::hgt;
			if (s_ == "hgte" || s_ == "10100")
				return PEOpcode::hgte;
			if (s_ == "neq" || s_ == "10101")
				return PEOpcode::neq;
			if (s_ == "div" || s_ == "10110")
				return PEOpcode::div;
			if (s_ == "mod")
				return PEOpcode::mod;
			if (s_ == "add1_comp")
				return PEOpcode::add1_comp;
			if (s_ == "add1")
				return PEOpcode::add1;
			if (s_ == "hadd")
				return PEOpcode::hadd;
			if (s_ == "hradd")
				return PEOpcode::hradd;
			if (s_ == "hrsf")
				return PEOpcode::hrsf;
			if (s_ == "hrneq")
				return PEOpcode::hrneq;
			if (s_ == "hlt")
				return PEOpcode::hlt;
			if (s_ == "cos")
				return PEOpcode::cos;
			if (s_ == "sin")
				return PEOpcode::sin;
			if (s_ == "smux")
				return PEOpcode::smux;
			if (s_ == "merge")
				return PEOpcode::merge;
			if (s_ == "hrlt")
				return PEOpcode::hrlt;
			if (s_ == "hrgt")
				return PEOpcode::hrgt;
			throw std::runtime_error("your configuration is boom(opcode)");
			DEBUG_ASSERT(false);
			return PEOpcode::null;
		}
	};

	class LocalregFromConverter
	{
	public:
		static auto toString(LocalregFrom type_)->string
		{
			if (type_ == LocalregFrom::null)
				return "null";
			if (type_ == LocalregFrom::alu)
				return "alu";
			if (type_ == LocalregFrom::in1)
				return "in1";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toEnum(string s_)->LocalregFrom
		{
			if (s_ == "null")
				return LocalregFrom::null;
			if (s_ == "alu")
				return LocalregFrom::alu;
			if (s_ == "in1")
				return LocalregFrom::in1;
			throw std::runtime_error("your configuration is boom(lr_from)");
			DEBUG_ASSERT(false);
			return LocalregFrom::null;
		}
	};

	class ControlModeConverter
	{
	public:
		static auto toString(ControlMode type_)->string
		{
			if (type_ == ControlMode::data_flow)
				return "null";
			if (type_ == ControlMode::loop_activate)
				return "loop_activate";
			if (type_ == ControlMode::loop_bind)
				return "loop_bind";
			if (type_ == ControlMode::loop_reset)
				return "loop_reset";
			if (type_ == ControlMode::break_pre)
				return "break_pre";
			if (type_ == ControlMode::break_post)
				return "break_post";
			if (type_ == ControlMode::lc_one)
				return "lc_one";
			if (type_ == ControlMode::lc_one_ini)
				return "lc_one_ini";
			if (type_ == ControlMode::lc_two)
				return "lc_two";
			if (type_ == ControlMode::lc_two_ini)
				return "lc_two_ini";
			if (type_ == ControlMode::continue_)
				return "continue_";
			if (type_ == ControlMode::cb)
				return "cb";
			if (type_ == ControlMode::cinvb)
				return "cinvb";
			if (type_ == ControlMode::bind)
				return "bind";
			if (type_ == ControlMode::transout)
				return "transout";
			if (type_ == ControlMode::trans)
				return "trans";
			if (type_ == ControlMode::fixstep_lc_one)
				return "fixstep_lc_one";
			if (type_ == ControlMode::fixN_lc_one)
				return "fixN_lc_one";
			if (type_ == ControlMode::fixbrk_lc_one)
				return "fixbrk_lc_one";
			if (type_ == ControlMode::calc_lc_two)
				return "calc_lc_two";
			if (type_ == ControlMode::brkgen)
				return "brkgen";
			if (type_ == ControlMode::comp_lc_two)
				return "comp_lc_two";
			if(type_ == ControlMode::calc_activate)
				return "calc_activate";
			if (type_ == ControlMode::fixstep_lc_one_ini)
				return "fixstep_lc_one_ini";
			if (type_ == ControlMode::fixN_lc_one_ini)
				return "fixN_lc_one_ini";
			if (type_ == ControlMode::fixbrk_lc_one_ini)
				return "fixbrk_lc_one_ini";
			if (type_ == ControlMode::comp_lc_two_ini)
				return "comp_lc_two_ini";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toEnum(string s_)->ControlMode
		{
			if (s_ == "null")
				return ControlMode::data_flow;
			if (s_ == "systolic")
				return ControlMode::systolic;
			if (s_ == "loop_activate")
				return ControlMode::loop_activate;
			if (s_ == "loop_bind")
				return ControlMode::loop_bind;
			if (s_ == "loop_reset")
				return ControlMode::loop_reset;
			if (s_ == "calc_activate")
				return ControlMode::calc_activate;
			if (s_ == "break_pre")
				return ControlMode::break_pre;
			if (s_ == "break_post")
				return ControlMode::break_post;
			if (s_ == "continue_")
				return ControlMode::continue_;
			if (s_ == "cb")
				return ControlMode::cb;
			if (s_ == "cinvb")
				return ControlMode::cinvb;
			if(s_=="bind")
				return ControlMode::bind;
			if (s_ == "transout")
				return ControlMode::transout;
			if (s_ == "trans")
				return ControlMode::trans;
			if (s_ == "fixstep_lc_one")
				return ControlMode::fixstep_lc_one;
			if (s_ == "fixN_lc_one")
				return ControlMode::fixN_lc_one;
			if (s_ == "lc_one")
				return ControlMode::lc_one;
			if (s_ == "lc_one_ini")
				return ControlMode::lc_one_ini;
			if (s_ == "lc_two")
				return ControlMode::lc_two;
			if (s_ == "lc_two_ini")
				return ControlMode::lc_two_ini;
			if (s_ == "fixbrk_lc_one")
				return ControlMode::fixbrk_lc_one;
			if (s_ == "calc_lc_two")
				return ControlMode::calc_lc_two;
			if (s_ == "brkgen")
				return ControlMode::brkgen;
			if (s_ == "comp_lc_two")
				return ControlMode::comp_lc_two;
			if (s_ == "fixstep_lc_one_ini")
				return ControlMode::fixstep_lc_one_ini;
			if (s_ == "fixN_lc_one_ini")
				return ControlMode::fixN_lc_one_ini;
			if (s_ == "fixbrk_lc_one_ini")
				return ControlMode::fixbrk_lc_one_ini;
			if (s_ == "comp_lc_two_ini")
				return ControlMode::comp_lc_two_ini;
			throw std::runtime_error("your configuration is boom(control mode)");
			DEBUG_ASSERT(false);
			return ControlMode::data_flow;
		}
	};

	class LoopControlConverter {
	public:
		static auto toString(LoopCtrol type_)->string
		{
			if (type_ == LoopCtrol::null)
				return "null";
			if (type_ == LoopCtrol::inner_loop)
				return "inner_loop";
			if (type_ == LoopCtrol::outermost_loop)
				return "outermost_loop";
			if (type_ == LoopCtrol::last_match0)
				return "last_match0";
			if (type_ == LoopCtrol::last_match1)
				return "last_match1";
			if (type_ == LoopCtrol::last_match2)
				return "last_match2";
			if (type_ == LoopCtrol::break_gen)
				return "break_gen";
			if (type_ == LoopCtrol::break_pre)
				return "break_pre";
			if (type_ == LoopCtrol::break_post)
				return "break_post";
			if (type_ == LoopCtrol::systolic)
				return "systolic";
			if (type_ == LoopCtrol::inner_loop_sync)
				return "inner_loop_sync";
			if (type_ == LoopCtrol::inner_loop_ini)
				return "inner_loop_ini";
			if (type_ == LoopCtrol::inner_loop_ini_sync)
				return "inner_loop_ini_sync";
			if (type_ == LoopCtrol::trans)
				return "trans";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toBit(LoopCtrol type_)->string
		{
			if (type_ == LoopCtrol::null)
				return "0000";
			if (type_ == LoopCtrol::inner_loop)
				return "0001";
			if (type_ == LoopCtrol::outermost_loop)
				return "0010";
			if (type_ == LoopCtrol::last_match0)
				return "0011";
			if (type_ == LoopCtrol::last_match1)
				return "0100";
			if (type_ == LoopCtrol::break_gen)
				return "0101";
			if (type_ == LoopCtrol::break_pre)
				return "0110";
			if (type_ == LoopCtrol::break_post)
				return "0111";
			if (type_ == LoopCtrol::systolic)
				return "1000";
			if (type_ == LoopCtrol::inner_loop_sync)
				return "1001";
			if (type_ == LoopCtrol::inner_loop_ini)
				return "1010";
			if (type_ == LoopCtrol::inner_loop_ini_sync)
				return "1011";
			if (type_ == LoopCtrol::trans)
				return "1100";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toEnum(string s_)->LoopCtrol
		{
			if (s_ == "null" || s_ == "0000")
				return LoopCtrol::null;
			if (s_ == "inner_loop" || s_ == "0001")
				return LoopCtrol::inner_loop;
			if (s_ == "outermost_loop" || s_ == "0010")
				return LoopCtrol::outermost_loop;
			if (s_ == "last_match0" || s_ == "0011")
				return LoopCtrol::last_match0;
			if (s_ == "last_match1" || s_ == "0100")
				return LoopCtrol::last_match1;
			if (s_ == "last_match2")
				return LoopCtrol::last_match2;
			if (s_ == "break_gen" || s_ == "0101")
				return LoopCtrol::break_gen;
			if (s_ == "break_pre" || s_ == "0110")
				return LoopCtrol::break_pre;
			if (s_ == "break_post" || s_ == "0111")
				return LoopCtrol::break_post;
			if (s_ == "systolic" || s_ == "1000")
				return LoopCtrol::systolic;
			if (s_ == "inner_loop_sync" || s_ == "1001")
				return LoopCtrol::inner_loop_sync;
			if (s_ == "inner_loop_ini" || s_ == "1010")
				return LoopCtrol::inner_loop_ini;
			if (s_ == "inner_loop_ini_sync" || s_ == "1011")
				return LoopCtrol::inner_loop_ini_sync;
			if (s_ == "trans" || s_ == "1100")
				return LoopCtrol::trans;
			throw std::runtime_error("your configuration is boom(loopcontrol from)");
			DEBUG_ASSERT(false);
			return LoopCtrol::null;
		}
	};
	class BranchControlConverter {
	public:
		static auto toString(BranchControl type_)->string {
			if (type_ == BranchControl::null)
				return "null";
			if (type_ == BranchControl::cb)
				return "cb";
			if (type_ == BranchControl::cinvb)
				return "cinvb";
			if (type_ == BranchControl::merge)
				return "merge";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toBit(BranchControl type_)->string {
			if (type_ == BranchControl::null)
				return "00";
			if (type_ == BranchControl::cb)
				return "01";
			if (type_ == BranchControl::cinvb)
				return "10";
			if (type_ == BranchControl::merge)
				return "11";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toEnum(string s_)->BranchControl {
			if (s_ == "null" || s_ == "00")
				return BranchControl::null;
			if (s_ == "cb" || s_ == "01")
				return BranchControl::cb;
			if (s_ == "cinvb" || s_ == "10")
				return BranchControl::cinvb;
			if (s_ == "merge" || s_ == "11")
				return BranchControl::merge;
			throw std::runtime_error("your configuration is boom(branchcontrol)");
			DEBUG_ASSERT(false);
			return BranchControl::null;
		}

	};
	class AluInFromConverter
	{
	public:
		static auto toString(AluInFrom type_)->string
		{
			if (type_ == AluInFrom::null)
				return "null";
			if (type_ == AluInFrom::inbuffer)
				return "inbuffer";
			if (type_ == AluInFrom::inport)
				return "inport";
			if (type_ == AluInFrom::lr)
				return "lr";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toEnum(string s_)->AluInFrom
		{
			if (s_ == "null")
				return AluInFrom::null;
			if (s_ == "inbuffer")
				return AluInFrom::inbuffer;
			if (s_ == "inport")
				return AluInFrom::inport;
			if (s_ == "lr")
				return AluInFrom::lr;
			throw std::runtime_error("your configuration is boom(alu from)");
			DEBUG_ASSERT(false);
			return AluInFrom::null;
		}
	};
	class StallModeConverter
	{
	public:
		static auto toString(stallType type_)->string
		{
			if (type_ == stallType::none)
				return "none";
			if (type_ == stallType::inbuffer_stall)
				return "inbuffer_stall";
			if (type_ == stallType::step_stall)
				return "step_stall";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toEnum(string s_)->stallType
		{
			if (s_ == "none")
				return stallType::none;
			if (s_ == "inbuffer_stall")
				return stallType::inbuffer_stall;
			if (s_ == "step_stall")
				return stallType::step_stall;
			throw std::runtime_error("your configuration is boom(stall_type,recommend choice:none,inbuffer_stall,step_stall)");
			DEBUG_ASSERT(false);
		}
	};

	class OutputBufferBypassConverter
	{
	public:
		static auto toString(OutputBufferBypass type_)->string
		{
			if (type_ == OutputBufferBypass::bypass)
				return "bypass";
			if (type_ == OutputBufferBypass::outbuffer)
				return "ob";
			if (type_ == OutputBufferBypass::null)
				return "null";
			if (type_ == OutputBufferBypass::inbuffer)
				return "inbuffer";
			if (type_ == OutputBufferBypass::lr)
				return "lr";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toBit(OutputBufferBypass type_)->string
		{
			if (type_ == OutputBufferBypass::bypass)
				return "1";
			if (type_ == OutputBufferBypass::outbuffer)
				return "0";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toEnum(string s_)->OutputBufferBypass
		{
			if (s_ == "outbuffer" || s_ == "0")
				return OutputBufferBypass::outbuffer;
			if (s_ == "bypass" || s_ == "1")
				return OutputBufferBypass::bypass;
			if (s_ == "null")
				return OutputBufferBypass::null;
			if (s_ == "inbuffer")
				return OutputBufferBypass::inbuffer;
			if (s_ == "lr")
				return OutputBufferBypass::lr;
			throw std::runtime_error("your configuration is boom(output from)");
			DEBUG_ASSERT(false);
			return OutputBufferBypass::outbuffer;
		}
	};

	class LSModeConverter
	{
	public:
		static auto toString(LSMode type_)->string
		{
			if (type_ == LSMode::load)
				return "load";
			if (type_ == LSMode::store_data)
				return "store_data";
			if (type_ == LSMode::store_addr)
				return "store_addr";
			if (type_ == LSMode::dummy)
				return "dummy";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toEnum(string s_)->LSMode
		{
			if (s_ == "load")
				return LSMode::load;
			if (s_ == "store_data")
				return LSMode::store_data;
			if (s_ == "store_addr")
				return LSMode::store_addr;
			if (s_ == "dummy")
				return LSMode::dummy;
			if (s_ == "g2l")
				return LSMode::g2l;
			if (s_ == "push")
				return LSMode::push;
			if (s_ == "pop")
				return LSMode::pop;
			if (s_ == "enque")
				return LSMode::enque;
			if (s_ == "deque")
				return LSMode::deque;
			if (s_ == "l2g")
				return LSMode::l2g;
			if (s_ == "l2p")
				return LSMode::l2p;
			if (s_ == "p2l")
				return LSMode::p2l;
			if (s_ == "release_async")
				return LSMode::release_async;
			if (s_ == "release")
				return LSMode::release;
			if (s_ == "g2p")
				return LSMode::g2p;
			if (s_ == "p2g")
				return LSMode::p2g;
			throw std::runtime_error("your configuration is boom(lsmode)");
			DEBUG_ASSERT(false);
			return LSMode::load;
		}
	};

	class BufferSizeConverter
	{
	public:
		static auto toString(BufferSize type_)->string
		{
			if (type_ == BufferSize::small)
				return "small";
			if (type_ == BufferSize::middle)
				return "middle";
			if (type_ == BufferSize::large)
				return "large";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toEnum(string s_)->BufferSize
		{
			if (s_ == "small")
				return BufferSize::small;
			if (s_ == "middle")
				return BufferSize::middle;
			if (s_ == "large")
				return BufferSize::large;
			throw std::runtime_error("your configuration is boom(buffersize)");
			DEBUG_ASSERT(false);
			return BufferSize::small;
		}
	};

	class PortTypeConverter
	{
	public:
		static auto toString(PortType type_)->string
		{
			if (type_ == PortType::d32_v1_b1_l1_s1)
				return "d32_v1_b1_l1_s1";
			if (type_ == PortType::v1_b1_l1_s1)
				return "v1_b1_l1_s1";
			if (type_ == PortType::d32_b1_v1)
				return "d32_b1_v1";
			if (type_ == PortType::d32_v1)
				return "d32_v1";
			if (type_ == PortType::d32_b1_v1_t)
				return "d32_b1_v1_t";
			if (type_ == PortType::d32_v1_t)
				return "d32_v1_t";
			if (type_ == PortType::d1_v1)
				return "d1_v1";
			if (type_ == PortType::d1_v1_t)
				return "d1_v1_t";
			if (type_ == PortType::bp)
				return "bp";
			if (type_ == PortType::func_op)
				return "func_op";
			if (type_ == PortType::b1)
				return "b1";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toEnum(string s_)->PortType
		{
			if (s_ == "d32_v1_b1_l1_s1")
				return PortType::d32_v1_b1_l1_s1;
			if (s_ == "v1_b1_l1_s1")
				return PortType::v1_b1_l1_s1;
			if (s_ == "d32_b1_v1")
				return PortType::d32_b1_v1;
			if (s_ == "d32_b1_v1")
				return PortType::d32_b1_v1;
			if (s_ == "d32_v1")
				return PortType::d32_v1;
			if (s_ == "d32_b1_v1_t")
				return PortType::d32_b1_v1_t;
			if (s_ == "d32_v1_t")
				return PortType::d32_v1_t;
			if (s_ == "d1_v1")
				return PortType::d1_v1;
			if (s_ == "d1_v1_t")
				return PortType::d1_v1_t;
			if (s_ == "bp")
				return PortType::bp;
			if (s_ == "func_op")
				return PortType::func_op;
			if (s_ == "b1")
				return PortType::b1;
			DEBUG_ASSERT(false);
			return PortType::null;
		}
	};

	using WireTypeConverter = PortTypeConverter;
	using SegmentTypeConverter = PortTypeConverter;

	class PortDirectionConverter
	{
	public:
		static auto toString(PortDirection type_)->string
		{
			if (type_ == PortDirection::in)
				return "in";
			if (type_ == PortDirection::out)
				return "out";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toEnum(string s_)->PortDirection
		{
			if (s_ == "in")
				return PortDirection::in;
			if (s_ == "out")
				return PortDirection::out;
			DEBUG_ASSERT(false);
			return PortDirection::null;
		}
	};

	class WireNetConfigModeConverter
	{
	public:
		static auto toString(WireNetConfigMode type_)->string
		{
			if (type_ == WireNetConfigMode::cluster)
				return "cluster";
			if (type_ == WireNetConfigMode::way4)
				return "way4";
			if (type_ == WireNetConfigMode::way8)
				return "way8";
			if (type_ == WireNetConfigMode::way4hop1)
				return "way4hop1";
			if (type_ == WireNetConfigMode::way4hop2)
				return "way4hop2";
			if (type_ == WireNetConfigMode::row_full_connect)
				return "row_full_connect";
			if (type_ == WireNetConfigMode::col_full_connect)
				return "col_full_connect";
			if (type_ == WireNetConfigMode::pe_fifo_connect)
				return "pe_fifo_connect";
			if (type_ == WireNetConfigMode::pe_rt_connect)
				return "pe_rt_connect";
			if (type_ == WireNetConfigMode::ls_rt_connect)
				return "ls_rt_connect";
			if (type_ == WireNetConfigMode::ls_fifo_connect)
				return "ls_fifo_connect";
			if (type_ == WireNetConfigMode::full_connect)
				return "full_connect";
			if (type_ == WireNetConfigMode::all_position_full_connect)
				return "all_position_full_connect";
			if (type_ == WireNetConfigMode::multi_connect)
				return "multi_connect";
			if (type_ == WireNetConfigMode::bus_pe_connect)
				return "bus_pe_connect";
			if (type_ == WireNetConfigMode::bus_bus_connect)
				return "bus_bus_connect";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toEnum(string s_)->WireNetConfigMode
		{
			if (s_ == "cluster")
				return WireNetConfigMode::cluster;
			if (s_ == "way4")
				return WireNetConfigMode::way4;
			if (s_ == "way8")
				return WireNetConfigMode::way8;
			if (s_ == "way4hop1")
				return WireNetConfigMode::way4hop1;
			if (s_ == "way4hop2")
				return WireNetConfigMode::way4hop2;
			if (s_ == "row_full_connect")
				return WireNetConfigMode::row_full_connect;
			if (s_ == "col_full_connect")
				return WireNetConfigMode::col_full_connect;
			if (s_ == "pe_fifo_connect")
				return WireNetConfigMode::pe_fifo_connect;
			if (s_ == "pe_rt_connect")
				return WireNetConfigMode::pe_rt_connect;
			if (s_ == "ls_rt_connect")
				return WireNetConfigMode::ls_rt_connect;
			if (s_ == "ls_fifo_connect")
				return WireNetConfigMode::ls_fifo_connect;
			if (s_ == "full_connect")
				return WireNetConfigMode::full_connect;
			if (s_ == "all_position_full_connect")
				return WireNetConfigMode::all_position_full_connect;
			if (s_ == "multi_connect")
				return WireNetConfigMode::multi_connect;
			if (s_ == "bus_pe_connect")
				return WireNetConfigMode::bus_pe_connect;
			if (s_ == "bus_bus_connect")
				return WireNetConfigMode::bus_bus_connect;
			DEBUG_ASSERT(false);
			return WireNetConfigMode::null;
		}
	};

	class PortFunctionConverter
	{
	public:
		static auto toString(PortFunction type_)->string
		{
			if (type_ == PortFunction::normal)
				return "normal";
			if (type_ == PortFunction::router)
				return "router";
			if (type_ == PortFunction::bp)
				return "bp";
			if (type_ == PortFunction::ctrlb)
				return "ctrlb";
			if (type_ == PortFunction::func)
				return "func";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toEnum(string s_)->PortFunction
		{
			if (s_ == "normal")
				return PortFunction::normal;
			if (s_ == "router")
				return PortFunction::router;
			if (s_ == "bp")
				return PortFunction::bp;
			if (s_ == "ctrlb")
				return PortFunction::ctrlb;
			if (s_ == "func")
				return PortFunction::func;
			DEBUG_ASSERT(false);
			return PortFunction::null;
		}
	};

	class BufferBpConverter
	{
	public:
		static auto toString(BpType type_)->string
		{
			if (type_ == BpType::credit)
				return "credit";
			if (type_ == BpType::ack)
				return "ack";
			DEBUG_ASSERT(false);
			return "";
		}

		static auto toEnum(string s_)->BpType
		{
			if (s_ == "credit")
				return BpType::credit;
			if (s_ == "ack")
				return BpType::ack;
			DEBUG_ASSERT(false);
			return BpType::credit;
		}
	};

	class OpTypeConverter
	{
	public:
		static auto toString(Op type_)->string
		{
			if (type_ == Op::Phi)
				return "Phi";
			if (type_ == Op::Add)
				return "Add";
			if (type_ == Op::Sub)
				return "Sub";
			if (type_ == Op::Mul)
				return "Mul";
			if (type_ == Op::Div)
				return "Div";
			if (type_ == Op::And)
				return "And";
			if (type_ == Op::Or)
				return "Or";
			if (type_ == Op::Xor)
				return "Xor";
			if (type_ == Op::Cmp)
				return "Cmp";
			if (type_ == Op::Load)
				return "Load";
			if (type_ == Op::Store)
				return "Store";
			if (type_ == Op::Undefine)
				return "Undef";
			Util::throwError("Undefined operation conversion!", __FILE__, __LINE__);
			return "";
		}
	};
}