#include "Model.h"
namespace Tagging
{
	Model::Model(const std::string&  model, int round, const std::string templatesFile, bool tagConstraint)
	{/*for train*/
		m_modelFile     = model;
		m_Mode          = true;//train flag
		m_Round         = round;
		m_tagConstraint = tagConstraint;
		m_Trigram       = false;
		m_Bigram        = false;
		if (templatesFile.empty() || templatesFile == "B")
		{
			TemplatesBaseline();
		}
		else if (templatesFile == "S")
		{
			TemplatesSyllables();
		}
		else
		{
			ParseTemplates(templatesFile);
		}
	}

	Model::Model()
	{/*for test*/
		m_Mode       = false;//test falg
		m_leftBound  = 0;
		m_rightBound = 0;
		m_Trigram    = false;
		m_Bigram     = false;
	}

	bool Model::LoadSamples(class Sample * sample)
	{
		/*for train*/
		if (m_tagConstraint)
		{
			tokensTagset.insert(sample->tokenTagSet.begin(), sample->tokenTagSet.end());
		}
		else
		{
			std::set<int>::iterator it = sample->tagSet.begin();
			while (it != sample->tagSet.end())
			{
				tokensTagset[-1].insert(*it);
				it++;
			}
		}
		lastTagset.insert(sample->lastTagSet.begin(), sample->lastTagSet.end());
		m_SampleSize = sample->GetSize();
		return true;
	}

	void Model::SetBeamSize(int s)
	{
		m_BeamSize = s;
	}
	void Model::SetNGram(int s)
	{
		m_NGram   = s;
	}
	bool Model::Bigram()
	{
		return m_Bigram;
	}
	bool Model::Trigram()
	{
		return m_Trigram;
	}
	bool Model::Mode()
	{
		return m_Mode;
	}
	int Model::FieldSize()
	{
		return m_FieldSize;
	}
	int Model::LeftBound()
	{
		return abs(m_leftBound);
	}
	int Model::RightBound()
	{
		return abs(m_rightBound);
	}

	bool Model::ReadModel(const std::string& model, class Sample* sample)
	{
		std::ifstream fin(model, ios::binary);
		if (!fin.is_open())
		{
			//std::cout << "Read Model " << model << " Error!!!" << std::endl;;
			return false;
		}
		char strBuf[32];
		memset(strBuf, 0, 32 * sizeof(char));
		fin.read((char*)strBuf, sizeof(char)* 4);
		fin.close();
		std::string cmd = strBuf;
		if (cmd == "#bin")
		{
			return ReadBinModel(model, sample);
		}
		else if (cmd == "#txt")
		{
			return ReadTxtModel(model, sample);
		}
		else
		{
			return false;
		}
	}

	bool Model::SaveModel(class Sample * sample, bool saveBin)
	{
		if (saveBin)
		{
			return SaveBinModel(m_modelFile, sample);
		}
		else
		{
			return SaveTxtModel(m_modelFile, sample);
		}
	}

