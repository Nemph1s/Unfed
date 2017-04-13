/**
* @file Utils/Parser/JsonParser.cpp
* Copyright (C) 2017
* Company       Octohead LTD
*               All Rights Reserved
*               Secrecy Level STRICTLY CONFIDENTIAL
*
* @author VMartyniuk
*/

#include "Utils/Parser/JsonParser.h"
#include "Utils/Parser/JsonNames.h"
#include "Utils/GameResources.h"
#include "Common/GlobalInfo/GlobalInfo.h"

//--------------------------------------------------------------------
void _JsonParser::parseGlobalInfo()
//--------------------------------------------------------------------
{
    cocos2d::log("JsonParser::parseGlobalInfo:");
    auto fileUtils = cocos2d::FileUtils::getInstance();
    auto json = fileUtils->getStringFromFile(GameResources::s_GlobalInfoPath.getCString());

    Json::Reader jsonReader;
    jsonReader.parse(json, mGlobalInfoRootNode);
    if (!checkGlobalInfoStatus()) {
        throw std::invalid_argument("json parse error");
    }
}

//--------------------------------------------------------------------
bool _JsonParser::checkGlobalInfoStatus()
//--------------------------------------------------------------------
{
    Json::Value& status = mGlobalInfoRootNode[JsonNames::GlobalInfo::loadResult];
    if (!status.isString())
        return false;

    return std::strcmp(mGlobalInfoRootNode[JsonNames::GlobalInfo::loadResult].asCString(), "OK") == 0;
}

//--------------------------------------------------------------------
CT::SGlobalInfo _JsonParser::getJsonGlobalInfo()
//--------------------------------------------------------------------
{
    auto globalInfo = CT::SGlobalInfo();
    globalInfo.swVersion = getSwVersion();
    globalInfo.imgPackType = getImagePackType();
    globalInfo.imgPackVersion = getImagePackVersion();
    globalInfo.tileWidth = getTileWidth();
    globalInfo.tileHeigth = getTileHeight();
    globalInfo.levelsCount = getLevelsCount();

    return globalInfo;
}

//--------------------------------------------------------------------
void _JsonParser::parseScoreValues()
//--------------------------------------------------------------------
{
    cocos2d::log("JsonParser::parseScoreValues:");
    mScoreValuesRootNode = mGlobalInfoRootNode[JsonNames::GlobalInfo::scoreValues];

    if (!checkScoreValuesStatus()) {
        throw std::invalid_argument("json parse error");
    }
}

//--------------------------------------------------------------------
bool _JsonParser::checkScoreValuesStatus()
//--------------------------------------------------------------------
{
    Json::Value& status = mGlobalInfoRootNode[JsonNames::GlobalInfo::scoreValues];
    if (!status.isObject())
        return false;

    return true;
}

//--------------------------------------------------------------------
void _JsonParser::updateScoreValues()
//--------------------------------------------------------------------
{
    auto globInfo = _GlobalInfo::getInstance();
    globInfo->mCookieMinScore = getCookieMinimalScore();
    globInfo->mCookieDefaultScore = getCookieDefaultScore();
    globInfo->mFieldObjScore = getFieldObjScore();
    globInfo->mEnemyScore = getEnemyScore();
    updateDudeScoreMap();
}

//--------------------------------------------------------------------
const char* _JsonParser::getSwVersion() const
//--------------------------------------------------------------------
{
    const char* res = nullptr;
    if (mGlobalInfoRootNode[JsonNames::GlobalInfo::swVersion].isString()) {
        res = mGlobalInfoRootNode[JsonNames::GlobalInfo::swVersion].asCString();
    }
    return res;
}

//--------------------------------------------------------------------
const char* _JsonParser::getImagePackType() const
//--------------------------------------------------------------------
{
    const char* res = nullptr;
    if (mGlobalInfoRootNode[JsonNames::GlobalInfo::imgPackType].isString()) {
        res = mGlobalInfoRootNode[JsonNames::GlobalInfo::imgPackType].asCString();
    }
    return res;
}

