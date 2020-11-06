#pragma once

/**
 * 这个文件主要是定义布局布线方面的一些基本的数据结构
 *
 * 行 2019.9.5
 */

#include "../../util/cord.hpp"
#include "../../tinyxml2/tinyxml2.h"
#include "../../common/enum_definition.hpp"
#include "../../common/enum_converter.hpp"

namespace Simulator::Preprocess
{
	/**
	 * PortIndex 为一个辅助定位的结构体，用来定位端口
	 * 定位精确到 什么方向什么功能的哪一个
	 */
	struct PortIndex final
	{
		PortDirection direction;
		PortFunction function;
		uint index;

		bool operator== (const PortIndex& b_) const
		{
			return direction == b_.direction && function == b_.function && index == b_.index;
		}

		// input string = [in/out][normal/router/..][x]
		// eg: [in][normal][1]
		static auto stringAnalysis(string s_)->PortIndex
		{
			vector<string> tmp = Util::splitString(s_, "][");
			Util::deleteAllMark(tmp[0], "[");
			Util::deleteAllMark(tmp[2], "]");
			return PortIndex{
				PortDirectionConverter::toEnum(tmp[0]),
				PortFunctionConverter::toEnum(tmp[1]),
				static_cast<uint>(std::stoi(tmp[2]))
			};
		}

		explicit operator string() const
		{
			string result = "[" + PortDirectionConverter::toString(direction) + "]";
			result += "[" + PortFunctionConverter::toString(function) + "]";
			result += "[" + std::to_string(index) + "]";
			return result;
		}
	};

	/**
	 * PortPos 为一个辅助定位的结构体，用来定位端口
	 * 定位精确到 什么类型哪个坐标的什么方向什么功能的第几个
	 * PortPos相比PortIndex有更多的信息量
	 *
	 * 例如 PortPos{ PE, [3, 2], out, normal, 0}
	 * 就是坐标3，2位置的PE block的normal功能的输出端口的第0号端口
	 */
	struct PortPos final
	{
		BlockType block_type;
		Cord block_pos;
		PortDirection direction;
		PortFunction function;
		uint index;

		PortPos(BlockType block_type_, Cord block_pos_, PortDirection port_direction_, PortFunction port_function_, uint port_num_)
			: block_type(block_type_)
			, block_pos(std::move(block_pos_))
			, direction(port_direction_)
			, function(port_function_)
			, index(port_num_)
		{
		}

		PortPos(BlockType block_type_, Cord block_pos_, const PortIndex& port_index_)
			: block_type(block_type_)
			, block_pos(std::move(block_pos_))
			, direction(port_index_.direction)
			, function(port_index_.function)
			, index(port_index_.index)
		{
		}

		bool operator==(const PortPos& b_) const
		{
			return block_type == b_.block_type
				&& block_pos == b_.block_pos
				&& direction == b_.direction
				&& function == b_.function
				&& index == b_.index;
		}

		auto detachPortIndex() const->PortIndex
		{
			return PortIndex{ direction, function, index };
		}

		// input string = [pe/fg/..][x, x][in/out][normal/router/..][x]
		static auto stringAnalysis(string s_)->PortPos
		{
			Util::deleteAllMark(s_, " ");
			vector<string> tmp = Util::splitString(s_, "][");
			// block type
			Util::deleteAllMark(tmp[0], "[");
			BlockType block_type = BlockTypeConverter::toEnum(tmp[0]);
			// block pos
			tmp[1] = "[" + tmp[1] + "]";
			Cord block_pos = Cord::stringAnalysis(tmp[1]);
			// port direction
			PortDirection port_direction = PortDirectionConverter::toEnum(tmp[2]);
			// port function
			PortFunction port_function = PortFunctionConverter::toEnum(tmp[3]);
			// port num
			Util::deleteAllMark(tmp[4], "]");
			uint port_num = std::stoi(tmp[4]);
			// generate
			return PortPos{ block_type, block_pos, port_direction, port_function, port_num };
		}

		explicit operator string() const
		{
			string result = "[" + BlockTypeConverter::toString(block_type) + "]";
			result += static_cast<string>(block_pos);
			result += static_cast<string>(PortIndex{ direction, function, index });
			return result;
		}
	};

