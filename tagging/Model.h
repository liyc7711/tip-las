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
/*********************************************************************
* Model Version(1.1)
* 
*********************************************************************/
#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include <iterator>
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <memory.h>
#include "Sample.h"
#include "Utils.h"

using namespace std;

namespace Tagging
{
	struct Cand
	{
		vector <int> taglist;
		double acc_score;
		bool operator> (const Cand &cand_rhs) const
		{
			return (acc_score > cand_rhs.acc_score);
		}
	};

	struct WeightInfo
	{
		double weight;
		double acc_weight;
		int    lastline;
		int    lastround;
		int    freq;
	};

	struct vechash
	{
		size_t operator()(const vector<int>& v) const
		{
			size_t hash = 2166136261;
			for (auto &i : v)
			{
				hash *= 16777619;
				hash ^= i;
			}
			return hash;
		}
	};

	class Model
	{
	public:
		Model(const std::string&  model, int round, std::string templatesFile = "B", bool tagConstraint = false);
		Model(const std::string& model);
		Model();
		std::vector< std::vector <std::pair<int, int> > > m_Templates;
		bool   Trigram();
		bool   Bigram();
		bool   Mode();
		int    FieldSize();
		int    LeftBound();
		int    RightBound();
		bool   LoadSamples(class Sample * sample);
		double FeaturesWeight(const std::vector< std::vector<int> > & features);
		bool   SaveModel(class Sample * sample, bool saveBin = true);
		bool   ReadModel(const std::string& model, class Sample* sample);
		bool   ReadBinModel(const std::string& model, class Sample* sample);
		bool   SaveBinModel(std::string model, class Sample * sample);
		bool   ReadTxtModel(const std::string& model, class Sample* sample);
		bool   SaveTxtModel(std::string model, class Sample * sample);
		void   UpdateWeights(const int round, const int line);
		void   UpdateWeights(const std::vector<std::vector<int> > & features, const std::vector<std::vector<int> > & goldFeatures, const int round, const int line);
		void   CandidateTags(std::vector<int>& candTags, int token, int lastTag);
		void   SetBeamSize(int s);
		void   SetNGram(int s);
	private:
		bool m_Trigram;
		bool m_Bigram;
		bool m_Mode;
		bool m_tagConstraint;
		int  m_SampleSize;
		int  m_Round;
		int  m_FieldSize;
		int  m_leftBound;
		int  m_rightBound;
		int  m_BeamSize;
		int  m_NGram;
		std::string m_modelFile;
		std::unordered_map<std::vector<int>, WeightInfo, vechash> trainParas;
		std::unordered_map<std::vector<int>, double, vechash> testParas;
		std::unordered_map<int, std::set<int> > tokensTagset;
		std::unordered_map<int, std::set<int> >lastTagset;
	private:
		//bool ParseTemplates();
		bool TemplatesBaseline();
		bool TemplatesSyllables();
		bool ParseTemplates(const std::string & templates);
		bool ParseTemplates(const std::string& tempStr, std::vector<std::pair<int, int> >& features);

	};
}
#endif