//--------------------------------------------------------------------
const char* _JsonParser::getImagePackVersion() const
//--------------------------------------------------------------------
{
    const char* res = nullptr;
    if (mGlobalInfoRootNode[JsonNames::GlobalInfo::imgPackVersion].isString()) {
        res = mGlobalInfoRootNode[JsonNames::GlobalInfo::imgPackVersion].asCString();
    }
    return res;
}

//--------------------------------------------------------------------
const double& _JsonParser::getTileWidth() const
//--------------------------------------------------------------------
{
    if (mGlobalInfoRootNode[JsonNames::GlobalInfo::tileWidth].isDouble()) {
        return mGlobalInfoRootNode[JsonNames::GlobalInfo::tileWidth].asDouble();
    }
}

//--------------------------------------------------------------------
const double& _JsonParser::getTileHeight() const
//--------------------------------------------------------------------
{
    if (mGlobalInfoRootNode[JsonNames::GlobalInfo::tileHeigth].isDouble()) {
        return mGlobalInfoRootNode[JsonNames::GlobalInfo::tileHeigth].asDouble();
    }
}

//--------------------------------------------------------------------
const uint16_t& _JsonParser::getLevelsCount() const
//--------------------------------------------------------------------
{
    if (mGlobalInfoRootNode[JsonNames::GlobalInfo::levelsCount].isUInt()) {
        return mGlobalInfoRootNode[JsonNames::GlobalInfo::levelsCount].asUInt();
    }
}

//--------------------------------------------------------------------
const uint16_t& _JsonParser::getCookieMinimalScore() const
//--------------------------------------------------------------------
{
    if (mScoreValuesRootNode[JsonNames::GlobalInfo::ScoreValues::cookieMinimal].isUInt()) {
        return mScoreValuesRootNode[JsonNames::GlobalInfo::ScoreValues::cookieMinimal].asUInt();
    }
}

//--------------------------------------------------------------------
const uint16_t& _JsonParser::getCookieDefaultScore() const
//--------------------------------------------------------------------
{
    if (mScoreValuesRootNode[JsonNames::GlobalInfo::ScoreValues::cookieDefault].isUInt()) {
        return mScoreValuesRootNode[JsonNames::GlobalInfo::ScoreValues::cookieDefault].asUInt();
    }
}

//--------------------------------------------------------------------
const uint16_t& _JsonParser::getFieldObjScore() const
//--------------------------------------------------------------------
{
    if (mScoreValuesRootNode[JsonNames::GlobalInfo::ScoreValues::fieldObject].isUInt()) {
        return mScoreValuesRootNode[JsonNames::GlobalInfo::ScoreValues::fieldObject].asUInt();
    }
}

//--------------------------------------------------------------------
const uint16_t& _JsonParser::getEnemyScore() const
//--------------------------------------------------------------------
{
    if (mScoreValuesRootNode[JsonNames::GlobalInfo::ScoreValues::enemy].isUInt()) {
        return mScoreValuesRootNode[JsonNames::GlobalInfo::ScoreValues::enemy].asUInt();
    }
}

//--------------------------------------------------------------------
void _JsonParser::updateDudeScoreMap()
//--------------------------------------------------------------------
{
    auto globInfo = _GlobalInfo::getInstance();
    const Json::Value& subNode = getScoreDudes();

    for (uint16_t i = 0; i < subNode.size(); ++i) {
        const Json::Value& dudesNode = subNode[i];
        CC_ASSERT(dudesNode.isObject());

        auto dudeType = getScoreDudeType(dudesNode);
        auto dudeValue = getScoreDudeValue(dudesNode);
        globInfo->mDudeScoreMap[dudeType] = dudeValue;
    }
}

//--------------------------------------------------------------------
const Json::Value & _JsonParser::getScoreDudes()
//--------------------------------------------------------------------
{
    const Json::Value& value = mScoreValuesRootNode[JsonNames::GlobalInfo::ScoreValues::dudes];
    if (!value.isArray())
        throw std::logic_error("bad collect array");

    return value;
}

//--------------------------------------------------------------------
uint16_t _JsonParser::getScoreDudeType(const Json::Value & node)
//--------------------------------------------------------------------
{
    uint16_t res = 0;
    if (node[JsonNames::GlobalInfo::ScoreValues::dudeType].isInt()) {
        res = node[JsonNames::GlobalInfo::ScoreValues::dudeType].asInt();
    }
    return res;
}

