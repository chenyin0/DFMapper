#pragma once
#include "../define/define.hpp"
#include "../util/cord.hpp"
#include "../tinyxml2/tinyxml2.h"

namespace DFMpr {
    enum class LogMode {
        Print_detail,
        Print_brief,
        Turn_off
    };

    enum class Op {
        Phi,
        Add,  // add, fadd
        Sub,  // sub, fsub
        Mul,  // mul, fmul, shl 
        Div,  // udiv, sdiv, fdiv, lshr, ashr, trunc
        And,  // and
        Or,  // or
        Xor,  // xor
        Cmp,  // icmp
        Load,  // load
        Store,  // store
        Undefine
    }; 

    enum class NodeType
    {
        /** real node */
        pe,
        fg,
        ls,
        lv,
        lc,
        /** functional node */
        null,
        begin,
        bus,
        bus0,
        bus1,
        fifo,
        router,
        rt
    };
    enum class LseSpmMode
    {
        /** real node */
        normal,
        g2l,
        l2g,
        l2p,
        p2l,
        /** functional node */
        g2p,
        p2g

    };
    enum class LcSync
    {
        invalid,
        valid,
        last
    };
    enum class FunctionOp {
        gatherLast,
        gatherBreak,
        gatherEnd
    };
    //std::ostream& operator<<(std::ostream& os, const enum class NodeType& nodetype)
    //{
    //	static const char* maps[7] = { "pe","fg","ls","lv","lc","null","begin" };
    //	return os << maps[int(nodetype)];
    //}
    using BlockType = NodeType;

    enum class PEOpcode
    {
        null,
        add,
        mul,
        sub,
        div,
        mod,
        lt,
        eq,
        mux,
        hneq,
        hrsf,
        hrneq,
        mac,
        add1,
        leshift,
        rishift,
        add1_comp,
        logic_and,
        logic_or,
        logic_not,
        bit_and,
        bit_or,
        bit_xor,
        cos,
        sin,
        smux,
        neq,
        lte,
        bit_not,
        merge,
        hlt,
        hadd,
        hradd,
        hrlt,
        hgt,
        hgte,
        hrgt,
        sadd
    };

    enum class VecMode
    {
        vect,
        vecr,
        null
    };
    enum class RecallMode
    {
        cached,
        nocache,
        both
    };
    enum class AckMode
    {
        goten,
        gotunen
    };
    enum class BpType
    {
        ack,
        credit
    };
    enum class stallType
    {
        none,
        inbuffer_stall,
        step_stall
    };
    enum class LoopCtrol {
        null,
        inner_loop,
        outermost_loop,
        last_match0,
        last_match1,
        last_match2,
        break_gen,
        break_pre,
        break_post,
        systolic,
        inner_loop_sync,
        inner_loop_ini,
        inner_loop_ini_sync,
        continue_,
        trans
    };
    enum class BranchControl {
        null,
        cb,
        cinvb,
        merge
    };

    enum class ControlMode
    {
        data_flow,
        loop_activate,
        calc_activate,
        loop_bind,
        loop_reset,
        break_pre,
        break_post,
        continue_,
        cb,
        cinvb,
        bind,
        transout,
        trans,
        systolic,
        fixstep_lc_one,
        fixN_lc_one,
        fixbrk_lc_one,
        calc_lc_two,
        comp_lc_two,

        fixstep_lc_one_ini,
        fixN_lc_one_ini,
        fixbrk_lc_one_ini,
        comp_lc_two_ini,

        lc_one,
        lc_one_ini,
        lc_two,
        lc_two_ini,
        brkgen
    };

    enum class LocalregFrom
    {
        alu,
        in1,
        null
    };

    enum class AluInFrom
    {
        inbuffer,
        lr,
        inport,
        first_loop,
        null
    };

    enum class InBufferFrom
    {
        alu,
        flr,
        in0,
        in1,
        in2,
        aluin1,
        in0in1
    };
    enum class CondMode
    {
        in2data,
        cond,
        cb,
        cinvb
    };
    enum class BufferMode
    {
        buffer,
        keep,
        lr,
        lr_out,
        lrffer
    };
    enum class PortValid
    {
        PEA,
        BUS,
    };
    enum class InputBufferBypass
    {
        inbuffer,
        bypass,
    };
    enum class OutputBufferBypass
    {
        outbuffer,
        bypass,
        lr,
        inbuffer,
        null
    };

    enum class FGMode
    {
        null,
        lbegin
    };

    enum class LSMode
    {
        load,
        store_data,
        store_addr,
        dummy,
        null,
        g2l,
        l2g,
        release_async,
        release,
        l2p,
        p2l,
        /** functional node */
        g2p,
        p2g,
        push,
        pop,
        enque,
        deque
    };

    enum class BufferSize
    {
        small,
        middle,
        large
    };

    enum class PortType
    {
        d32_v1_b1_l1_s1,
        v1_b1_l1_s1,
        d32_b1_v1,
        d32_v1,
        d32_b1_v1_t,
        d32_v1_t,
        d1_v1,
        d1_v1_t,
        bp,
        func_op,
        b1,
        null
    };

    enum class PortDirection
    {
        in,
        out,
        null
    };

    using WireType = PortType;
    using SegmentType = PortType;

    enum class WireNetConfigMode
    {
        cluster,
        way4,
        way8,
        way4hop1,
        way4hop2,
        row_full_connect,
        col_full_connect,
        pe_fifo_connect,
        pe_rt_connect,
        ls_rt_connect,
        ls_fifo_connect,
        full_connect,
        all_position_full_connect,
        multi_connect,
        bus_pe_connect,
        bus_bus_connect,
        null
    };

    enum class PortFunction
    {
        normal,
        router,
        ctrlb,
        func,
        bp,
        router_bp,
        null
    };

    enum class MappingMethod
    {
        simulate_anneal
    };
}