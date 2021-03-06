#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <list>
#include <map>
#include <cstdlib>

#define LINK_VALUE ','
#define END_BLOCK '}'
#define FLAG_END_FILE '!'

typedef struct currentBlockInfo
{
	std::string type;
	std::string block;
	std::string info;
};

typedef struct InfoParser
{
	std::string type;
	std::string value;
};

typedef std::map<std::string, std::map<std::string, InfoParser > > arrayInfoParser;

class ParserSceneInfo;
typedef void (ParserSceneInfo::*MemFunc)(const std::string &);
#define GET_STATE_INFO(object,ptrToMember)  ((object).*(ptrToMember))

//All state of parsing
enum STATE : int
{
	TYPE_BLOCK,
	NAME_BLOCK,
	NAME_INFO,
	TYPE_INFO,
	VALUE_INFO,
	state_count,
	END_FILE
};


//Info of current state
typedef struct StateInfo
{
	std::string meaningChar;
	std::string startFlag;
	std::string endFlag;
};

class ParserSceneInfo
{
public:
	ParserSceneInfo();
	~ParserSceneInfo();

	bool init(const std::string &nameFile);
	template<typename T> T setInfo(const std::string &keyType, const std::string &keyBlock, const std::string &keyObject);
	arrayInfoParser getAllBlockFromType(const std::string &idType);
private:
	int _cursor;
	STATE _currentState;
	currentBlockInfo _infoBlock;
	StateInfo _allStateInfo[state_count];
	MemFunc _gettersStateInfo[STATE::state_count];
	std::map<std::string, arrayInfoParser> _infoScene; 
	//the typeInfo will be used for create object directly from config.txt like "block","30, 450" but not to getting the info

	//All member parsing method
	void findTheNextStartingFlag(const std::string &bufferFile);
	void getTypeBlock(const std::string &bufferFile);
	void getNameBlock(const std::string &bufferFile);
	void getNameInfo(const std::string &bufferFile);
	void getTypeInfo(const std::string &bufferFile);
	void getValueInfo(const std::string &bufferFile);
	void getInfoState(ParserSceneInfo &parser, int memGetter, const std::string &bufferFile);
};

bool isMeaningChar(char c, const std::string & listMeaningChar);
std::string goToEndFlag(int *cursor, const std::string endFlag, const std::string &bufferFile, const std::string listMeaningChar);