	/**
	 * ArchBlock定义了阵列中的一类block应该有的信息，注意是一类，不是一个
	 * normal_dict为normal功能的端口字典，router_dict同理
	 * position表示这类block在阵列中出现在哪些位置，true表示这个坐标有这个block
	 *
	 * 假设一个PE包含normal功能的端口3入1出，router功能的端口1入1出
	 * 那么这个结构体的信息如下
	 * normal_dict = { in : [d32_b1_v1, d32_b1_v1, d32_b1_v1],
	 *				   out : [d32_b1_v1] }
	 * router_dict = { in : [d32_b1_v1],
	 *				   out : [d32_b1_v1] }
	 * bp_dict = { in : [d32_b1_v1, d32_b1_v1],
	 *				   out : [d32_b1_v1, d32_b1_v1] }
	 * ctrlb_dict = { in : [d32_b1_v1],
	 *				   out : [d32_b1_v1] }
	 * func_dict = { in : [d32_b1_v1],
	 *				   out : [d32_b1_v1] }
	 * position = [ [ 0, 1, 1, 1, 1, 0],
	 *				[ 0, 1, 1, 1, 1, 0],
	 *				[ 0, 1, 1, 1, 1, 0],
	 *				[ 0, 1, 1, 1, 1, 0],
	 *				[ 0, 1, 1, 1, 1, 0] ]
	 * position就是整个阵列，1表示这个坐标有这个类型的block，0表示没有
	 */
	struct ArchBlock
	{
		map<PortDirection, vector<PortType>> normal_dict;
		vector<PortType> router_dict;
		map<PortDirection, vector<PortType>> ctrlb_dict;
		map<PortDirection, vector<PortType>> func_dict;
		map<PortDirection, vector<PortType>> bp_dict;
		vector<vector<Bool>> position;
	};


	/**
	 * WireGenerateBundle用来表示按照模式生成的线捆
	 * 一个WireGenerateBundle是一个pair，第一个元素是这捆线的线类型
	 * 第二个元素是一个vector，存线捆包含的所有线
	 * 每一根线是一个pair，第一个是源PortPos索引，第二个是端PortPos索引
	 * 表示从源PortPos到端PortPos的一个线
	 */
	using WireGenerateBundle = pair<WireType, vector<pair<PortPos, PortPos>>>;

	// 针对wire的纯虚类接口
	interface WireNetInterface
	{
		virtual vector<WireGenerateBundle> generate() const = 0;
		virtual ~WireNetInterface() = default;
	};

	// 泛型原型，用来特化
	template<WireNetConfigMode Mode>
	struct WireNet : public WireNetInterface
	{
	};

	template<>
	struct WireNet<WireNetConfigMode::way4> : public WireNetInterface
	{
		WireType wire_type;
		BlockType block_type;
		vector<PortIndex> in_ports;
		vector<PortIndex> out_ports;

		WireNet(WireType wire_type_, BlockType block_type_, vector<PortIndex> in_ports_, vector<PortIndex> out_ports_, const ArchBlock& arch_block_)
			: wire_type(wire_type_)
			, block_type(block_type_)
			, in_ports(std::move(in_ports_))
			, out_ports(std::move(out_ports_))
			, arch_block(arch_block_)
		{
		}

		auto generate() const -> vector<WireGenerateBundle> override
		{
			vector<pair<PortPos, PortPos>> temp;
			const vector<vector<Bool>>& position = arch_block.position;

			auto generate_link = [&](Cord cord1_, Cord cord2_) -> void
			{
				for (auto& every_in : in_ports)
				{
					for (auto& every_out : out_ports)
					{
						// from 1 out to 2 in
						temp.emplace_back(
							PortPos{ block_type, cord1_, every_out },
							PortPos{ block_type, cord2_, every_in }
						);
					}
				}
			};

			for (uint i = 0; i < position.size(); i++)
			{
				for (uint j = 0; j < position[i].size(); j++)
				{
					if (!position[i][j])
						continue;

					Cord central(i, j);
					// up
					if (static_cast<int>(i) - 2 >= 0)
						if (position[i - 2][j])
							generate_link(central, Cord{ i - 2, j });
					// right
					if (j + 1 < position[0].size())
						if (position[i][j + 2])
							generate_link(central, Cord{ i, j + 2 });
					// down
					if (i + 1 < position.size())
						if (position[i + 2][j])
							generate_link(central, Cord{ i + 2, j });
					// left
					if (static_cast<int>(j) - 2 >= 0)
						if (position[i][j - 2])
							generate_link(central, Cord{ i, j - 2 });
				}
			}

			return { std::make_pair(wire_type, temp) };
		}
	private:
		const ArchBlock& arch_block;
	};

	template<>
	struct WireNet<WireNetConfigMode::way8> : public WireNetInterface
	{
		WireType wire_type;
		BlockType block_type;
		vector<PortIndex> in_ports;
		vector<PortIndex> out_ports;

