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

#include "cocos2d/external/json/document.h"
#include "cocos2d/external/json/rapidjson.h"
#include "cocos2d/external/json/filereadstream.h"
#include <sstream>
#include <fstream>

using namespace rapidjson;

//--------------------------------------------------------------------
/*CommonTypes::LevelInfo JsonParser::getLevelInfo(const int16_t& level)*/
//--------------------------------------------------------------------

//    CCLOGINFO("JsonParser::getLevelInfo: loading level=%d", level);
//    auto fullPath = cocos2d::StringUtils::format(GameResources::s_templateLevelName.c_str(), level);
//    auto json = cocos2d::FileUtils::getInstance()->getStringFromFile(fullPath.c_str());
//    CCLOGINFO("JsonParser::getLevelInfo: loading json=%s", json.c_str());

//--------------------------------------------------------------------
void JsonParser::parseLevelInfo(const int16_t & level)
//--------------------------------------------------------------------
{
	CCLOGINFO("JsonParser::parseLevelInfo: parsing level=%d", level);
	auto fullPath = cocos2d::StringUtils::format(GameResources::s_templateLevelName.c_str(), level);

	mLoadedLevel = level;

// 	std::stringstream ss;
// 	std::ifstream ifs;
// 	ifs.open(fullPath.c_str(), std::ios::binary);
// 	ss << ifs.rdbuf();
// 	ifs.close();
// 

	FILE* pFile = fopen(fullPath.c_str(), "rb");
	char buffer[65536];
	FileReadStream is(pFile, buffer, sizeof(buffer));
	
	if (mDoc.ParseStream<0, UTF8<>, FileReadStream>(is).HasParseError()) {
		throw std::invalid_argument("json parse error");
	}
}

//--------------------------------------------------------------------
bool JsonParser::checkStatus()
//--------------------------------------------------------------------
{
	rapidjson::Value& status = mDoc["res"];
	if (!status.IsString())
		return false;

	return std::strcmp(mDoc["res"].GetString(), "OK") == 0;
}

//--------------------------------------------------------------------
CommonTypes::LevelInfo JsonParser::getLevelInfo()
//--------------------------------------------------------------------
{
	auto levelInfo = CommonTypes::LevelInfo();
	levelInfo.id = mLoadedLevel;
	levelInfo.targetScore = getTargetScore();
	levelInfo.moves = getMoves();

	const rapidjson::Value& tiles = getTiles();

	for (SizeType i = 0; i < tiles.Size(); i++) {
		CC_ASSERT(tiles[i].IsArray());

		for (SizeType j = 0; j < tiles[i].Size(); j++) {
			const Value& tile = tiles[i][j];
			levelInfo.tiles[i][j] = tile.GetInt();
		}
	}
	return levelInfo;
}

//--------------------------------------------------------------------
const rapidjson::Value& JsonParser::getTiles()
//--------------------------------------------------------------------
{
	rapidjson::Value& value = mDoc["tiles"];
	if (!value.IsArray())
		throw std::logic_error("bad tiles array");

	return value;
}

//--------------------------------------------------------------------
int16_t JsonParser::getTargetScore()
//--------------------------------------------------------------------
{
	int16_t res = 0;
	if (mDoc.HasMember("targetScore")) {
		res = mDoc["targetScore"].GetInt();
	}
	return res;
}

//--------------------------------------------------------------------
int16_t JsonParser::getMoves()
//--------------------------------------------------------------------
{
	int16_t res = 0;
	if (mDoc.HasMember("moves")) {
		res = mDoc["moves"].GetInt();
	}
	return res;
}
