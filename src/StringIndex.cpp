#include "StringIndex.h"
#include "ErrorMaskHelper.h"

void StringIndex::init(size_t maxCode)
{
	index.resize(maxCode);
}

void StringIndex::buildReverseIndex()
{
	reverseIndex.resize(index.size());
	for (size_t i = 0; i < index.size(); i++)
	{
		reverseIndex[i].resize(index[i].size(), "");
		for (auto pair : index[i])
		{
			assert(reverseIndex[i][pair.second] == "");
			assert(pair.second < reverseIndex[i].size());
			reverseIndex[i][pair.second] = pair.first;
		}
		for (size_t j = 0; j < reverseIndex[i].size(); j++)
		{
			assert(reverseIndex[i][j] != "");
		}
	}
}

std::string StringIndex::getString(uint16_t compressed, uint32_t index) const
{
	if (compressed >= 0 && compressed <= 3)
	{
		std::string result;
		for (size_t i = 0; i < index; i++)
		{
			result += "ACGT"[compressed];
		}
		return result;
	}
	if (complement(compressed) < compressed)
	{
		compressed = complement(compressed);
		assert(compressed < reverseIndex.size());
		assert(index < reverseIndex[compressed].size());
		return revCompRaw(reverseIndex[compressed][index]);
	}
	else
	{
		assert(compressed < reverseIndex.size());
		assert(index < reverseIndex[compressed].size());
		return reverseIndex[compressed][index];
	}
}

uint32_t StringIndex::getIndex(uint16_t compressed, std::string expanded)
{
	if (compressed >= 0 && compressed <= 3)
	{
		return expanded.size();
	}
	if (complement(compressed) < compressed)
	{
		compressed = complement(compressed);
		expanded = revCompRaw(expanded);
	}
	uint32_t maybeResult = index[compressed].size();
	auto found = index[compressed].find(expanded);
	if (found != index[compressed].end())
	{
		return found->second;
	}
	index[compressed][expanded] = maybeResult;
	return maybeResult;
}
