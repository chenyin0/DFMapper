#pragma once
#include "../define/define.hpp"
#include "../common/enum_definition.hpp"
#include "../resource/dfg.h"

namespace DFMpr {
    class Logger {
    public:
        Logger(string fileName);
        ~Logger();
        static std::ofstream& getOutputFile();

        std::ofstream& getFile()
        {
            if (debug_mode != LogMode::Turn_off)
            {
                return _output_file;
            }
            else
            {
                return _null_file;
            }
        }

        static void printDfg(const Dfg& dfg);
        static void printMaxNodeDelay(const Dfg& dfg);
        static void printNormDelayDistribution(const Dfg& dfg);
        static void printDelayDistribution(const Dfg& dfg);
        static void printDelayDistIntervalNorm(const Dfg& dfg);
        static void printNodeLevelDistribution(const Dfg& dfg);

        LogMode debug_mode = LogMode::Print_detail;

    private:
        static std::ofstream _output_file;
        static std::ofstream _null_file;  // Used in Print_off, not print to log_file
    };
}