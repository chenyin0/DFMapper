#pragma once
#include "../define/define.hpp"

namespace DFMpr {
    enum class Op {
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
}