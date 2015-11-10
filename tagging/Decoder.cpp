#include "Decoder.h"

namespace Tagging
{
	Decoder::Decoder(class Model * model, bool mode, int pl, int pr, int beamSize, int nGram)
	{
		m_model      = model;
		m_mode       = mode;
		paddingLeft  = pl;
		paddingRight = pr;
		BeamSize     = beamSize;
		NGram        = nGram;
	}

	Decoder::~Decoder()
	{
		m_tokensMatrix = NULL;
	}

	void Decoder::SetBeamSize(int s)
	{
		BeamSize = s;
	}

	void Decoder::SetNGram(int s)
	{
		NGram = s;
	}

	bool Decoder::Decode(std::vector<std::vector<int> >* samples, std::vector<int>& tags, std::vector<int>* tagsGold)
	{
		/*******clear*********************/
		m_candsOld.clear();
		m_candsNew.clear();
		m_goldTags.clear();
		m_tokensMatrix = NULL;
		tags.clear();
		if (m_mode)
		{
			tagsGold->clear();
		}
		/*********************************/
		m_tokensMatrix = samples;
		Cand cand;
		cand.acc_score = 0;
		cand.taglist.resize(paddingLeft, 0);
		if (m_mode)
		{
			m_goldTags.resize(paddingLeft, 0);
		}
		m_candsOld.push_back(cand);
		int size = m_tokensMatrix->size();

		for (int i = paddingLeft; i < (size - paddingRight); i++)
		{
			m_pos = i;
			for (int j = 0; j < m_candsOld.size(); j++)
			{
				std::vector<Cand> tmp;
				ExpandStates(m_candsOld[j], tmp);
				AddNewStates(tmp);
			}
			sort(m_candsNew.begin(), m_candsNew.end(), greater<Cand>());
			m_candsOld.swap(m_candsNew);
			if (m_candsOld.size() > BeamSize)
			{
				m_candsOld.resize(BeamSize);
			}
			m_candsNew.resize(0);
			if (m_mode)
			{
				int len = m_tokensMatrix->at(i).size();
				m_goldTags.push_back(m_tokensMatrix->at(i).at(len - 1));
				bool track = true;
				for (int j = 0; j < m_candsOld.size(); j++)
				{
					if (m_candsOld[j].taglist == m_goldTags)
					{
						track = false;
						break;
					}
				}
				if (track)
				{
					tags = m_candsOld[0].taglist;
					tagsGold->assign(m_goldTags.begin(), m_goldTags.end());
					return false;
				}
			}
		}
		if (m_mode)
		{
			if (m_candsOld[0].taglist == m_goldTags)
			{
				return true;
			}
			tags = m_candsOld[0].taglist;
			tagsGold->assign(m_goldTags.begin(), m_goldTags.end());
			return false;
		}
		else
		{
			tags = m_candsOld[0].taglist;
			return true;
		}
	}

	bool Decoder::CheckHistory(const Cand & cand0, const Cand & cand1)
	{
		for (int i = 0; i < NGram; i++)
		{
			int index = m_pos - i;
			if (cand0.taglist[index] != cand1.taglist[index])
			{
				return false;
			}
		}
		return true;
	}

	void Decoder::ExpandStates(const Cand& cand, std::vector<Cand>& candStates)
	{
		candStates.clear();
		int token = m_tokensMatrix->at(m_pos).at(0);
		int lastTag = 0;
		if (cand.taglist.size() > 0)
		{
			lastTag = cand.taglist[cand.taglist.size() - 1];
		}
		std::vector<int> candTags;
		m_model->CandidateTags(candTags, token, lastTag);
		std::vector<int>::iterator it = candTags.begin();
		while (it != candTags.end())
		{
			Cand cand_n;
			cand_n.taglist = cand.taglist;
			cand_n.taglist.push_back((*it));
			std::vector<std::vector<int> > features;
			GenerateFeatures(features, cand_n.taglist, cand_n.taglist.size() - 1);
			double score     = m_model->FeaturesWeight(features);
			cand_n.acc_score = cand.acc_score + score;
			candStates.push_back(cand_n);
			it++;
		}
	}

	void Decoder::AddNewStates(const std::vector<Cand>& cands)
	{
		int size = cands.size();
		for (int i = 0; i < size; i++)
		{
			bool history = false;
			for (int j = 0; j < m_candsNew.size(); j++)
			{
				history = CheckHistory(cands[i], m_candsNew[j]);
				if (history)
				{
					if (cands[i].acc_score >  m_candsNew[j].acc_score)
					{
						m_candsNew[j].taglist = cands[i].taglist;
						m_candsNew[j].acc_score = cands[i].acc_score;
					}
					break;
				}
			}
			if (false == history)
			{
				m_candsNew.push_back(cands[i]);
			}
		}
	}

	void Decoder::GenerateFeatures(std::vector<std::vector<int> >& features, const std::vector<int>& tagList, int pos)
	{
		features.clear();
		std::vector<int> feature;
		int size = m_model->m_Templates.size();
		std::vector< std::vector <std::pair<int, int> > >::iterator it = m_model->m_Templates.begin();
		int id = -1;
		while (it != m_model->m_Templates.end())
		{
			id++;
			feature.clear();
			feature.push_back(id);
			std::vector <std::pair<int, int> >::iterator it1 = (*it).begin();
			while (it1 != (*it).end())
			{
				feature.push_back(m_tokensMatrix->at((*it1).first + pos).at((*it1).second));
				it1++;
			}
			feature.push_back(tagList[pos]);
			features.push_back(feature);
			it++;
		}
		if (m_model->Bigram())
		{
			feature.clear();
			feature.push_back(++id);
			feature.push_back(tagList[pos - 1]);
			feature.push_back(tagList[pos]);
			features.push_back(feature);
		}
		if (m_model->Trigram())
		{
			feature.clear();
			feature.push_back(++id);
			feature.push_back(tagList[pos - 2]);
			feature.push_back(tagList[pos - 1]);
			feature.push_back(tagList[pos]);
			features.push_back(feature);
		}
	}

}
