#include "Perceptron.h"
namespace Tagging
{
	Perceptron::Perceptron(class Sample* sample, class Model* model, bool train, int round, int beamSize, int nGram)
	{
		m_sample  = sample;
		m_model   = model;
		m_round   = round;
		m_decoder = new Decoder(m_model, train, m_model->LeftBound(), m_model->RightBound());
		m_decoder->SetBeamSize(beamSize);
		m_decoder->SetNGram(nGram);
		model->SetBeamSize(beamSize);
		model->SetNGram(nGram);
	}

	Perceptron::~Perceptron()
	{
		delete m_decoder;
	}

	bool  Perceptron::Test(const std::vector<std::vector<std::string> >& tokens, std::vector<std::string>& result)
	{
		result.clear();
		std::vector<std::vector<int> > sample;
		std::vector<int> tags;
		m_sample->Tokens2Id(tokens, &sample);
		Test(&sample, tags);
		int size = tags.size();
		for (int i = m_sample->LeftPadding; i < size; i++)
		{
			result.push_back(m_sample->GetTag(tags[i]));
		}
		return true;
	}

	bool Perceptron::Test(std::vector< std::vector<int> >* samples, std::vector<int>& result)
	{
		result.clear();
		return m_decoder->Decode(samples, result);
	}

	bool Perceptron::Test(const std::string& result)
	{
		std::ofstream fout(result);
		if (!fout.is_open())
		{
			std::cout << "Write Result File " << result << " Error!!!" << std::endl;;
			return false;
		}
		int size = m_sample->GetSize();
		std::vector<std::vector<int> > test;
		test.resize(size);
		int fieldSize  = m_model->FieldSize();
		bool score     = (fieldSize == m_sample->FieldSize);
		double total   = 0;
		double correct = 0;
		for (int i = 0; i < size; i++)
		{
			std::vector<int> t;
			std::vector<int> g;
			if (!m_decoder->Decode(m_sample->GetSample(i), t))
			{
				//
			}
			for (int j = m_sample->LeftPadding; j < (t.size()); j++)
			{
				fout << m_sample->Samples[i][j - m_sample->LeftPadding] << "\t" << m_sample->GetTag(t[j]) << std::endl;
			}
			fout << std::endl;
			if (score)
			{
				g.clear();
				std::vector<std::vector<int> > s = *m_sample->GetSample(i);
				for (int j = 0; j < (s.size() - m_sample->RightPadding); j++)
				{
					g.push_back(s[j][fieldSize - 1]);
				}
				if (t.size() == g.size())
				{
					total += (t.size() - m_sample->LeftPadding);
					for (int j = m_sample->LeftPadding; j < t.size(); j++)
					{
						if (t[j] == g[j])
						{
							correct++;
						}
					}
				}
			}
		}
		if (score)
		{
			std::cout << (correct / total) << std::endl;
		}
		return true;
	}

	bool Perceptron::Train()
	{
		int size = m_sample->GetSize();
		int progress = size / 30;
		for (int iter = 0; iter < m_round; iter++)
		{
			m_sample->Shuffle();
			std::vector<int> tags_test;
			std::vector<int> tags_gold;
			for (int i = 0; i < size; i++)
			{
				if (!m_decoder->Decode(m_sample->GetSample(i), tags_test, &tags_gold))
				{
					for (int j = m_sample->LeftPadding; j < tags_test.size(); j++)
					{
						std::vector<std::vector<int> > featuresTest;
						std::vector<std::vector<int> > featuresGold;
						m_decoder->GenerateFeatures(featuresTest, tags_test, j);
						m_decoder->GenerateFeatures(featuresGold, tags_gold, j);
						m_model->UpdateWeights(featuresTest, featuresGold, iter, i);
					}
				}
				if (i == (size - 1))
				{
					m_model->UpdateWeights(iter, i);
				}
				if (i % progress == 0)
				{
					cout << '.';
					cout.flush();
				}
			}
			std::cout << "\t iter" << iter + 1 << std::endl;
		}
		return true;
	}
}