		WireNet(WireType wire_type_, BlockType block_type_, vector<PortIndex> in_ports_, vector<PortIndex> out_ports_, const ArchBlock& arch_block_)
			: wire_type(wire_type_)
			, block_type(block_type_)
			, in_ports(std::move(in_ports_))
			, out_ports(std::move(out_ports_))
			, arch_block(arch_block_)
		{
		}

		auto generate() const -> vector<WireGenerateBundle> override
		{
			vector<pair<PortPos, PortPos>> temp;
			const vector<vector<Bool>>& position = arch_block.position;

			auto generate_link = [&](Cord cord1_, Cord cord2_) -> void
			{
				for (auto& every_in : in_ports)
				{
					for (auto& every_out : out_ports)
					{
						// from 1 out to 2 in
						temp.emplace_back(
							PortPos{ block_type, cord1_, every_out },
							PortPos{ block_type, cord2_, every_in }
						);
					}
				}
			};

			for (uint i = 0; i < position.size(); i++)
			{
				for (uint j = 0; j < position[i].size(); j++)
				{
					if (!position[i][j])
						continue;

					Cord central{ i, j };
					// up
					if (static_cast<int>(i) - 1 >= 0)
						if (position[i - 1][j])
							generate_link(central, Cord{ i - 1, j });
					// up right
					if (static_cast<int>(i) - 1 >= 0 && j + 1 < position[0].size())
						if (position[i - 1][j + 1])
							generate_link(central, Cord{ i - 1, j + 1 });
					// right
					if (j + 1 < position[0].size())
						if (position[i][j + 1])
							generate_link(central, Cord{ i, j + 1 });
					// down right
					if (i + 1 < position.size() && j + 1 < position[0].size())
						if (position[i + 1][j + 1])
							generate_link(central, Cord{ i + 1, j + 1 });
					// down
					if (i + 1 < position.size())
						if (position[i + 1][j])
							generate_link(central, Cord{ i + 1, j });
					// down left
					if (i + 1 < position.size() && static_cast<int>(j) - 1 >= 0 && position[i][j - 1])
						if (position[i + 1][j - 1])
							generate_link(central, Cord{ i + 1, j - 1 });
					// left
					if (static_cast<int>(j) - 1 >= 0)
						if (position[i][j - 1])
							generate_link(central, Cord{ i, j - 1 });
					// up left
					if (static_cast<int>(i) - 1 >= 0 && static_cast<int>(j) - 1 >= 0)
						if (position[i - 1][j - 1])
							generate_link(central, Cord{ i - 1, j - 1 });
				}
			}

			return { std::make_pair(wire_type, temp) };
		}

	private:
		const ArchBlock& arch_block;
	};

	template<>
	struct WireNet<WireNetConfigMode::cluster> : public WireNetInterface
	{
		WireType wire_type;
		BlockType block_type;
		vector<PortIndex> in_ports;
		vector<PortIndex> out_ports;
		vector<PortIndex> bp_in_ports;
		vector<PortIndex> bp_out_ports;

		WireNet(WireType wire_type_, BlockType block_type_, vector<PortIndex> in_ports_, vector<PortIndex> out_ports_, const ArchBlock& arch_block_, vector<PortIndex> bp_in_ports_, vector<PortIndex> bp_out_ports_)
			: wire_type(wire_type_)
			, block_type(block_type_)
			, in_ports(std::move(in_ports_))
			, out_ports(std::move(out_ports_))
			, arch_block(arch_block_)
			, bp_in_ports(std::move(bp_in_ports_))
			, bp_out_ports(std::move(bp_out_ports_))
		{
		}