//--------------------------------------------------------------------
uint16_t _JsonParser::getScoreDudeValue(const Json::Value & node)
//--------------------------------------------------------------------
{
    uint16_t res = 0;
    if (node[JsonNames::GlobalInfo::ScoreValues::dudeValue].isInt()) {
        res = node[JsonNames::GlobalInfo::ScoreValues::dudeValue].asInt();
    }
    return res;
}

//--------------------------------------------------------------------
void _JsonParser::parseLevelInfo(const int16_t& level)
//--------------------------------------------------------------------
{
	cocos2d::log("JsonParser::parseLevelInfo: parsing level=%d", level);
	auto fullPath = cocos2d::StringUtils::format(GameResources::s_templateLevelName.getCString(), level);
	auto json = cocos2d::FileUtils::getInstance()->getStringFromFile(fullPath.c_str());
	mLoadedLevel = level;

	Json::Reader jsonReader;
	jsonReader.parse(json, mLevelInfoRootNode);
	if (!checkLevelInfoStatus()) {
		throw std::invalid_argument("json parse error");
	}
}

//--------------------------------------------------------------------
bool _JsonParser::checkLevelInfoStatus()
//--------------------------------------------------------------------
{
	Json::Value& status = mLevelInfoRootNode[JsonNames::loadResult];
	if (!status.isString())
		return false;

	return std::strcmp(mLevelInfoRootNode[JsonNames::loadResult].asCString(), "OK") == 0;
}

//--------------------------------------------------------------------
CT::LevelInfo _JsonParser::getJsonLevelInfo()
//--------------------------------------------------------------------
{
	auto levelInfo = CT::LevelInfo();
	levelInfo.id = mLoadedLevel;
	levelInfo.targetScore = getTargetScore();
	levelInfo.moves = getMoves();
    levelInfo.typesCount = getTypesCount();
    levelInfo.skipEmptyHoles = getSkipEmptyHoles();
    levelInfo.isPredefinedCookies = isPredefinedCookies();

    updateTiles(levelInfo);
    updatePredefinedCookies(levelInfo);
    updateAllowedCookieTypes(levelInfo);
    updateFieldObjects(levelInfo);

	return levelInfo;
}

//--------------------------------------------------------------------
CT::LevelGoals _JsonParser::getJsonLevelGoals()
//--------------------------------------------------------------------
{
    auto levelGoals = CT::LevelGoals();

    if (mLevelInfoRootNode[JsonNames::levelGoals].isNull()) {
        return levelGoals;
    }
    const Json::Value& levelGoalsNode = mLevelInfoRootNode[JsonNames::levelGoals];

    levelGoals.goalsCount = getGoalsCount(levelGoalsNode);

    const Json::Value& subNode = getGoalTypeCollect(levelGoalsNode);

    for (uint16_t i = 0; i < subNode.size(); ++i) {
        const Json::Value& goalCollectNode = subNode[i];
        CC_ASSERT(goalCollectNode.isObject());

        auto goalInfo = CT::CollectGoalInfo();
        goalInfo.baseObjectType = getTargetBaseObjType(goalCollectNode);
        goalInfo.objectType = getTargetObjType(goalCollectNode);
        goalInfo.targetCount = getTargetObjectCount(goalCollectNode);

        levelGoals.collectGoals.push_back(goalInfo);
    }
    return levelGoals;
}

//--------------------------------------------------------------------
const Json::Value& _JsonParser::getTiles()
//--------------------------------------------------------------------
{
	Json::Value& value = mLevelInfoRootNode[JsonNames::tilesArray];
	if (!value.isArray())
		throw std::logic_error("bad tiles array");

	return value;
}

//--------------------------------------------------------------------
const Json::Value & _JsonParser::getPredefinedCookies()
//--------------------------------------------------------------------
{
    Json::Value& value = mLevelInfoRootNode[JsonNames::cookiesArray];
    if (!value.isArray())
        throw std::logic_error("bad cookies array");

    return value;
}

