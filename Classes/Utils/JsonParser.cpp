/**
* @file Utils/JsonParser.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/


#include "JsonParser.h"
#include "Utils/GameResources.h"

//--------------------------------------------------------------------
void JsonParser::parseLevelInfo(const int16_t & level)
//--------------------------------------------------------------------
{
	CCLOGINFO("JsonParser::parseLevelInfo: parsing level=%d", level);
	auto fullPath = cocos2d::StringUtils::format(GameResources::s_templateLevelName.c_str(), level);
	auto json = cocos2d::FileUtils::getInstance()->getStringFromFile(fullPath.c_str());
	mLoadedLevel = level;

	Json::Reader jsonReader;
	jsonReader.parse(json, mRootNode);
	if (!checkStatus()) {
		throw std::invalid_argument("json parse error");
	}
}

//--------------------------------------------------------------------
bool JsonParser::checkStatus()
//--------------------------------------------------------------------
{
	Json::Value& status = mRootNode["res"];
	if (!status.isString())
		return false;

	return std::strcmp(mRootNode["res"].asCString(), "OK") == 0;
}

//--------------------------------------------------------------------
CommonTypes::LevelInfo JsonParser::getLevelInfo()
//--------------------------------------------------------------------
{
	auto levelInfo = CommonTypes::LevelInfo();
	levelInfo.id = mLoadedLevel;
	levelInfo.targetScore = getTargetScore();
	levelInfo.moves = getMoves();

	const Json::Value& node = getTiles();

	for (uint16_t i = 0; i < node.size(); ++i) {
		const Json::Value& subnode = node[i];
		CC_ASSERT(subnode.isArray());

		for (uint16_t j = 0; j < subnode.size(); ++j) {
			CC_ASSERT(subnode[j].isInt());
            levelInfo.tiles[i][j] = node[i][j].asInt();
			levelInfo.tiles[i][j] = node[node.size() - j - 1][i].asInt();
		}
	}
	return levelInfo;
}

//--------------------------------------------------------------------
const Json::Value& JsonParser::getTiles()
//--------------------------------------------------------------------
{
	Json::Value& value = mRootNode["tiles"];
	if (!value.isArray())
		throw std::logic_error("bad tiles array");

	return value;
}

//--------------------------------------------------------------------
int16_t JsonParser::getTargetScore()
//--------------------------------------------------------------------
{
	int16_t res = 0;
	if (mRootNode["targetScore"].isInt()) {
		res = mRootNode["targetScore"].asInt();
	}
	return res;
}

//--------------------------------------------------------------------
int16_t JsonParser::getMoves()
//--------------------------------------------------------------------
{
	int16_t res = 0;
	if (mRootNode["moves"].isInt()) {
		res = mRootNode["moves"].asInt();
	}
	return res;
}