		auto generate() const -> vector<WireGenerateBundle> override
		{
			vector<pair<PortPos, PortPos>> temp1, temp2;
			const vector<vector<Bool>>& position = arch_block.position;

			auto generate_link = [&](Cord cord1_, Cord cord2_) -> void
			{
				for (auto& every_in : in_ports)
				{
					for (auto& every_out : out_ports)
					{
						// from 1 out to 2 in
						temp1.emplace_back(
							PortPos{ block_type, cord1_, every_out },
							PortPos{ block_type, cord2_, every_in }
						);
					}
				}

				for (int i = 0; i < bp_out_ports.size(); ++i)
				{
					temp2.emplace_back(
						PortPos{ block_type, cord2_, bp_out_ports[i] },
						PortPos{ block_type, cord1_, bp_in_ports[0] }
					);
					temp2.emplace_back(
						PortPos{ block_type, cord2_, bp_out_ports[i] },
						PortPos{ block_type, cord1_, bp_in_ports[1] }
					);
				}
			};

			auto generate_nn_link = [&](Cord cord1_, Cord cord2_) -> void
			{
				for (uint i = 0; i < in_ports.size() - 1; ++i)
				{
					for (uint j = 0; j < out_ports.size() - 1; ++j)
					{
						// from 1 out to 2 in
						temp1.emplace_back(
							PortPos{ block_type, cord1_, out_ports[j] },
							PortPos{ block_type, cord2_, in_ports[i] }
						);
					}
				}
				for (int i = 0; i < bp_out_ports.size() - 1; ++i)
				{
					temp2.emplace_back(
						PortPos{ block_type, cord2_, bp_out_ports[i] },
						PortPos{ block_type, cord1_, bp_in_ports[0] }
					);
				}
			};

			auto generate_nr_link = [&](Cord cord1_, Cord cord2_) -> void
			{
				for (uint j = 0; j < out_ports.size() - 1; ++j)
				{
					// from 1 out to 2 in
					temp1.emplace_back(
						PortPos{ block_type, cord1_, out_ports[j] },
						PortPos{ block_type, cord2_, in_ports.back() }
					);
				}
				temp2.emplace_back(
					PortPos{ block_type, cord2_, bp_out_ports.back() },
					PortPos{ block_type, cord1_, bp_in_ports[0] }
				);
			};

			auto generate_rn_link = [&](Cord cord1_, Cord cord2_) -> void
			{
				for (uint i = 0; i < in_ports.size() - 1; ++i)
				{
					// from 1 out to 2 in
					temp1.emplace_back(
						PortPos{ block_type, cord1_, out_ports.back() },
						PortPos{ block_type, cord2_, in_ports[i] }
					);
				}
				for (int i = 0; i < bp_out_ports.size() - 1; ++i)
				{
					temp2.emplace_back(
						PortPos{ block_type, cord2_, bp_out_ports[i] },
						PortPos{ block_type, cord1_, bp_in_ports[1] }
					);
				}
			};

			auto generate_rr_link = [&](Cord cord1_, Cord cord2_) -> void
			{
				// from 1 out to 2 in
				temp1.emplace_back(
					PortPos{ block_type, cord1_, out_ports.back() },
					PortPos{ block_type, cord2_, in_ports.back() }
				);
				temp2.emplace_back(
					PortPos{ block_type, cord2_, bp_out_ports.back() },
					PortPos{ block_type, cord1_, bp_in_ports[1] }
				);
			};

			if (block_type == BlockType::pe)
			{
				for (uint i = 2; i + 4 < position.size(); i = i + 4)
				{
					for (uint j = 2; j + 2 < position[i].size(); j = j + 8)
					{
						if (!position[i][j])
							continue;

						vector<Cord> v = { Cord{ i, j }, Cord{ i, j + 2 }, Cord{ i, j + 4 }, Cord{ i, j + 6 }, Cord{ i + 2, j }, Cord{ i + 2, j + 2 }, Cord{ i + 2, j + 4 }, Cord{ i + 2, j + 6 } };
						for (uint m = 0; m < v.size(); ++m)
						{
							for (uint n = 0; n < v.size(); ++n)
							{
								if (m != n)
								{
									generate_nn_link(v[m], v[n]);
								}
							}
						}
						/*
						//generate_nn_link(Cord{ i, j }, Cord{ i, j });
						generate_nn_link(Cord{ i, j }, Cord{ i, j + 2 });
						generate_nn_link(Cord{ i, j }, Cord{ i, j + 4 });
						generate_nn_link(Cord{ i, j }, Cord{ i, j + 6 });
						generate_nn_link(Cord{ i, j }, Cord{ i + 2, j });
						generate_nn_link(Cord{ i, j }, Cord{ i + 2, j + 2 });
						generate_nn_link(Cord{ i, j }, Cord{ i + 2, j + 4 });
						generate_nn_link(Cord{ i, j }, Cord{ i + 2, j + 6 });

						generate_nn_link(Cord{ i, j + 2 }, Cord{ i, j });
						//generate_nn_link(Cord{ i, j + 2 }, Cord{ i, j + 2 });
						generate_nn_link(Cord{ i, j + 2 }, Cord{ i, j + 4 });
						generate_nn_link(Cord{ i, j + 2 }, Cord{ i, j + 6 });
						generate_nn_link(Cord{ i, j + 2 }, Cord{ i + 2, j });
						generate_nn_link(Cord{ i, j + 2 }, Cord{ i + 2, j + 2 });
						generate_nn_link(Cord{ i, j + 2 }, Cord{ i + 2, j + 4 });
						generate_nn_link(Cord{ i, j + 2 }, Cord{ i + 2, j + 6 });

						generate_nn_link(Cord{ i, j + 4 }, Cord{ i, j });
						generate_nn_link(Cord{ i, j + 4 }, Cord{ i, j + 2 });
						//generate_nn_link(Cord{ i, j + 4 }, Cord{ i, j + 4 });
						generate_nn_link(Cord{ i, j + 4 }, Cord{ i, j + 6 });
						generate_nn_link(Cord{ i, j + 4 }, Cord{ i + 2, j });
						generate_nn_link(Cord{ i, j + 4 }, Cord{ i + 2, j + 2 });
						generate_nn_link(Cord{ i, j + 4 }, Cord{ i + 2, j + 4 });
						generate_nn_link(Cord{ i, j + 4 }, Cord{ i + 2, j + 6 });

						generate_nn_link(Cord{ i, j + 6 }, Cord{ i, j });
						generate_nn_link(Cord{ i, j + 6 }, Cord{ i, j + 2 });
						generate_nn_link(Cord{ i, j + 6 }, Cord{ i, j + 4 });
						//generate_nn_link(Cord{ i, j + 6 }, Cord{ i, j + 6 });
						generate_nn_link(Cord{ i, j + 6 }, Cord{ i + 2, j });
						generate_nn_link(Cord{ i, j + 6 }, Cord{ i + 2, j + 2 });
						generate_nn_link(Cord{ i, j + 6 }, Cord{ i + 2, j + 4 });
						generate_nn_link(Cord{ i, j + 6 }, Cord{ i + 2, j + 6 });

						generate_nn_link(Cord{ i + 2, j }, Cord{ i, j });
						generate_nn_link(Cord{ i + 2, j }, Cord{ i, j + 2 });
						generate_nn_link(Cord{ i + 2, j }, Cord{ i, j + 4 });
						generate_nn_link(Cord{ i + 2, j }, Cord{ i, j + 6 });
						//generate_nn_link(Cord{ i + 2, j }, Cord{ i + 2, j });
						generate_nn_link(Cord{ i + 2, j }, Cord{ i + 2, j + 2 });
						generate_nn_link(Cord{ i + 2, j }, Cord{ i + 2, j + 4 });
						generate_nn_link(Cord{ i + 2, j }, Cord{ i + 2, j + 6 });

						generate_nn_link(Cord{ i + 2, j + 2 }, Cord{ i, j });
						generate_nn_link(Cord{ i + 2, j + 2 }, Cord{ i, j + 2 });
						generate_nn_link(Cord{ i + 2, j + 2 }, Cord{ i, j + 4 });
						generate_nn_link(Cord{ i + 2, j + 2 }, Cord{ i, j + 6 });
						generate_nn_link(Cord{ i + 2, j + 2 }, Cord{ i + 2, j });
						//generate_nn_link(Cord{ i + 2, j + 2 }, Cord{ i + 2, j + 2 });
						generate_nn_link(Cord{ i + 2, j + 2 }, Cord{ i + 2, j + 4 });
						generate_nn_link(Cord{ i + 2, j + 2 }, Cord{ i + 2, j + 6 });

						generate_nn_link(Cord{ i + 2, j + 4 }, Cord{ i, j });
						generate_nn_link(Cord{ i + 2, j + 4 }, Cord{ i, j + 2 });
						generate_nn_link(Cord{ i + 2, j + 4 }, Cord{ i, j + 4 });
						generate_nn_link(Cord{ i + 2, j + 4 }, Cord{ i, j + 6 });
						generate_nn_link(Cord{ i + 2, j + 4 }, Cord{ i + 2, j });
						generate_nn_link(Cord{ i + 2, j + 4 }, Cord{ i + 2, j + 2 });
						//generate_nn_link(Cord{ i + 2, j + 4 }, Cord{ i + 2, j + 4 });
						generate_nn_link(Cord{ i + 2, j + 4 }, Cord{ i + 2, j + 6 });

						generate_nn_link(Cord{ i + 2, j + 6 }, Cord{ i, j });
						generate_nn_link(Cord{ i + 2, j + 6 }, Cord{ i, j + 2 });
						generate_nn_link(Cord{ i + 2, j + 6 }, Cord{ i, j + 4 });
						generate_nn_link(Cord{ i + 2, j + 6 }, Cord{ i, j + 6 });
						generate_nn_link(Cord{ i + 2, j + 6 }, Cord{ i + 2, j });
						generate_nn_link(Cord{ i + 2, j + 6 }, Cord{ i + 2, j + 2 });
						generate_nn_link(Cord{ i + 2, j + 6 }, Cord{ i + 2, j + 4 });
						//generate_nn_link(Cord{ i + 2, j + 6 }, Cord{ i + 2, j + 6 });
						*/
					}
				}

				for (uint i = 2; i + 2 < position.size(); i = i + 2)
				{
					for (uint j = 2; j + 2 < position[i].size(); j = j + 2)
					{
						if (!position[i][j])
							continue;

						Cord central(i, j);

						// up
						if (static_cast<int>(i) - 4 > 0)
						{
							generate_nr_link(central, Cord{ i - 4, j });
							generate_rr_link(central, Cord{ i - 4, j });
							generate_rn_link(central, Cord{ i - 4, j });
						}
						// right
						if (j + 10 < position[0].size())
						{
							generate_nr_link(central, Cord{ i, j + 8 });
							generate_rr_link(central, Cord{ i, j + 8 });
							generate_rn_link(central, Cord{ i, j + 8 });
						}
						// down
						if (i + 6 < position.size())
						{
							generate_nr_link(central, Cord{ i + 4, j });
							generate_rr_link(central, Cord{ i + 4, j });
							generate_rn_link(central, Cord{ i + 4, j });
						}
						// left
						if (static_cast<int>(j) - 8 > 0)
						{
							generate_nr_link(central, Cord{ i, j - 8 });
							generate_rr_link(central, Cord{ i, j - 8 });
							generate_rn_link(central, Cord{ i, j - 8 });
						}
					}
				}

			}
			else if (block_type == BlockType::ls)
			{
				for (uint i = 0; i < position.size(); i = i + 2)
				{
					for (uint j = 0; j < position[i].size(); j = j + 2)
					{
						if (!position[i][j])
							continue;
						/*
						Cord central{ i, j };
						// up
						if (static_cast<int>(i) - 2 >= 0)
							if (position[i - 2][j])
							{
								generate_nr_link(central, Cord{ i - 2, j });
								generate_rn_link(central, Cord{ i - 2, j });
								generate_rr_link(central, Cord{ i - 2, j });
							}
						// right
						if (j + 2 < position[0].size())
							if (position[i][j + 2])
							{
								generate_nr_link(central, Cord{ i, j + 2 });
								generate_rn_link(central, Cord{ i, j + 2 });
								generate_rr_link(central, Cord{ i, j + 2 });
							}
						// down
						if (i + 2 < position.size())
							if (position[i + 2][j])
							{
								generate_nr_link(central, Cord{ i + 2, j });
								generate_rn_link(central, Cord{ i + 2, j });
								generate_rr_link(central, Cord{ i + 2, j });
							}
						// left
						if (static_cast<int>(j) - 2 >= 0)
							if (position[i][j - 2])
							{
								generate_nr_link(central, Cord{ i, j - 2 });
								generate_rn_link(central, Cord{ i, j - 2 });
								generate_rr_link(central, Cord{ i, j - 2 });
							}
						
						// up up
						if (static_cast<int>(i) - 4 >= 0)
							if (position[i - 4][j])
							{
								generate_nr_link(central, Cord{ i - 4, j });
								generate_rn_link(central, Cord{ i - 4, j });
								generate_rr_link(central, Cord{ i - 4, j });
							}

						// right right
						if (j + 4 < position[0].size())
							if (position[i][j + 4])
							{
								generate_nr_link(central, Cord{ i, j + 4 });
								generate_rn_link(central, Cord{ i, j + 4 });
								generate_rr_link(central, Cord{ i, j + 4 });
							}

						// down down
						if (i + 4 < position.size())
							if (position[i + 4][j])
							{
								generate_nr_link(central, Cord{ i + 4, j });
								generate_rn_link(central, Cord{ i + 4, j });
								generate_rr_link(central, Cord{ i + 4, j });
							}

						// left left
						if (static_cast<int>(j) - 4 >= 0)
							if (position[i][j - 4])
							{
								generate_nr_link(central, Cord{ i, j - 4 });
								generate_rn_link(central, Cord{ i, j - 4 });
								generate_rr_link(central, Cord{ i, j - 4 });
							}
						*/
					}
				}
			}
			else if (block_type == BlockType::fifo)
			{
				for (uint i = 2; i < position.size() - 2; ++i)
				{
					for (uint j = 2; j < position[i].size() - 2; ++j)
					{
						if (!position[i][j])
							continue;

						Cord central{ i, j };
						// up up
						if (static_cast<int>(i) - 2 >= 0)
							if (position[i - 2][j])
							{
								generate_link(central, Cord{ i - 2, j });
							}
						// up right
						if (position[i - 1][j + 1])
						{
							generate_link(central, Cord{ i - 1, j + 1 });
						}
						// right right
						if (j + 2 < position[0].size())
							if (position[i][j + 2])
							{
								generate_link(central, Cord{ i, j + 2 });
							}
						// down right
						if (position[i + 1][j + 1])
						{
							generate_link(central, Cord{ i + 1, j + 1 });
						}
						// down down
						if (i + 2 < position.size())
							if (position[i + 2][j])
							{
								generate_link(central, Cord{ i + 2, j });
							}
						// down left
						if (position[i + 1][j - 1])
						{
							generate_link(central, Cord{ i + 1, j - 1 });
						}
						// left left
						if (static_cast<int>(j) - 2 >= 0)
							if (position[i][j - 2])
							{
								generate_link(central, Cord{ i, j - 2 });
							}
						// up left
						if (position[i - 1][j - 1])
						{
							generate_link(central, Cord{ i - 1, j - 1 });
						}
					}
				}
			}

			return { std::make_pair(wire_type, temp1), std::make_pair(WireType::b1, temp2) };
		}