	bool Model::ReadTxtModel(const std::string& model, class Sample* sample)
	{
		std::ifstream fin(model);
		if (!fin.is_open())
		{
			std::cout << "Read Model File " << model << " Error!!!" << std::endl;;
			return false;
		}
		std::string line;
		int col = -1;
		std::string seg = "\t";
		std::string flag = "#txt";
		while (getline(fin, line))
		{
			Utils::TrimLine(line);
			if (line.empty())
			{
				continue;
			}
			if (line == "#Tokens")
			{
				flag = "#Tokens";
				std::unordered_map<string, int> kvs;
				sample->Dict.push_back(kvs);
				col++;
				continue;
			}
			else if ((line == "#Tokens,Tagset") || (line == "#Tokens,LastTagset") || line == "#Weights" || line == "#Templates" || line == "#Fields")
			{
				flag = line;
				continue;
			}
			std::vector<std::string> tokens;
			Utils::Split(tokens, line, seg);
			if (flag == "#Tokens")
			{
				if (tokens.size() == 2)
					sample->Dict[col].insert(std::make_pair(tokens[0], stoi(tokens[1])));
			}
			else if (flag == "#Tokens,Tagset")
			{
				for (int i = 1; i < tokens.size(); i++)
				{
					tokensTagset[stoi(tokens[0])].insert(stoi(tokens[i]));
				}
			}
			else if (flag == "#Tokens,LastTagset")
			{
				for (int i = 1; i < tokens.size(); i++)
				{
					lastTagset[stoi(tokens[0])].insert(stoi(tokens[i]));
				}
			}
			else if (flag == "#Weights")
			{
				std::vector<int> f;
				for (int i = 0; i < (tokens.size() - 1); i++)
				{
					f.push_back(stoi(tokens[i]));
				}
				testParas[f] = stod(tokens[tokens.size() - 1]); //数据为空可能导致异常
			}
			else if (flag == "#Templates")
			{
				if (line == "B")
				{
					m_Bigram = true;
				}
				else if (line == "T")
				{
					m_Trigram = true;
				}
				else
				{
					std::vector<pair<int, int> > templates;
					vector<string> toks;
					std::string sep = "%";
					Utils::Split(toks, line, sep);
					for (size_t i = 0; i < toks.size(); i++)
					{
						string &tok = toks.at(i);
						tok = tok.substr(2, tok.size() - 3);
						size_t p = tok.find(',');
						int row = stoi(tok.substr(0, p));
						int col = stoi(tok.substr(p + 1));
						templates.push_back(make_pair(row, col));
					}
					m_Templates.push_back(templates);
				}
			}
			else if (flag == "#Fields")
			{
				m_FieldSize = stoi(line);
			}
		}
		std::unordered_map<string, int>::iterator it = sample->Dict[sample->Dict.size() - 1].begin();
		while (it != sample->Dict[sample->Dict.size() - 1].end())
		{
			sample->id2Tag.insert(std::make_pair((*it).second, (*it).first));
			it++;
		}
		return true;
	}

