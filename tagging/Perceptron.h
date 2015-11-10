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

#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <string>
#include <vector>
#include "Model.h"
#include "Sample.h"
#include "Decoder.h"

namespace Tagging
{
	class Perceptron
	{
	public:
		Perceptron(class Sample* sample, class Model* model, bool train = false, int round = 20, int beamSize = 8, int nGram = 2);
		~Perceptron();
		bool Train();
		bool Test(const std::string& result);
		bool Test(const std::vector<std::vector<std::string> >& tokens, std::vector<std::string>& result);
	private:
		class Sample  * m_sample;
		class Model   * m_model;
		class Decoder * m_decoder;
		int             m_round;
	private:
		bool Test(std::vector< std::vector<int> >* samples, std::vector<int>& result);
	};
}
#endif