	private:
		const ArchBlock& arch_block;
	};

	template<>
	struct WireNet<WireNetConfigMode::way4hop1> : public WireNetInterface
	{
		WireType wire_type;
		BlockType block_type;
		vector<PortIndex> in_ports;
		vector<PortIndex> out_ports;

		WireNet(WireType wire_type_, BlockType block_type_, vector<PortIndex> in_ports_, vector<PortIndex> out_ports_, const ArchBlock& arch_block_)
			: wire_type(wire_type_)
			, block_type(block_type_)
			, in_ports(std::move(in_ports_))
			, out_ports(std::move(out_ports_))
			, arch_block(arch_block_)
		{
		}

		auto generate() const -> vector<WireGenerateBundle> override
		{
			vector<pair<PortPos, PortPos>> temp;
			const vector<vector<Bool>>& position = arch_block.position;

			auto generate_link = [&](Cord cord1_, Cord cord2_) -> void
			{
				for (auto& every_in : in_ports)
				{
					for (auto& every_out : out_ports)
					{
						// from 1 out to 2 in
						temp.emplace_back(
							PortPos{ block_type, cord1_, every_out },
							PortPos{ block_type, cord2_, every_in }
						);
					}
				}
			};

			for (uint i = 0; i < position.size(); i++)
			{
				for (uint j = 0; j < position[i].size(); j++)
				{
					if (!position[i][j])
						continue;

					Cord central{ i, j };
					// up
					if (static_cast<int>(i) - 2 >= 0)
						if (position[i - 2][j])
							generate_link(central, Cord{ i - 2, j });
					// up up
					if (static_cast<int>(i) - 4 >= 0)
						if (position[i - 4][j])
							generate_link(central, Cord{ i - 4, j });
					// right
					if (j + 2 < position[0].size())
						if (position[i][j + 2])
							generate_link(central, Cord{ i, j + 2 });
					// right right
					if (j + 4 < position[0].size())
						if (position[i][j + 4])
							generate_link(central, Cord{ i, j + 4 });
					// down
					if (i + 2 < position.size())
						if (position[i + 2][j])
							generate_link(central, Cord{ i + 2, j });
					// down down
					if (i + 4 < position.size())
						if (position[i + 4][j])
							generate_link(central, Cord{ i + 4, j });
					// left
					if (static_cast<int>(j) - 2 >= 0)
						if (position[i][j - 2])
							generate_link(central, Cord{ i, j - 2 });
					// left left
					if (static_cast<int>(j) - 4 >= 0)
						if (position[i][j - 4])
							generate_link(central, Cord{ i, j - 4 });
				}
			}

			return { std::make_pair(wire_type, temp) };
		}

