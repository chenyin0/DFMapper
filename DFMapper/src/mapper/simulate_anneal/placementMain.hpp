#pragma once

#include <sstream>
#include <iomanip>
#include "placementPara.hpp"
#include "../../util/Log.hpp"
#include "../../resource/preprocess/preprocess.h"

namespace DFMpr
{
	enum class PlacementStage
	{
		First,
		Second,
		Third,
		Fourth,
		Null
	};

	class SimulateAnnealEngine
	{
	public:
		SimulateAnnealEngine(const AppGraph& app_graph_, const ResourceGraph& resourceGraph_, const SimulateAnnealPara* para_, Log& log)
			: _appGraph(app_graph_)
			, _resourceGraph(resourceGraph_)
			, _para(para_)
			, _placement(_appGraph, _resourceGraph)
			, _log(log)
		{
		}

	public:
		auto goSimulatedAnneal() -> tuple<double, uint, vector<double>>
		{
			Timer timer{};
			timer.start();

			//_log.writeNoTime(" ");
			_log.write(" - ** Simulated Anneal Start ** - ");
#pragma region Init
			_log.writeNoTime(" - Initial Parameters - ");

			uint nodes_num = _para->nodesToPlace.size();

			randomGenerate();

			double init_cost = _para->cost(_placement);
			double standard_deviation = initAStandardDeviation();
			double temperature = 20 * standard_deviation;

			vector<double> alpha_count(204, 0);
			// 有些cost函数产生0的cost，直接返回当前布局，认为退火完成
			if (temperature == 0)
			{
				string usingTime = timer.getCountTime();
				usingTime.pop_back();
				usingTime.pop_back();
				return std::make_tuple(0, stoi(usingTime), alpha_count);
			}

			int moves_per_t = static_cast<int>(10 * std::pow(_para->nodesToPlace.size(), 4.0 / 3.0));
			//cout << _para->netsNum << endl;

			string paraStr = " - temperature = " + std::to_string(temperature) + "  movers per T = " + std::to_string(moves_per_t);
			_log.writeNoTime(paraStr);
#pragma endregion 

#pragma region Assist Function
			auto is_over = [&](double cost_) -> bool
			{
				double limit = _para->epsilon * (cost_ / _para->netsNum);
				return temperature < limit;
			};
#pragma endregion 

#pragma region Main Loop
			double cost = init_cost;

			const vector<AppNode>& app_nodes = _appGraph.getNodes();
			printf(" - ** Placement Started ** - \n");

			int iteration_count = 0;
			while (!is_over(cost))
			{
				PlacementBoard temp_place(_placement);
				int accept_count = 0;

				vector<double> cost_list;
				for (int i = 0; i < moves_per_t; i++)
				{
					uint node_index = _para->nodesToPlace[Util::uRandom(0, nodes_num - 1)];
					temp_place.swapOnce(node_index);
					//temp_place.anotherSwapOnce(node_index);

					double best_cost = _para->cost(temp_place);

					if (best_cost < cost)
					{
						accept_count++;
						std::swap(_placement, temp_place);
						std::swap(cost, best_cost);
						cost_list.push_back(best_cost);
					}
					else
					{
						int random_number = Util::uRandom(1, 100);
						int accept_threshold = static_cast<int>(100 * std::exp(-std::abs(cost - best_cost) / temperature));

						if (random_number < accept_threshold)
						{
							accept_count++;
							std::swap(_placement, temp_place);
							std::swap(cost, best_cost);
							cost_list.push_back(best_cost);
						}
					}
				}

				double alpha = static_cast<double>(accept_count) / moves_per_t;

#if PRINT_DETAIL_FILE || PRINT_DETAIL_CONSOLE
				string temp = "T = " + std::to_string(temperature) + ", Cost = " + std::to_string(cost) + ", Alpha = " + std::to_string(alpha);
#endif

#if PRINT_DETAIL_FILE
				//_log.writeNoTime(temp);
#endif
#if PRINT_DETAIL_CONSOLE
				printf(string(temp + "\n").c_str());
#endif

				temperature = _para->temperatureDecay(temperature, Util::standardDeviation(cost_list), iteration_count);
				alpha_count[iteration_count] += alpha;
				if (iteration_count >= 22 && iteration_count < 97)
					alpha_count[201] += alpha;
				iteration_count++;
			}

			vector<vector<uint>> all_reorder = _placement.getReorderAll();
			for (auto reorder : all_reorder)
			{
				vector<uint> best_order = reorder;
				do {
					_placement.reorder(reorder);
					double temp_cost = _para->cost(_placement);
					if (temp_cost < cost)
					{
						cost = temp_cost;
						best_order = reorder;
					}
				} while (next_permutation(reorder.begin() + 1, reorder.begin() + reorder.size()));
				_placement.reorder(best_order);
			}

			alpha_count[200] = static_cast<double>(iteration_count);
			printf(" - ** Placement Finished ** - \n");
			string usingTime = timer.getCountTime();
			_log.write(" - ** Simulated Anneal Finished ** - ");
			_log.writeNoTime(" - ** -- Spend Time = " + usingTime + " ** - ");
			vector<string> resultVec = getResult();
			_log.writeNoTime("  ");
			for (auto& s : resultVec)
				_log.writeNoTime(s);
			_log.writeNoTime("  ");

			usingTime.pop_back();
			usingTime.pop_back();
			return std::make_tuple(cost, stoi(usingTime), alpha_count);
#pragma endregion 
		}

