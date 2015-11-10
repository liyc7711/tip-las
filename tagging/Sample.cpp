#include "Sample.h"

namespace Tagging
{
	Sample::Sample()
	{
		LeftPadding = 2;
		RightPadding = 2;
	}

	Sample::Sample(std::vector<std::unordered_map<string, int> >& dict)
	{
		Dict.assign(dict.begin(), dict.end());
	}

	Sample::~Sample()
	{

	}

	Sample::Sample(int left, int right)
	{
		LeftPadding  = left;
		RightPadding = right;
		FieldSize = 0;
	}

	int Sample::GetSize()
	{
		return SamplesMatrix.size();
	}

	void Sample::Shuffle()
	{
		random_shuffle(SamplesMatrix.begin(), SamplesMatrix.end());
	}

	std::string Sample::GetTag(int tagid)
	{
		return  id2Tag[tagid];
	}

	std::vector<std::vector<int> >* Sample::GetSample(int index)
	{
		if (index < 0 || index >= SamplesMatrix.size())
		{
			return NULL;
		}
		else
		{
			return &SamplesMatrix[index];
		}
	}

	void Sample::Tokens2Id(const std::vector<std::vector<std::string> >& tokens, std::vector< std::vector<int> >* samples)
	{
		std::vector<int> defaultEvent;
		defaultEvent.resize(FieldSize, 0);
		int size = tokens.size();
		for (int i = 0; i < LeftPadding; i++)
		{//left padding for sample
			samples->push_back(defaultEvent);
		}
		for (int i = 0; i < size; i++)
		{
			std::vector<int> s;
			for (int j = 0; j < tokens[i].size(); j++)
			{
				std::unordered_map<string, int>::iterator it = Dict[j].find(tokens.at(i).at(j));
				if (it != Dict[j].end())
				{
					s.push_back(it->second);
				}
				else
				{
					s.push_back(-1);
				}
			}
			samples->push_back(s);
		}
		for (int i = 0; i < RightPadding; i++)
		{//right padding for sample
			samples->push_back(defaultEvent);
		}
	}

	bool Sample::Test(const std::string & testFile)
	{
		std::ifstream fin(testFile);
		if (!fin.is_open())
		{
			std::cout << "Read Test File " << testFile << " Error!!!" << std::endl;;
			return false;
		}
		std::vector< std::vector<int> > events;
		std::vector<int> defaultEvent;
		std::vector<std::string> fields;
		std::vector<std::string> samples;
		std::string line = "";
		int lineIndex    = 0;
		std::string seg  = "\t";
		while (getline(fin, line))
		{
			Utils::TrimLine(line);
			lineIndex++;
			if (line.empty())
			{
				for (int i = 0; i < RightPadding; i++)
				{//right padding for samples
					events.push_back(defaultEvent);
				}
				SamplesMatrix.push_back(events);
				Samples.push_back(samples);
				samples.clear();
				events.clear();
				for (int i = 0; i < LeftPadding; i++)
				{//left padding for samples
					events.push_back(defaultEvent);
				}
				continue;
			}
			fields.clear();
			Utils::Split(fields, line, seg);
			if (lineIndex == 1)
			{
				FieldSize = fields.size();
				defaultEvent.resize(FieldSize, 0);
				for (int i = 0; i < LeftPadding; i++)
				{//left padding for samples
					events.push_back(defaultEvent);
				}
			}
			std::vector<int>tokens;
			for (int i = 0; i < fields.size(); i++)
			{
				std::unordered_map<string, int>::iterator it = Dict[i].find(fields[i]);
				if (it != Dict[i].end())
				{
					tokens.push_back((*it).second);
				}
				else
				{
					tokens.push_back(-1);
				}
			}
			events.push_back(tokens);
			samples.push_back(line);
		}
		return true;
	}

