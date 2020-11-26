#include "placementMain.hpp"
#include "../../define/define.hpp"

namespace DFMpr
{
	class PlacementPattern
	{
	public:
		PlacementPattern(const AppGraph& app_graph_, const ResourceGraph& resource_graph_, Log& log_)
			: _appGraph(app_graph_)
			, _resourceGraph(resource_graph_)
			, _log(log_)
		{
		}

		tuple <PlacementBoard, double, vector<double>> setPlacement()
		{
			int pattern = 2;
			Placement<PlacementType::TwoStages> placement(_appGraph, _resourceGraph, _log);
			placement.setPara(PlacementStage::First, pattern, innerNumber, epsilon, bypassLimit, CostFunType::Combination, WireNetConfigMode::cluster);
			placement.setPara(PlacementStage::Second, pattern, innerNumber, epsilon, bypassLimit, CostFunType::Combination, WireNetConfigMode::cluster);
			auto [placementBoard, cost, alpha_count] = placement.goPlacement();
			string placement_addr = GlobalPara::getInstance()->getOutputAddr(OutputAddr::placement_result_xml_addr);
			placement.savePlacementResult(placement_addr, placementBoard);
			return std::make_tuple(placementBoard, cost, alpha_count);
		}

	private:
		const AppGraph& _appGraph;
		const ResourceGraph& _resourceGraph;
		Log& _log;
		// 自动化阶段布局 - 设置参数
		uint innerNumber = 20;
		double epsilon = 0.0005;
		uint bypassLimit = 3;
	};
}