		PlacementBoard getPlacementBoard() const
		{
			return _placement;
		}

		void loadPlacement(PlacementBoard&& b)
		{
			_placement = std::move(b);
		}

	private:
		auto randomGenerate() -> void
		{

			_placement.clear(_para->nodesToPlace);
			auto [cluster_pe, other_block] = _placement.initClusters(_para->nodesToPlace);
			const vector<AppNode>& app_nodes = _appGraph.getNodes();
			for (auto node_index : cluster_pe)
				_placement.randomPutClusterNode(node_index);

			for (auto node_index : other_block)
			{
				BlockType node_type = app_nodes[node_index].type;
				_placement.randomPutNode(node_type, node_index);
			}
			/*
			_placement.clear(_para->nodesToPlace);
			const vector<AppNode>& app_nodes = _appGraph.getNodes();
			for (auto node_index : _para->nodesToPlace)
			{
				BlockType node_type = app_nodes[node_index].type;
				_placement.randomPutNode(node_type, node_index);
			}
			*/
		}

		auto initAStandardDeviation() const -> double
		{
			uint node_num = _para->nodesToPlace.size();
			if (node_num == 0)
				return 0.0;

			vector<double> cost_list;
			PlacementBoard temp_placement(_placement);

			for (uint i = 0; i < node_num; i++)
			{
				uint node_index = _para->nodesToPlace[Util::uRandom(0, node_num - 1)];
				temp_placement.swapOnce(node_index);
				//temp_placement.anotherSwapOnce(node_index);
				cost_list.push_back(_para->cost(temp_placement));
			}

			return Util::standardDeviation(cost_list);
		}

		auto getResult() const -> vector<string>
		{
			const vector<AppNode>& appNodes = _appGraph.getNodes();
			vector<vector<uint>> result = _placement.getBoard();
			vector<string> strVec;
			for (uint i = 0; i < result.size(); i++)
			{
				std::stringstream ss;
				for (uint j = 0; j < result[0].size(); j++)
				{
					uint nodeIndex = result[i][j];
					if (nodeIndex == UINT_MAX)
						ss << std::setw(7) << "***";
					else
					{
						string temp = BlockTypeConverter::toString(appNodes[nodeIndex].type) + std::to_string(appNodes[nodeIndex].type_num);
						ss << std::setw(7) << temp;
					}
				}
				strVec.push_back(ss.str());
			}
			return strVec;
		}

