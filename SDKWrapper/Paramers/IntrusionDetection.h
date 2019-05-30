#pragma once
#include "ParamBase.h"

class IntrusionDetection : public CParamBase
{
public:
	std::string QueryAll(bool isDefault = false);
	void StructToJson(int n);

	int SaveAll(std::string strData);
	double Round(double dVal, short iPlaces);
	std::string m_str;
	std::vector <RSNet_SoleJDVCAIntrusionDetectionConfig_t> m_vec;
};