	private:
		const ArchBlock& arch_block;
	};

	template<>
	struct WireNet<WireNetConfigMode::way4hop2> : public WireNetInterface
	{
		WireType wire_type;
		BlockType block_type;
		vector<PortIndex> in_ports;
		vector<PortIndex> out_ports;

		WireNet(WireType wire_type_, BlockType block_type_, vector<PortIndex> in_ports_, vector<PortIndex> out_ports_, const ArchBlock& arch_block_)
			: wire_type(wire_type_)
			, block_type(block_type_)
			, in_ports(std::move(in_ports_))
			, out_ports(std::move(out_ports_))
			, arch_block(arch_block_)
		{
		}

		auto generate() const -> vector<WireGenerateBundle> override
		{
			vector<pair<PortPos, PortPos>> temp;
			const vector<vector<Bool>>& position = arch_block.position;

			auto generate_link = [&](Cord cord1_, Cord cord2_) -> void
			{
				for (auto& every_in : in_ports)
				{
					for (auto& every_out : out_ports)
					{
						// from 1 out to 2 in
						temp.emplace_back(
							PortPos{ block_type, cord1_, every_out },
							PortPos{ block_type, cord2_, every_in }
						);
					}
				}
			};

			for (uint i = 0; i < position.size(); i++)
			{
				for (uint j = 0; j < position[i].size(); j++)
				{
					if (!position[i][j])
						continue;

					Cord central{ i, j };
					// up
					if (static_cast<int>(i) - 2 >= 0)
						if (position[i - 2][j])
							generate_link(central, Cord{ i - 2, j });
					// up up
					if (static_cast<int>(i) - 4 >= 0)
						if (position[i - 4][j])
							generate_link(central, Cord{ i - 4, j });
					// up up up
					if (static_cast<int>(i) - 6 >= 0)
						if (position[i - 6][j])
							generate_link(central, Cord{ i - 6, j });
					// right
					if (j + 2 < position[0].size())
						if (position[i][j + 2])
							generate_link(central, Cord{ i, j + 2 });
					// right right
					if (j + 4 < position[0].size())
						if (position[i][j + 4])
							generate_link(central, Cord{ i, j + 4 });
					// right right right
					if (j + 6 < position[0].size())
						if (position[i][j + 6])
							generate_link(central, Cord{ i, j + 6 });
					// down
					if (i + 2 < position.size())
						if (position[i + 2][j])
							generate_link(central, Cord{ i + 2, j });
					// down down
					if (i + 4 < position.size())
						if (position[i + 4][j])
							generate_link(central, Cord{ i + 4, j });
					// down down down
					if (i + 6 < position.size())
						if (position[i + 6][j])
							generate_link(central, Cord{ i + 6, j });
					// left
					if (static_cast<int>(j) - 2 >= 0)
						if (position[i][j - 2])
							generate_link(central, Cord{ i, j - 2 });
					// left left
					if (static_cast<int>(j) - 4 >= 0)
						if (position[i][j - 4])
							generate_link(central, Cord{ i, j - 4 });
					// left left left
					if (static_cast<int>(j) - 6 >= 0)
						if (position[i][j - 6])
							generate_link(central, Cord{ i, j - 6 });
				}
			}

			return { std::make_pair(wire_type, temp) };
		}