//--------------------------------------------------------------------
const Json::Value & _JsonParser::getAllowedCookieTypes()
//--------------------------------------------------------------------
{
    Json::Value& value = mLevelInfoRootNode[JsonNames::allowedCookieTypes];
    if (!value.isArray())
        throw std::logic_error("bad allowedCookieTypes array");

    return value;
}

//--------------------------------------------------------------------
uint32_t _JsonParser::getTargetScore()
//--------------------------------------------------------------------
{
	uint32_t res = 0;
	if (mLevelInfoRootNode[JsonNames::targetScore].isInt()) {
		res = mLevelInfoRootNode[JsonNames::targetScore].asInt();
	}
	return res;
}

//--------------------------------------------------------------------
uint8_t _JsonParser::getMoves()
//--------------------------------------------------------------------
{
	uint8_t res = 0;
	if (mLevelInfoRootNode[JsonNames::movesLeft].isInt()) {
		res = mLevelInfoRootNode[JsonNames::movesLeft].asInt();
	}
	return res;
}

//--------------------------------------------------------------------
uint8_t _JsonParser::getTypesCount()
//--------------------------------------------------------------------
{
    uint8_t res = 0;
    if (mLevelInfoRootNode[JsonNames::CommonTypesCount].isInt()) {
        res = mLevelInfoRootNode[JsonNames::CommonTypesCount].asInt();
    }
    return res;
}

//--------------------------------------------------------------------
bool _JsonParser::isPredefinedCookies()
//--------------------------------------------------------------------
{
    bool res = false;
    if (mLevelInfoRootNode[JsonNames::predefinedCookies].asBool()) {
        res = mLevelInfoRootNode[JsonNames::predefinedCookies].asBool();
    }
    return res;
}

//--------------------------------------------------------------------
bool _JsonParser::getSkipEmptyHoles()
//--------------------------------------------------------------------
{
    bool res = false;
    if (mLevelInfoRootNode[JsonNames::skipEmptyHoles].asBool()) {
        res = mLevelInfoRootNode[JsonNames::skipEmptyHoles].asBool();
    }
    return res;
}

//--------------------------------------------------------------------
uint8_t _JsonParser::getGoalsCount(const Json::Value & node)
//--------------------------------------------------------------------
{
    uint8_t res = 0;
    if (node[JsonNames::goalsCount].isInt()) {
        res = node[JsonNames::goalsCount].asInt();
    }
    return res;
}

//--------------------------------------------------------------------
const Json::Value& _JsonParser::getGoalTypeCollect(const Json::Value& node)
//--------------------------------------------------------------------
{
    const Json::Value& value = node[JsonNames::collectGoal];
    if (!value.isArray())
        throw std::logic_error("bad collect array");

    return value;
}

//--------------------------------------------------------------------
uint8_t _JsonParser::getTargetBaseObjType(const Json::Value& node)
//--------------------------------------------------------------------
{
    uint8_t res = 0;
    if (node[JsonNames::targetBaseObjType].isInt()) {
        res = node[JsonNames::targetBaseObjType].asInt();
    }
    return res;
}

//--------------------------------------------------------------------
uint8_t _JsonParser::getTargetObjType(const Json::Value& node)
//--------------------------------------------------------------------
{
    uint8_t res = 0;
    if (node[JsonNames::targetObjType].isInt()) {
        res = node[JsonNames::targetObjType].asInt();
    }
    return res;
}

//--------------------------------------------------------------------
uint8_t _JsonParser::getTargetObjectCount(const Json::Value& node)
//--------------------------------------------------------------------
{
    uint8_t res = 0;
    if (node[JsonNames::targetObjectCount].isInt()) {
        res = node[JsonNames::targetObjectCount].asInt();
    }
    return res;
}

//--------------------------------------------------------------------
void _JsonParser::updateTiles(CT::LevelInfo& levelInfo)
//--------------------------------------------------------------------
{
    const Json::Value& node = getTiles();

    for (uint16_t i = 0; i < node.size(); ++i) {
        const Json::Value& subnode = node[i];
        CC_ASSERT(subnode.isArray());

        for (uint16_t j = 0; j < subnode.size(); ++j) {
            CC_ASSERT(subnode[j].isInt());
            levelInfo.tiles[i][j] = node[j][i].asInt();
        }
    }
}