	bool Model::ReadBinModel(const std::string& model, class Sample* sample)
	{
		std::ifstream fin(model, ios::binary);
		if (!fin.is_open())
		{
			//std::cout << "Read Model File " << model << " Error!!!" << std::endl;;
			return false;
		}
		std::string line;
		std::string seg = "\t";
		int  myLength;
		char strBuf[256];
		int  size = 0;
		fin.read((char*)strBuf, 4); //skip the model flag
		fin.read((char*)&size, sizeof(int));
		for (int i = 0; i < size; i++)
		{
			memset(strBuf, 0, 256 * sizeof(char));
			fin.read((char*)&myLength, sizeof(int));
			fin.read((char*)strBuf, myLength);
			/***************************************************/
			std::vector<pair<int, int> > templates;
			vector<string> toks;
			std::string sep = "%";
			Utils::Split(toks, strBuf, sep);
			for (size_t j = 0; j < toks.size(); j++)
			{
				string &tok = toks.at(j);
				tok = tok.substr(2, tok.size() - 3);
				int p = tok.find(',');
				int row = stoi(tok.substr(0, p));
				int col = stoi(tok.substr(p + 1));
				templates.push_back(make_pair(row, col));
				if (row < m_leftBound)
				{
					m_leftBound = row;
				}
				if (row > m_rightBound)
				{
					m_rightBound = row;
				}
			}
			m_Templates.push_back(templates);
		}
		/*****left and right padding***********************/
		sample->LeftPadding  = abs(m_leftBound);
		sample->RightPadding = abs(m_rightBound);
		/**************************************************/
		fin.read((char*)&size, sizeof(int));
		if (size == 1)
		{
			m_Bigram = true;
		}
		else
		{
			m_Bigram = false;
		}
		fin.read((char*)&size, sizeof(int));
		if (size == 1)
		{
			m_Trigram = true;
		}
		else
		{
			m_Trigram = false;
		}
		/**********************************************************************/
		fin.read((char*)&size, sizeof(int));
		m_BeamSize = size;
		fin.read((char*)&size, sizeof(int));
		m_NGram    = size;
		/**********************************************************************/
		fin.read((char*)&size, sizeof(int));
		m_FieldSize = size;
		sample->FieldSize = m_FieldSize;
		/****************************************************/
		for (int idx = 0; idx < size; idx++)
		{
			std::unordered_map<string, int> kvs;
			sample->Dict.push_back(kvs);
			fin.read((char*)&myLength, sizeof(int));
			for (int i = 0; i < myLength; i++)
			{
				memset(strBuf, 0, 256 * sizeof(char));
				int buf_len = 0;
				fin.read((char*)&buf_len, sizeof(int));
				fin.read((char*)strBuf, buf_len);
				int v = 0;
				fin.read((char*)&v, sizeof(int));
				sample->Dict[idx].insert(std::make_pair(strBuf, v));
			}
		}
		/**************************************************/
		fin.read((char*)&size, sizeof(int));
		for (int idx = 0; idx < size; idx++)
		{
			int token = 0;
			fin.read((char*)&token, sizeof(int));
			fin.read((char*)&myLength, sizeof(int));
			for (int i = 0; i < myLength; i++)
			{
				int tt = 0;
				fin.read((char*)&tt, sizeof(int));
				tokensTagset[token].insert(tt);
			}
		}
		/**************************************************/
		fin.read((char*)&size, sizeof(int));
		for (int idx = 0; idx < size; idx++)
		{
			int token = 0;
			fin.read((char*)&token, sizeof(int));
			fin.read((char*)&myLength, sizeof(int));
			for (int i = 0; i < myLength; i++)
			{
				int tt = 0;
				fin.read((char*)&tt, sizeof(int));
				lastTagset[token].insert(tt);
			}
		}
		/**************************************************/
		fin.read((char*)&size, sizeof(int));
		for (int i = 0; i < size; i++)
		{
			fin.read((char*)&myLength, sizeof(int));
			vector<int> feature;
			feature.resize(myLength);
			fin.read((char*)&(feature.at(0)), sizeof(int)*myLength);
			double weight;
			fin.read((char*)&weight, sizeof(double));
			testParas[feature] = weight;
		}
		/*************************************************/
		std::unordered_map<string, int>::iterator it = sample->Dict[sample->Dict.size() - 1].begin();
		while (it != sample->Dict[sample->Dict.size() - 1].end())
		{
			sample->id2Tag.insert(std::make_pair((*it).second, (*it).first));
			it++;
		}
		//std::cout << "Load Model Over" << std::endl;
		return true;
	}
	bool Model::SaveBinModel(std::string model, class Sample * sample)
	{
		std::ofstream fout(model, ios::binary);
		int size = 0;
		if (!fout.is_open())
		{
			//std::cout << "Write Model File " << m_modelFile << " Error!!!" << std::endl;;
			return false;
		}
		size = m_Templates.size();
		std::string myLine = "#bin";
		fout.write(myLine.c_str(), myLine.length());
		myLine.clear();
		int myLength = 0;
		fout.write((char*)&size, sizeof(int));
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < m_Templates[i].size(); j++)
			{
				myLine += ("%x[" + Utils::ToString(m_Templates[i][j].first) + "," + Utils::ToString(m_Templates[i][j].second) + "]");
			}
			myLength = myLine.size();
			fout.write((char*)&myLength, sizeof(int));
			fout.write(myLine.c_str(), myLength);
			myLine.clear();
		}
		/**********************************************************************/
		if (Bigram())
		{
			size = 1;
		}
		else
		{
			size = 0;
		}
		fout.write((char*)&size, sizeof(int));
		if (Trigram())
		{
			size = 1;
		}
		else
		{
			size = 0;
		}
		fout.write((char*)&size, sizeof(int));
		/**********************************************************************/
		size = m_BeamSize;
		fout.write((char*)&size, sizeof(int));
		size = m_NGram;
		fout.write((char*)&size, sizeof(int));
		/**********************************************************************/
		size = sample->FieldSize;
		fout.write((char*)&size, sizeof(int));
		/**********************************************************/
		for (int i = 0; i < sample->Dict.size(); i++)
		{
			size = sample->Dict[i].size();
			fout.write((char*)&size, sizeof(int));
			std::unordered_map<string, int>::iterator it = sample->Dict[i].begin();
			while (it != sample->Dict[i].end())
			{
				size = it->first.size();
				fout.write((char*)&size, sizeof(int));
				fout.write(it->first.c_str(), size);
				size = it->second;
				fout.write((char*)&size, sizeof(int));
				it++;
			}
		}
		/**********************************************************/
		size = tokensTagset.size();
		fout.write((char*)&size, sizeof(int));
		std::unordered_map<int, std::set<int> >::iterator it_tags = tokensTagset.begin();
		while (it_tags != tokensTagset.end())
		{
			fout.write((char*)&it_tags->first, sizeof(int));
			size = it_tags->second.size();
			fout.write((char*)&size, sizeof(int));
			std::set<int>::iterator tmp = (*it_tags).second.begin();
			while (tmp != (*it_tags).second.end())
			{
				size = *tmp;
				fout.write((char*)(&size), sizeof(int));
				tmp++;
			}
			it_tags++;
		}
		/**********************************************************/
		size = lastTagset.size();
		fout.write((char*)&size, sizeof(int));
		it_tags = lastTagset.begin();
		while (it_tags != lastTagset.end())
		{
			fout.write((char*)&it_tags->first, sizeof(int));
			size = it_tags->second.size();
			fout.write((char*)&size, sizeof(int));
			std::set<int>::iterator tmp = (*it_tags).second.begin();
			while (tmp != (*it_tags).second.end())
			{
				size = *tmp;
				fout.write((char*)(&size), sizeof(int));
				tmp++;
			}
			it_tags++;
		}
		/**********************************************************/
		size = 0;
		for (const auto &feature : trainParas)
		{
			if (feature.second.acc_weight > -1e-10 && feature.second.acc_weight < 1e-10)
			{
				continue;
			}
			size++;
		}
		fout.write((char*)&size, sizeof(int));
		for (const auto &feature : trainParas)
		{
			if (feature.second.acc_weight > -1e-10 && feature.second.acc_weight < 1e-10)
			{
				continue;
			}
			int len = feature.first.size();
			fout.write((char*)&len, sizeof(int));
			fout.write((char*)&(feature.first.at(0)), sizeof(int)*feature.first.size());
			double weight = feature.second.acc_weight / (m_SampleSize*m_Round);
			fout.write((char*)&(weight), sizeof(double));
		}
		return true;
	}

	bool Model::SaveTxtModel(std::string model, class Sample * sample)
	{
		std::ofstream fout(model);
		int size = 0;
		if (!fout.is_open())
		{
			std::cout << "Write Model File " << m_modelFile << " Error!!!" << std::endl;;
			return false;
		}
		fout << "#txt" << std::endl;
		fout << "#Templates" << std::endl;
		size = m_Templates.size();
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < m_Templates[i].size(); j++)
			{
				fout << "%x[" << m_Templates[i][j].first << "," << m_Templates[i][j].second << "]";
				if (j == (m_Templates[i].size() - 1))
				{
					fout << std::endl;
				}
			}
		}
		if (Bigram())
		{
			fout << "B" << std::endl;
		}
		if (Trigram())
		{
			fout << "T" << std::endl;
		}
		fout << "#Fields" << std::endl << sample->FieldSize << std::endl;
		/**********************************************************/
		for (int i = 0; i < sample->Dict.size(); i++)
		{
			size = sample->Dict[i].size();
			fout << "#Tokens" << std::endl;
			std::unordered_map<string, int>::iterator it = sample->Dict[i].begin();
			while (it != sample->Dict[i].end())
			{
				fout << (*it).first << "\t" << (*it).second << std::endl;
				it++;
			}
		}
		fout << "#Tokens,Tagset" << std::endl;
		std::unordered_map<int, std::set<int> >::iterator it_tags = tokensTagset.begin();
		while (it_tags != tokensTagset.end())
		{
			fout << (*it_tags).first << "\t";
			std::set<int>::iterator tmp = (*it_tags).second.begin();
			while (tmp != (*it_tags).second.end())
			{
				fout << (*tmp) << "\t";
				tmp++;
			}
			fout << std::endl;
			it_tags++;
		}
		fout << "#Tokens,LastTagset" << std::endl;
		it_tags = lastTagset.begin();
		while (it_tags != lastTagset.end())
		{
			fout << (*it_tags).first << "\t";
			std::set<int>::iterator tmp = (*it_tags).second.begin();
			while (tmp != (*it_tags).second.end())
			{
				fout << (*tmp) << "\t";
				tmp++;
			}
			it_tags++;
			fout << std::endl;
		}
		/*****************************************************************************/
		fout << "#Weights" << std::endl << std::endl;
		std::unordered_map<std::vector<int>, WeightInfo, vechash>::iterator  it = trainParas.begin();
		while (it != trainParas.end())
		{
			if ((it->second.acc_weight > -1e10) && (it->second.acc_weight < 1e-10))
			{
				//continue;
			}
			else
			{
				std::vector<int>  vec = (*it).first;
				std::vector<int>::iterator it1 = vec.begin();
				while (it1 != vec.end())
				{
					fout << (*it1) << "\t";
					it1++;
				}
				fout << (it->second.acc_weight / (m_SampleSize * m_Round)) << std::endl;
			}
			it++;
		}
		return true;
	}

	bool Model::ParseTemplates(const std::string& tempStr, std::vector<std::pair<int, int> >& features)
	{
		features.clear();
		std::string seg = "%";
		std::vector<std::string> tokens;
		Utils::Split(tokens, tempStr, seg);
		if (tokens.size() <= 1)
		{
			return false;
		}
		for (int i = 1; i < tokens.size(); i++)
		{
			std::string token = tokens.at(i);
			token = token.substr(2, token.size() - 3);
			int s = token.find(',');
			int row = stoi(token.substr(0, s));
			int col = stoi(token.substr(s + 1));
			features.push_back(make_pair(row, col));
			if (row < m_leftBound)
			{
				m_leftBound = row;
			}
			if (row > m_rightBound)
			{
				m_rightBound = row;
			}
		}
		return true;
	}

	bool  Model::TemplatesBaseline()
	{
		std::vector<std::string> templates;
		templates.push_back("U00:%x[-2,0]");
		templates.push_back("U01:%x[-1,0]");
		templates.push_back("U02:%x[0,0]");
		templates.push_back("U03:%x[1,0]");
		templates.push_back("U04:%x[2,0]");
		templates.push_back("U05:%x[-2,0]/%x[-1,0]");
		templates.push_back("U06:%x[-1,0]/%x[0,0]");
		templates.push_back("U07:%x[0,0]/%x[1,0]");
		templates.push_back("U08:%x[1,0]/%x[2,0]");
		templates.push_back("U09:%x[-1,0]/%x[1,0]");
		templates.push_back("B");
		for (int i = 0; i < templates.size(); i++)
		{
			std::string& line = templates[i];
			if (line == "B")
			{
				m_Bigram = true;
			}
			else if (line == "T")
			{
				m_Trigram = true;
			}
			else
			{
				std::vector<std::pair<int, int> > features;
				ParseTemplates(line, features);
				m_Templates.push_back(features);
			}
		}
		return true;
	}
	bool  Model::TemplatesSyllables()
	{
		std::vector<std::string> templates;
		templates.push_back("U00:%x[-2,0]");
		templates.push_back("U01:%x[-1,0]");
		templates.push_back("U02:%x[0,0]");
		templates.push_back("U03:%x[1,0]");
		templates.push_back("U04:%x[2,0]");
		templates.push_back("U05:%x[-2,0]/%x[-1,0]");
		templates.push_back("U06:%x[-1,0]/%x[0,0]");
		templates.push_back("U07:%x[0,0]/%x[1,0]");
		templates.push_back("U08:%x[1,0]/%x[2,0]");
		templates.push_back("U09:%x[-1,0]/%x[1,0]");
		templates.push_back("U10:%x[0,1]");
		templates.push_back("U11:%x[0,2]");
		templates.push_back("U12:%x[0,3]");
		templates.push_back("U13:%x[0,4]");
		templates.push_back("B");
		for (int i = 0; i < templates.size(); i++)
		{
			std::string& line = templates[i];
			if (line == "B")
			{
				m_Bigram = true;
			}
			else if (line == "T")
			{
				m_Trigram = true;
			}
			else
			{
				std::vector<std::pair<int, int> > features;
				ParseTemplates(line, features);
				m_Templates.push_back(features);
			}
		}
		return true;
	}

	bool Model::ParseTemplates(const std::string& templates)
	{
		std::ifstream fin(templates);
		if (!fin.is_open())
		{
			std::cout << "Read Templates File " << templates << " Error!!!" << std::endl;;
			return false;
		}
		std::string line;
		while (getline(fin, line))
		{
			Utils::TrimLine(line);
			if ((line.size() == 0) || (line[0] == '#'))
			{
				continue;
			}
			if (line == "B")
			{
				m_Bigram = true;
			}
			else if (line == "T")
			{
				m_Trigram = true;
			}
			else
			{
				std::vector<std::pair<int, int> > features;
				ParseTemplates(line, features);
				m_Templates.push_back(features);
			}
		}
		fin.close();
		return true;
	}

	void Model::CandidateTags(std::vector<int>& candTags, int token, int lastTag)
	{
		std::set<int> tokenCandTags;
		std::set<int> lastCandTags;
		std::unordered_map<int, std::set<int> >::iterator it = tokensTagset.find(token);
		if (it != tokensTagset.end())
		{
			tokenCandTags = it->second;
		}
		else
		{
			tokenCandTags = tokensTagset[-1];
		}
		it = lastTagset.find(lastTag);
		if (it != lastTagset.end())
		{
			lastCandTags = it->second;
		}
		else
		{
			lastCandTags = lastTagset[-1];
		}
		candTags.clear();
		std::set_intersection(tokenCandTags.begin(), tokenCandTags.end(), lastCandTags.begin(), lastCandTags.end(), std::back_inserter(candTags));
	}

	double Model::FeaturesWeight(const std::vector< std::vector<int> >& features)
	{
		double weight = 0;
		int    size   = features.size();
		for (int i = 0; i < size; i++)
		{
			if (true == m_Mode)
			{//train
				weight += trainParas[features[i]].weight;
			}
			else
			{//test
				weight += testParas[features[i]];
			}
		}
		return weight;
	}

	void  Model::UpdateWeights(const int round, const int line)
	{
		std::unordered_map<std::vector<int>, WeightInfo, vechash>::iterator  it = trainParas.begin();
		while (it != trainParas.end())
		{
			it->second.acc_weight += it->second.weight * ((round - it->second.lastround)* m_SampleSize + line - it->second.lastline);
			it->second.lastline   = line;
			it->second.lastround  = round;
			it->second.freq++;
			it++;
		}
	}

	void Model::UpdateWeights(const std::vector<std::vector<int> > & features, const std::vector<std::vector<int> > & goldFeatures, const int round, const int line)
	{
		int size = features.size();
		for (int i = 0; i < size; i++)
		{
			std::unordered_map<std::vector<int>, WeightInfo, vechash>::iterator  it = trainParas.find(features[i]);
			if (it != trainParas.end())
			{
				it->second.acc_weight += it->second.weight * ((round - it->second.lastround)*m_SampleSize + line - it->second.lastline) - 1;
				it->second.weight     += -1;
				it->second.lastline   = line;
				it->second.lastround  = round;
				it->second.freq++;
			}
			else
			{
				WeightInfo info;
				info.acc_weight = -1;
				info.weight     = -1;
				info.lastline   = line;
				info.lastround  = round;
				info.freq       = 1;
				trainParas.insert(std::make_pair(features[i], info));
			}
		}
		size = goldFeatures.size();
		for (int i = 0; i < size; i++)
		{
			std::unordered_map<std::vector<int>, WeightInfo, vechash>::iterator  it = trainParas.find(goldFeatures[i]);
			if (it != trainParas.end())
			{
				it->second.acc_weight += it->second.weight * ((round - it->second.lastround)*m_SampleSize + line - it->second.lastline) + 1;
				it->second.weight     += 1;
				it->second.lastline   = line;
				it->second.lastround  = round;
				it->second.freq++;
			}
			else
			{
				WeightInfo info;
				info.acc_weight = 1;
				info.weight     = 1;
				info.lastline   = line;
				info.lastround  = round;
				info.freq       = 1;
				trainParas.insert(std::make_pair(goldFeatures[i], info));
			}
		}

	}

}
