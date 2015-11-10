/********************************************************************
* Copyright (C) 2011 - 2015   
* The Inistitute of Chinese Ethnic Information Technology ( www.nlit.edu.cn)
* Contact: Li Yachao, harry_lyc@foxmail.com or liyc7711@gmail.com 
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
#ifndef SAMPLE_H
#define SAMPLE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <map>
#include <algorithm>
#include <time.h>
#include "Utils.h"

using namespace std;

namespace Tagging
{
	class Sample
	{
	public:
		Sample(std::vector<std::unordered_map<string, int> >& dict);
		Sample();
		Sample(int left, int right);
		~Sample();
		bool Train(const std::string & trainFile);
		bool AddEvents(const std::vector<std::vector<std::string> >& sample);
		bool AddEventsOver();
		bool Test(const std::string & testFile);
		std::vector<std::vector<int> >* GetSample(int index);
		void Tokens2Id(const std::vector<std::vector<std::string> >& tokens, std::vector< std::vector<int> >* samples);
		std::string GetTag(int tagid);
		int  GetSize();
		void Shuffle();
		std::string GetSampleTag(int index);
		std::vector<std::vector< std::vector<int> > > SamplesMatrix;
		std::vector<std::vector<std::string> > Samples;
		std::vector<std::unordered_map<string, int> > Dict;
		std::unordered_map<int, std::set<int> > lastTagSet;
		std::unordered_map<int, std::set<int> > tokenTagSet;
		std::set<int> tagSet;
		std::unordered_map<int, std::string> id2Tag;
		int  FieldSize;
		int  LeftPadding;
		int  RightPadding;
	private:
		std::vector<int> ids;
		std::unordered_map<int, int> tokensFrequent;
	};
}
#endif
