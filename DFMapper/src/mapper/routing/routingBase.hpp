#pragma once

#include "../../resource/app_graph/app_graph.h"
#include "../../resource/resource_graph/pr_res_graph.h"

namespace DFMpr
{
	struct RoutingResult
	{
		vector<AppNet> net;
		vector<vector<vector<uint>>> paths;
		double resourceRate = 0;
		double resourceWireRate = 0;
		double resourceSegmentRate = 0;
	};
	
}