	bool Sample::Train(const std::string & trainFile)
	{
		std::ifstream fin(trainFile);
		if (!fin.is_open())
		{
			std::cout << "Read Train File " << trainFile << " Error!!!" << std::endl;;
			return false;
		}
		std::vector< std::vector<int> > events;
		std::vector<int> defaultEvent;
		std::vector<std::string> fields;
		std::string line;
		int lineIndex   = 0;
		std::string seg = "\t";
		int lastTag     = 0;
		while (getline(fin, line))
		{
			lineIndex++;
			Utils::TrimLine(line);
			if (line.empty())
			{
				for (int i = 0; i < RightPadding; i++)
				{//right padding for samples
					events.push_back(defaultEvent);
				}
				SamplesMatrix.push_back(events);
				events.clear();
				for (int i = 0; i < LeftPadding; i++)
				{//left padding for samples
					events.push_back(defaultEvent);
				}
				lastTag = 0;
				continue;
			}
			fields.clear();
			Utils::Split(fields, line, seg);
			if (lineIndex == 1)
			{
				FieldSize = fields.size();
				if (FieldSize < 2)
				{
					std::cout << "File Broken At " << lineIndex << std::endl;
				}
				ids.resize(FieldSize, 1);
				Dict.resize(FieldSize);
				/*****************************/
				defaultEvent.resize(FieldSize, 0);
				for (int i = 0; i < LeftPadding; i++)
				{
					events.push_back(defaultEvent);
				}
				/*****************************/
			}
			if (FieldSize != fields.size())
			{
				std::cout << "File Broken At " << lineIndex << std::endl;
				return false;
			}
			std::vector<int> event_tmp;
			for (int i = 0; i < fields.size(); i++)
			{
				std::string str = fields[i];
				std::unordered_map<string, int>::iterator it = Dict[i].find(str);
				if (it != Dict[i].end())
				{
					event_tmp.push_back((*it).second);
				}
				else
				{
					Dict[i].insert(std::make_pair(str, ids.at(i)));
					event_tmp.push_back(ids.at(i));
					ids.at(i) += 1;
				}
			}
			tokensFrequent[event_tmp[0]] += 1;
			lastTagSet[lastTag].insert(event_tmp[FieldSize - 1]);
			tagSet.insert(event_tmp[FieldSize - 1]);
			tokenTagSet[event_tmp[0]].insert(event_tmp[FieldSize - 1]);
			lastTag = event_tmp[FieldSize - 1];
			events.push_back(event_tmp);
		}
		fin.close();
		AddEventsOver();
		return true;

	}

	bool Sample::AddEvents(const std::vector<std::vector<std::string> >& sample)
	{
		if (sample.size() == 0)
		{
			return true;
		}
		std::vector< std::vector<int> > events;
		std::vector<int> defaultEvent;
		int lastTag = 0;
		if (FieldSize < 2)
		{
			FieldSize = sample.at(0).size();
			ids.resize(FieldSize, 1);
			Dict.resize(FieldSize);
		}
		defaultEvent.resize(FieldSize, 0);
		for (int i = 0; i < LeftPadding; i++)
		{//left padding for samples
			events.push_back(defaultEvent);
		}
		int row = sample.size();
		for (int i = 0; i < row; i++)
		{
			int col = sample[i].size();
			std::vector<int> event_tmp;
			for (int j = 0; j < col; j++)
			{
				std::string str = sample[i][j];
				std::unordered_map<string, int>::iterator it = Dict[j].find(str);
				if (it != Dict[j].end())
				{
					event_tmp.push_back(it->second);
				}
				else
				{
					Dict[j].insert(std::make_pair(str, ids.at(j)));
					event_tmp.push_back(ids.at(j));
					ids.at(j) += 1;
				}
			}
			tokensFrequent[event_tmp[0]] += 1;
			lastTagSet[lastTag].insert(event_tmp[FieldSize - 1]);
			tagSet.insert(event_tmp[FieldSize - 1]);
			tokenTagSet[event_tmp[0]].insert(event_tmp[FieldSize - 1]);
			lastTag = event_tmp[FieldSize - 1];
			events.push_back(event_tmp);

		}
		for (int i = 0; i < RightPadding; i++)
		{//right padding for samples
			events.push_back(defaultEvent);
		}
		SamplesMatrix.push_back(events);
		//Samples.push_back(sample);
		return true;
	}

	bool  Sample::AddEventsOver()
	{
		std::set<int>::iterator it = tagSet.begin();
		while (it != tagSet.end())
		{
			tokenTagSet[-1].insert(*it);//for all tags
			lastTagSet[-1].insert(*it);
			it++;
		}
		return true;
	}
}