	private:
		const AppGraph& _appGraph;
		const ResourceGraph& _resourceGraph;
		const SimulateAnnealPara* _para;
		PlacementBoard _placement;
		Log& _log;

	};

	enum class PlacementType
	{
		TwoStages
	};

	template <PlacementType Type>
	class Placement;

	template<>
	class Placement<PlacementType::TwoStages>
	{
	public:
		Placement(const AppGraph& appGraph_, const ResourceGraph& resourceGraph_, Log& log)
			: _appGraph(appGraph_)
			, _resourceGraph(resourceGraph_)
			, _paraStage1Ptr(nullptr)
			, _paraStage2Ptr(nullptr)
			//, _result(appGraph_, resourceGraph_)
			, _log(log)
		{
			devideNodes();
		}

		~Placement()
		{
			if (_paraStage1Ptr != nullptr)
				delete _paraStage1Ptr;
			if (_paraStage2Ptr != nullptr)
				delete _paraStage2Ptr;
		}

	public:
		void setPara(PlacementStage stage_, int pattern_, uint inner_number_, double epsilon_, uint bypassLimit_, CostFunType costFunType_, WireNetConfigMode wireConnectionMode_)
		{
			if (stage_ == PlacementStage::First)
			{
				if (_paraStage1Ptr != nullptr)
					delete _paraStage1Ptr;

				vector<uint> nodesToPlace = nodes[0];

				uint netsNum = 0;
				uint index = 0;
				const vector<AppNode>& app_nodes = _appGraph.getNodes();
				for_each(begin(app_nodes), end(app_nodes), [&](const AppNode& app_node_)
				{
					if (Util::findIndex(nodesToPlace, index) != UINT_MAX)
					{
						for (auto& link : app_node_.next_node)
						{
							uint next_index = link.target_node_vec_index;
							if (Util::findIndex(nodesToPlace, next_index) != UINT_MAX)
							{
								netsNum++;
							}
						}
					}
					index++;
				});

				_paraStage1Ptr = new SimulateAnnealPara(_appGraph, _resourceGraph, inner_number_, epsilon_, bypassLimit_, costFunType_, wireConnectionMode_, nodesToPlace, netsNum);
			}
			else if (stage_ == PlacementStage::Second)
			{
				if (_paraStage2Ptr != nullptr)
					delete _paraStage2Ptr;

				vector<uint> nodesToPlace = nodes[1];

				uint netsNum = 0;
				uint index = 0;
				const vector<AppNode>& app_nodes = _appGraph.getNodes();
				for_each(begin(app_nodes), end(app_nodes), [&](const AppNode& app_node_)
				{
					netsNum += app_node_.next_node.size();
					index++;
				});

				_paraStage2Ptr = new SimulateAnnealPara(_appGraph, _resourceGraph, inner_number_, epsilon_, bypassLimit_, costFunType_, wireConnectionMode_, nodesToPlace, netsNum);
			}
		}

		vector<Cord> getPlacementResult(const PlacementBoard _result) const
		{
			return _result.generateVectorCord();
		}

