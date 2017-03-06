/**
* @file Utils/JsonParser.cpp
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

//--------------------------------------------------------------------
void _JsonParser::parseLevelInfo(const int16_t & level)
//--------------------------------------------------------------------
{
	cocos2d::log("JsonParser::parseLevelInfo: parsing level=%d", level);
	auto fullPath = cocos2d::StringUtils::format(GameResources::s_templateLevelName.getCString(), level);
	auto json = cocos2d::FileUtils::getInstance()->getStringFromFile(fullPath.c_str());
	mLoadedLevel = level;

	Json::Reader jsonReader;
	jsonReader.parse(json, mRootNode);
	if (!checkStatus()) {
		throw std::invalid_argument("json parse error");
	}
}

//--------------------------------------------------------------------
bool _JsonParser::checkStatus()
//--------------------------------------------------------------------
{
	Json::Value& status = mRootNode[JsonNames::loadResult];
	if (!status.isString())
		return false;

	return std::strcmp(mRootNode[JsonNames::loadResult].asCString(), "OK") == 0;
}

//--------------------------------------------------------------------
CommonTypes::LevelInfo _JsonParser::getLevelInfo()
//--------------------------------------------------------------------
{
	auto levelInfo = CommonTypes::LevelInfo();
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
CommonTypes::LevelGoals _JsonParser::getLevelGoals()
//--------------------------------------------------------------------
{
    auto levelGoals = CommonTypes::LevelGoals();

    if (mRootNode[JsonNames::levelGoals].isNull()) {
        return levelGoals;
    }
    const Json::Value& levelGoalsNode = mRootNode[JsonNames::levelGoals];

    levelGoals.goalsCount = getGoalsCount(levelGoalsNode);

    const Json::Value& subNode = getGoalTypeCollect(levelGoalsNode);

    for (uint16_t i = 0; i < subNode.size(); ++i) {
        const Json::Value& goalCollectNode = subNode[i];
        CC_ASSERT(goalCollectNode.isObject());

        auto goalInfo = CommonTypes::CollectGoalInfo();
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
	Json::Value& value = mRootNode[JsonNames::tilesArray];
	if (!value.isArray())
		throw std::logic_error("bad tiles array");

	return value;
}

//--------------------------------------------------------------------
const Json::Value & _JsonParser::getPredefinedCookies()
//--------------------------------------------------------------------
{
    Json::Value& value = mRootNode[JsonNames::cookiesArray];
    if (!value.isArray())
        throw std::logic_error("bad cookies array");

    return value;
}

//--------------------------------------------------------------------
const Json::Value & _JsonParser::getAllowedCookieTypes()
//--------------------------------------------------------------------
{
    Json::Value& value = mRootNode[JsonNames::allowedCookieTypes];
    if (!value.isArray())
        throw std::logic_error("bad allowedCookieTypes array");

    return value;
}

//--------------------------------------------------------------------
uint32_t _JsonParser::getTargetScore()
//--------------------------------------------------------------------
{
	uint32_t res = 0;
	if (mRootNode[JsonNames::targetScore].isInt()) {
		res = mRootNode[JsonNames::targetScore].asInt();
	}
	return res;
}

//--------------------------------------------------------------------
uint8_t _JsonParser::getMoves()
//--------------------------------------------------------------------
{
	uint8_t res = 0;
	if (mRootNode[JsonNames::movesLeft].isInt()) {
		res = mRootNode[JsonNames::movesLeft].asInt();
	}
	return res;
}

//--------------------------------------------------------------------
uint8_t _JsonParser::getTypesCount()
//--------------------------------------------------------------------
{
    uint8_t res = 0;
    if (mRootNode[JsonNames::objTypesCount].isInt()) {
        res = mRootNode[JsonNames::objTypesCount].asInt();
    }
    return res;
}

//--------------------------------------------------------------------
bool _JsonParser::isPredefinedCookies()
//--------------------------------------------------------------------
{
    bool res = false;
    if (mRootNode[JsonNames::predefinedCookies].asBool()) {
        res = mRootNode[JsonNames::predefinedCookies].asBool();
    }
    return res;
}

//--------------------------------------------------------------------
bool _JsonParser::getSkipEmptyHoles()
//--------------------------------------------------------------------
{
    bool res = false;
    if (mRootNode[JsonNames::skipEmptyHoles].asBool()) {
        res = mRootNode[JsonNames::skipEmptyHoles].asBool();
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
const Json::Value & _JsonParser::getGoalTypeCollect(const Json::Value & node)
//--------------------------------------------------------------------
{
    const Json::Value& value = node[JsonNames::collectGoal];
    if (!value.isArray())
        throw std::logic_error("bad collect array");

    return value;
}

//--------------------------------------------------------------------
uint8_t _JsonParser::getTargetBaseObjType(const Json::Value & node)
//--------------------------------------------------------------------
{
    uint8_t res = 0;
    if (node[JsonNames::targetBaseObjType].isInt()) {
        res = node[JsonNames::targetBaseObjType].asInt();
    }
    return res;
}

//--------------------------------------------------------------------
uint8_t _JsonParser::getTargetObjType(const Json::Value & node)
//--------------------------------------------------------------------
{
    uint8_t res = 0;
    if (node[JsonNames::targetObjType].isInt()) {
        res = node[JsonNames::targetObjType].asInt();
    }
    return res;
}

//--------------------------------------------------------------------
uint8_t _JsonParser::getTargetObjectCount(const Json::Value & node)
//--------------------------------------------------------------------
{
    uint8_t res = 0;
    if (node[JsonNames::targetObjectCount].isInt()) {
        res = node[JsonNames::targetObjectCount].asInt();
    }
    return res;
}

//--------------------------------------------------------------------
void _JsonParser::updateTiles(CommonTypes::LevelInfo & levelInfo)
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
void _JsonParser::updatePredefinedCookies(CommonTypes::LevelInfo & levelInfo)
//--------------------------------------------------------------------
{
    if (!mRootNode[JsonNames::cookiesArray].isNull()) {
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
void _JsonParser::updateAllowedCookieTypes(CommonTypes::LevelInfo & levelInfo)
//--------------------------------------------------------------------
{
    if (!mRootNode[JsonNames::allowedCookieTypes].isNull()) {
        const Json::Value& allowedCookieTypes = getAllowedCookieTypes();

        for (uint16_t i = 0; i < allowedCookieTypes.size(); ++i) {
            auto type = allowedCookieTypes[i].asInt();
            CC_ASSERT(allowedCookieTypes[i].isInt());
            levelInfo.allowedCookieTypes.push_back(type);
        }
    }
}

//--------------------------------------------------------------------
void _JsonParser::updateFieldObjects(CommonTypes::LevelInfo & levelInfo)
//--------------------------------------------------------------------
{
    if (mRootNode[JsonNames::fieldObjectsArray].isNull()) {
        return;
    }
    const Json::Value& subNode = getFieldObjects(mRootNode[JsonNames::fieldObjectsArray]);

    for (uint16_t i = 0; i < subNode.size(); ++i) {
        const Json::Value& fieldObjectNode = subNode[i];
        CC_ASSERT(fieldObjectNode.isObject());

        auto fieldInfo = ObjTypes::FieldJsonInfo();
        fieldInfo.baseInfo.column = getFieldObjectCol(fieldObjectNode);
        fieldInfo.baseInfo.row = getFieldObjectCol(fieldObjectNode);

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
const Json::Value & _JsonParser::getFieldObjects(const Json::Value & node)
//--------------------------------------------------------------------
{
    Json::Value& value = node[JsonNames::fieldObject_objects];
    if (!value.isArray())
        throw std::logic_error("bad fieldObjects array");

    return value;
}

//--------------------------------------------------------------------
uint8_t _JsonParser::getFieldObjectCol(const Json::Value & node)
//--------------------------------------------------------------------
{
    uint8_t res = 0;
    if (node[JsonNames::fieldObject_col].isInt()) {
        res = node[JsonNames::fieldObject_col].asInt();
    }
    return res;
}

//--------------------------------------------------------------------
uint8_t _JsonParser::getFieldObjectRow(const Json::Value & node)
//--------------------------------------------------------------------
{
    uint8_t res = 0;
    if (node[JsonNames::fieldObject_row].isInt()) {
        res = node[JsonNames::fieldObject_row].asInt();
    }
    return res;
}

//--------------------------------------------------------------------
const Json::Value & _JsonParser::getFieldObjectTypes(const Json::Value & node)
//--------------------------------------------------------------------
{
    Json::Value& value = node[JsonNames::fieldObject_types];
    if (!value.isArray())
        throw std::logic_error("bad fieldObject_types array");

    return value;
}
