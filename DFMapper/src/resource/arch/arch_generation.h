#pragma once
#include "../../define/define.hpp"

namespace DFMpr
{
	class ArchGeneration
	{
	public:
		ArchGeneration(string addr_, int cnt_);
		~ArchGeneration() {};
		void archConfig();

	private:
		void blockConfig();
		void netConfig();

		void blockGeneration();
		void peTool();
		void lseTool();
		void nullTool();
		void blockConfigBegin();
		void blockConfigEnd();
		void netConfigBegin();
		void netConfigEnd();
		void configBegin();
		void configEnd();
		void sameType(string config_mode, string wire_type, string block_type, string port);
		void difType(string config_mode, string wire_type, string block_type, string port);

	private:
		string addr;
		int cnt;
		vector<vector<bool>> isNull;

	};
}