		void savePlacementResult(const string& addr_, const PlacementBoard _result) const
		{
			//vector<Cord> all_result = getManualPlacementResult(_result);
			vector<Cord> all_result = getPlacementResult(_result);

			const vector<AppNode>& nodes = _appGraph.getNodes();

			const char* declaration = R"(<?xml version="1.0" encoding="UTF-8" standalone="no"?>)";
			XMLDocument doc;
			doc.Parse(declaration);

			XMLElement* root = doc.NewElement("PlacementResult");
			doc.InsertEndChild(root);

			for (uint i = 0; i < all_result.size(); i++)
			{
				BlockType type = nodes[i].type;
				uint index = nodes[i].type_num;
				Cord cord = all_result[i];

				XMLElement* node_xml = doc.NewElement("node");
				node_xml->SetAttribute("type", BlockTypeConverter::toString(type).c_str());
				node_xml->SetAttribute("num", std::to_string(index).c_str());
				node_xml->SetAttribute("placement", static_cast<string>(cord).c_str());

				root->InsertEndChild(node_xml);
			}

			XMLElement* array_xml = doc.NewElement("array");
			root->InsertEndChild(array_xml);

			uint row_size = Para::getInstance()->getArchPara().row_num;
			uint col_size = Para::getInstance()->getArchPara().col_num;
			const vector<vector<Bool>>& block_info = Para::getInstance()->getArchPara().arch_block_type.find(BlockType::null)->second.position;

			for (uint row = 0; row < row_size; row++)
			{
				string row_str;
				for (uint col = 0; col < col_size; col++)
				{
					Cord cord{ row, col };
					uint node_num = Util::findIndex(all_result, cord);
					string temp;
					if (node_num != UINT_MAX)
					{
						BlockType type = nodes[node_num].type;
						uint index = nodes[node_num].type_num;
						temp = BlockTypeConverter::toString(type) + std::to_string(index);
						while (temp.size() < 6)
							temp += "-";
						temp += " ";
					}
					else if (block_info[row][col] == true)
					{
						temp = "      ";
						temp += " ";
					}
					else
					{
						temp = "******";
						temp += " ";
					}
					row_str += temp;
				}
				row_str.pop_back();

				XMLElement* row_xml = doc.NewElement("row");
				row_xml->SetText(row_str.c_str());
				array_xml->InsertEndChild(row_xml);
			}

			doc.SaveFile(addr_.c_str());
		}

		tuple<PlacementBoard, double, vector<double>> goPlacement() const
		{
			if (_paraStage1Ptr == nullptr || _paraStage2Ptr == nullptr)
				DEBUG_ASSERT(false);

			SimulateAnnealEngine e1(_appGraph, _resourceGraph, _paraStage1Ptr, _log);
			//uint time1 = e1.goSimulatedAnneal(PlacementStage::First);
			auto [cost1, time1, alpha_count1] = e1.goSimulatedAnneal();
			PlacementBoard tempBoard1 = e1.getPlacementBoard();

#ifdef DEBUG_CONSOLE
			string s1 = " - Stage 1 Finished. Spend Time = " + std::to_string(time1) + "ms\n";
			printf("%s", s1.c_str());
#endif

			SimulateAnnealEngine e2(_appGraph, _resourceGraph, _paraStage2Ptr, _log);
			e2.loadPlacement(std::move(tempBoard1));
			//uint time2 = e2.goSimulatedAnneal(PlacementStage::Second);
			auto [cost2, time2, alpha_count2] = e2.goSimulatedAnneal();
			PlacementBoard tempBoard2 = e2.getPlacementBoard();

#ifdef DEBUG_CONSOLE
			string s2 = " - Stage 2 Finished. Spend Time = " + std::to_string(time2) + "ms\n";
			printf("%s", s2.c_str());
#endif
			vector<double> alpha_count(204, 0);
			for (uint i = 0; i < alpha_count.size(); ++i)
			{
				alpha_count[i] = alpha_count1[i] + alpha_count2[i];
			}
			return std::make_tuple(e2.getPlacementBoard(), cost2, alpha_count);
		}

		void devideNodes()
		{
			const vector<AppNode>& app_nodes = _appGraph.getNodes();
			vector<uint> nodes_pe, nodes_ls;
			uint index = 0;
			for_each(begin(app_nodes), end(app_nodes), [&](const AppNode& app_node_)
			{
				BlockType type = app_node_.type;
				if (type == BlockType::pe)
					nodes_pe.push_back(index);
				else
					nodes_ls.push_back(index);

				index++;
			});

			nodes.push_back(nodes_pe);
			nodes.push_back(nodes_ls);
		}

	private:
		const AppGraph& _appGraph;
		const ResourceGraph& _resourceGraph;
		SimulateAnnealPara* _paraStage1Ptr;
		SimulateAnnealPara* _paraStage2Ptr;
		vector<vector<uint>> nodes;
		Log& _log;
	};
}