//--------------------------------------------------------------------
void _JsonParser::updatePredefinedCookies(CT::LevelInfo& levelInfo)
//--------------------------------------------------------------------
{
    if (!mLevelInfoRootNode[JsonNames::cookiesArray].isNull()) {
        const Json::Value& cookiesObjects = getPredefinedCookies();

        for (uint16_t i = 0; i < cookiesObjects.size(); ++i) {
            const Json::Value& subnode = cookiesObjects[i];
            CC_ASSERT(subnode.isArray());

            for (uint16_t j = 0; j < subnode.size(); ++j) {
                CC_ASSERT(subnode[j].isInt());
                levelInfo.cookies[i][j] = cookiesObjects[j][i].asInt();
            }
        }
    }
}

//--------------------------------------------------------------------
void _JsonParser::updateAllowedCookieTypes(CT::LevelInfo& levelInfo)
//--------------------------------------------------------------------
{
    if (!mLevelInfoRootNode[JsonNames::allowedCookieTypes].isNull()) {
        const Json::Value& allowedCookieTypes = getAllowedCookieTypes();

        for (uint16_t i = 0; i < allowedCookieTypes.size(); ++i) {
            auto type = allowedCookieTypes[i].asInt();
            CC_ASSERT(allowedCookieTypes[i].isInt());
            levelInfo.allowedCookieTypes.push_back(type);
        }
    }
}

//--------------------------------------------------------------------
void _JsonParser::updateFieldObjects(CT::LevelInfo& levelInfo)
//--------------------------------------------------------------------
{
    if (mLevelInfoRootNode[JsonNames::fieldObjectsArray].isNull()) {
        return;
    }
    const Json::Value& subNode = getFieldObjects(mLevelInfoRootNode[JsonNames::fieldObjectsArray]);

    for (uint16_t i = 0; i < subNode.size(); ++i) {
        const Json::Value& fieldObjectNode = subNode[i];
        CC_ASSERT(fieldObjectNode.isObject());

        auto fieldInfo = CT::JsonFieldInfo();
        fieldInfo.baseInfo.column = getFieldObjectCol(fieldObjectNode);
        fieldInfo.baseInfo.row = getFieldObjectRow(fieldObjectNode);

        const Json::Value& typesNode = getFieldObjectTypes(fieldObjectNode);
        CC_ASSERT(typesNode.isArray());

        for (uint16_t i = 0; i < typesNode.size(); ++i) {
            CC_ASSERT(typesNode[i].isInt());
            fieldInfo.fieldType.push_back(typesNode[i].asInt());
        }

        levelInfo.fieldObjects.push_back(fieldInfo);
    }
}

//--------------------------------------------------------------------
const Json::Value& _JsonParser::getFieldObjects(const Json::Value& node)
//--------------------------------------------------------------------
{
    const Json::Value& value = node[JsonNames::fieldObject_objects];
    if (!value.isArray())
        throw std::logic_error("bad fieldObjects array");

    return value;
}

//--------------------------------------------------------------------
uint8_t _JsonParser::getFieldObjectCol(const Json::Value& node)
//--------------------------------------------------------------------
{
    uint8_t res = 0;
    if (node[JsonNames::fieldObject_col].isInt()) {
        res = node[JsonNames::fieldObject_col].asInt();
    }
    return res;
}

//--------------------------------------------------------------------
uint8_t _JsonParser::getFieldObjectRow(const Json::Value& node)
//--------------------------------------------------------------------
{
    uint8_t res = 0;
    if (node[JsonNames::fieldObject_row].isInt()) {
        res = node[JsonNames::fieldObject_row].asInt();
    }
    return res;
}

//--------------------------------------------------------------------
const Json::Value& _JsonParser::getFieldObjectTypes(const Json::Value& node)
//--------------------------------------------------------------------
{
    const Json::Value& value = node[JsonNames::fieldObject_types];
    if (!value.isArray())
        throw std::logic_error("bad fieldObject_types array");

    return value;
}
