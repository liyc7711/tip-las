/********************************************************************
* Copyright (C) 2011 - 2015
* The Inistitute of Chinese Ethnic Information Technology ( www.nlit.edu.cn)
* Contact: Li Yachao,harry_lyc@foxmail.com or liyc7711@gmail.com
*
* Permission to use, copy, modify, and distribute this software for
* any non-commercial purpose is hereby granted without fee, provided
* that the above copyright notice appear in all copies and that both
* that copyright notice.
* It is provided "as is" without express or implied warranty.
*
* Version: 1.6
* Last update: 2015-10-23
*********************************************************************/
/********************************************************************
* Thanks to Wang Kun and Li Xiaoqing for the help of this project.
*
*
********************************************************************/

#include <iostream>
#include <string>
#include <vector>

#include "./utility/head.h"
#include "./tagging/Perceptron.h"
#include "Formatting.h"
using namespace utility_train;
using namespace utility;
using namespace Tagging;

namespace Tip
{
	class Las
	{
	public:
		Las();
		~Las();
		bool Train(const std::string& trainFile, const std::string& modelFile, const std::string& type, int iter = 30,int beamSize = 16, int nGram = 3);
		bool Test(const std::string& inFile, const std::string& outFile, const std::string& model_ws, const std::string& model_pos, const std::string& type);
		bool Development(const std::string& inFile, const std::string& model_ws = "ws.model", const std::string& model_pos = "pos.model", const std::string& type = "pos");
	private:
		std::string  flag_ws   = "ws";
		std::string  flag_pos  = "pos";
		std::string  flag_dev  = "dev";
		std::string  flag_all  = "all";
		std::string  delimiter = " ";
		std::string  templatesSet;
		double correct;
		double total;
		class Formatting*  format;
		class Perceptron*  tagging_ws;
		class Perceptron*  tagging_pos;
		bool WsTest(const  std::string& text, std::string& val);
		bool WsTest(const  std::string& text, std::vector<std::string>& words, bool withSeg = false);
		bool PosTest(const std::string& text, std::string& val);
		bool PosDevelopment(const std::string& text, std::string& val);

	};
}
