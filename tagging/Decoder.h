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
#ifndef DECODER_H
#define DECODER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <set>
#include <map>
#include "Model.h"

namespace Tagging
{
	class Decoder
	{
	public:
		Decoder(class Model * model, bool mode, int pl, int pr, int beamSize = 8, int nGram = 2);
		Decoder();
		~Decoder();
		bool Decode(std::vector<std::vector<int> > * samples, std::vector<int>& tags, std::vector<int>* tagsGold = NULL);
		void GenerateFeatures(std::vector<std::vector<int> >& features, const std::vector<int>& tagList, int pos);
		void SetBeamSize(int s);
		void SetNGram(int s);
	private:
		Model * m_model;
		bool  m_mode;
		int   BeamSize = 8;
		int   NGram    = 2;
		int   paddingLeft;
		int   paddingRight;
	private:

		void  ExpandStates(const Cand & cand, std::vector<Cand>& candStates);
		bool  CheckHistory(const Cand & cand0, const Cand & cand1);
		void  AddNewStates(const std::vector<Cand> &cands);

		std::vector<Cand> m_candsOld;
		std::vector<Cand> m_candsNew;
		std::vector<int>  m_goldTags;
		std::vector<std::vector<int> >* m_tokensMatrix;
		int  m_pos;

	};
}
#endif