	private:
		const ArchBlock& arch_block;
	};

	template<>
	struct WireNet<WireNetConfigMode::full_connect> : public WireNetInterface
	{
		WireType wire_type;
		BlockType block_type;
		vector<PortIndex> in_ports;
		vector<PortIndex> out_ports;
		vector<Cord> positions;

		WireNet(WireType wire_type_, BlockType block_type_, vector<PortIndex> in_ports_, vector<PortIndex> out_ports_, vector<Cord> positions_)
			: wire_type(wire_type_)
			, block_type(block_type_)
			, in_ports(std::move(in_ports_))
			, out_ports(std::move(out_ports_))
			, positions(std::move(positions_))
		{
		}

		auto generate() const -> vector<WireGenerateBundle> override
		{
			vector<pair<PortPos, PortPos>> temp;

			auto generate_link = [&](Cord source_cord_, Cord target_cord_) -> void
			{
				for (auto& every_in : in_ports)
					for (auto& every_out : out_ports)
						temp.emplace_back(
							PortPos{ block_type, source_cord_, every_out },
							PortPos{ block_type, target_cord_, every_in }
				);
			};

			for (uint i = 0; i < positions.size(); i++)
			{
				for (uint j = 0; j < positions.size(); j++)
				{
					if (i == j)
						continue;

					generate_link(positions[i], positions[j]);
				}
			}

			return { std::make_pair(wire_type, temp) };
		}
	};

	template<>
	struct WireNet<WireNetConfigMode::multi_connect> : public WireNetInterface
	{
		WireType wire_type;
		vector<PortPos> source_pos;
		vector<PortPos> target_pos;

		WireNet(WireType wire_type_, vector<PortPos> source_pos_, vector<PortPos> target_pos_)
			: wire_type(wire_type_)
			, source_pos(std::move(source_pos_))
			, target_pos(std::move(target_pos_))
		{
		}

		auto generate() const -> vector<WireGenerateBundle> override
		{
			vector<pair<PortPos, PortPos>> temp;

			for (auto& every_source_pos : source_pos)
				for (auto& every_target_pos : target_pos)
					temp.emplace_back(every_source_pos, every_target_pos);

			return { std::make_pair(wire_type, temp) };
		}
	};

	// TODO Segment 相关还没有实现
	struct SegmentNet
	{
		SegmentType segment_type;
		uint channel_num;
		vector<PortPos> source_pos;
		vector<PortPos> target_pos;

		SegmentNet(SegmentType wire_type_, uint channel_num_, vector<PortPos> source_pos_, vector<PortPos> target_pos_)
			: segment_type(wire_type_)
			, channel_num(channel_num_)
			, source_pos(std::move(source_pos_))
			, target_pos(std::move(target_pos_))
		{
		}
	};

}