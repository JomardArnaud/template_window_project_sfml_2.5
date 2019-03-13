#include "parser.h"

ParserSceneInfo::ParserSceneInfo()
{
	//
}

ParserSceneInfo::~ParserSceneInfo()
{
	//
}

void ParserSceneInfo::getInfoState(ParserSceneInfo &parser, int memGetter, const std::string &bufferFile)
{
	GET_STATE_INFO(parser, _gettersStateInfo[_currentState])(bufferFile);
}

bool ParserSceneInfo::init(const std::string &nameFile)
{
	//fat probleme, ne pas laisser vide un block
	//setting all Info State ps: letter, number and underscore are always meaning char, name is on SNAKE format except on one case, for the MORE_THAN_ONE_CHAR_FLAG like :{ for exemple
	_allStateInfo[STATE::TYPE_BLOCK] = StateInfo{ "_", "\"", "\":{" };
	_gettersStateInfo[STATE::TYPE_BLOCK] = &ParserSceneInfo::getTypeBlock;
	_allStateInfo[STATE::NAME_BLOCK] = StateInfo{ "_", "\"", "\":{" };
	_gettersStateInfo[STATE::NAME_BLOCK] = &ParserSceneInfo::getNameBlock;
	_allStateInfo[STATE::NAME_INFO] = StateInfo{ "_", "\"", "\":{"};
	_gettersStateInfo[STATE::NAME_INFO] = &ParserSceneInfo::getNameInfo;
	_allStateInfo[STATE::TYPE_INFO] = StateInfo{ "_", "\"", "\","};
	_gettersStateInfo[STATE::TYPE_INFO] = &ParserSceneInfo::getTypeInfo;
	_allStateInfo[STATE::VALUE_INFO] = StateInfo{ "_ ,.;!\\", "\"", "\"}"};
	_gettersStateInfo[STATE::VALUE_INFO] = &ParserSceneInfo::getValueInfo;
	_currentState = STATE::TYPE_BLOCK;

	//reading file part 
	_cursor = 0;
	std::ifstream sceneInfoFile(nameFile.c_str());
	if (sceneInfoFile.is_open())
	{
		std::filebuf* pbuf = sceneInfoFile.rdbuf();
		std::size_t size = pbuf->pubseekoff(0, sceneInfoFile.end, sceneInfoFile.in);
		pbuf->pubseekpos(0, sceneInfoFile.in);
		char* bufferFile = new char[size];
		pbuf->sgetn(bufferFile, size);

		//starting parsing
		while (_currentState != STATE::END_FILE)
		{
			findTheNextStartingFlag(bufferFile);
			if (_currentState != STATE::END_FILE)
				getInfoState((*this), _currentState, bufferFile);
		}
		sceneInfoFile.close();
	}
	else
	{
		std::cout << "error on open file: " << nameFile << " in the setSceneInfo function" << std::endl;
	}
	return true;
}

void ParserSceneInfo::findTheNextStartingFlag(const std::string &bufferFile)
{
	std::string tmpCheck = _allStateInfo[_currentState].startFlag;
	int cursorCheck = 0;

	while (cursorCheck != tmpCheck.size())
	{
		if (tmpCheck[cursorCheck] == bufferFile[_cursor])
			cursorCheck++;
		else
			cursorCheck = 0;
		if (bufferFile[_cursor] == FLAG_END_FILE)
		{
			_currentState = STATE::END_FILE;
			return;
		}
		_cursor++;
	}
}

bool isMeaningChar(char c, const std::string & listMeaningChar)
{
	return ((c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		(c >= '0' && c <= '9') ||
		listMeaningChar.find(c) != std::string::npos);
}

std::string goToEndFlag(int *cursor, const std::string endFlag, const std::string &bufferFile, const std::string listMeaningChar)
{
	std::string dest;
	std::string tmpCheck = endFlag;
	int cursorCheck = 0;

	while (cursorCheck != tmpCheck.size())
	{
		if (isMeaningChar(bufferFile[(*cursor)], listMeaningChar))
			dest.push_back(bufferFile[(*cursor)]);
		if (tmpCheck[cursorCheck] == bufferFile[(*cursor)])
			cursorCheck++;
		else
			cursorCheck = 0;
		(*cursor)++;
	}
	return dest;
}

void ParserSceneInfo::getTypeBlock(const std::string &bufferFile)
{
	_infoBlock.type = goToEndFlag(&_cursor, _allStateInfo[_currentState].endFlag, bufferFile, _allStateInfo[_currentState].meaningChar);
	_infoScene.insert(std::pair< std::string, arrayInfoParser >(_infoBlock.block, arrayInfoParser()));
	_currentState = STATE::NAME_BLOCK;
}

void ParserSceneInfo::getNameBlock(const std::string &bufferFile)
{
	_infoBlock.block = goToEndFlag(&_cursor, _allStateInfo[_currentState].endFlag, bufferFile, _allStateInfo[_currentState].meaningChar);
	_infoScene[_infoBlock.type].insert(std::pair< std::string, std::map<std::string, InfoParser > >(_infoBlock.block, std::map<std::string, InfoParser >()));
	_currentState = STATE::NAME_INFO;
}

void ParserSceneInfo::getNameInfo(const std::string &bufferFile)
{
	_infoBlock.info = goToEndFlag(&_cursor, _allStateInfo[_currentState].endFlag, bufferFile, _allStateInfo[_currentState].meaningChar);
	_infoScene[_infoBlock.type][_infoBlock.block].insert(std::pair<std::string, InfoParser >(_infoBlock.info, InfoParser()));
	_currentState = STATE::TYPE_INFO;
}

void ParserSceneInfo::getTypeInfo(const std::string &bufferFile)
{
	std::string tmpType;
	tmpType = goToEndFlag(&_cursor, _allStateInfo[_currentState].endFlag, bufferFile, _allStateInfo[_currentState].meaningChar);
	_infoScene[_infoBlock.type][_infoBlock.block][_infoBlock.info].type = tmpType;
	_currentState = STATE::VALUE_INFO;
}

void ParserSceneInfo::getValueInfo(const std::string &bufferFile)
{
	std::string tmpInfo;
	tmpInfo = goToEndFlag(&_cursor, _allStateInfo[_currentState].endFlag, bufferFile, _allStateInfo[_currentState].meaningChar);
	_infoScene[_infoBlock.type][_infoBlock.block][_infoBlock.info].value = tmpInfo;

	int nbEndBlock = 0;
	while (bufferFile[_cursor] != LINK_VALUE)
		{
			_cursor++;
			if (bufferFile[_cursor] == END_BLOCK)
			{
				nbEndBlock++;
			}
			else if (bufferFile[_cursor] == FLAG_END_FILE)
			{
				_currentState = STATE::END_FILE;
				return;
			}
		}
	_currentState = static_cast<STATE>(STATE::NAME_INFO - nbEndBlock);
}

arrayInfoParser ParserSceneInfo::getAllBlockFromType(const std::string &idType)
{
	return _infoScene[(idType)];
}

template<> float ParserSceneInfo::setInfo<>(const std::string &keyType, const std::string &keyBlock, const std::string &keyObject)
{
	return std::atof(_infoScene[keyType][keyBlock][keyObject].value.c_str());
}

template<> int ParserSceneInfo::setInfo<>(const std::string &keyType, const std::string &keyBlock, const std::string &keyObject)
{
	return std::atoi(_infoScene[keyType][keyBlock][keyObject].value.c_str());
}

template<> std::string ParserSceneInfo::setInfo<>(const std::string &keyType, const std::string &keyBlock, const std::string &keyObject)
{
	return _infoScene[keyType][keyBlock][keyObject].value;
}

template<> sf::Color ParserSceneInfo::setInfo<>(const std::string &keyType, const std::string &keyBlock, const std::string &keyObject)
{
	std::string tmp[4];
	int i = 0;
	std::string str(_infoScene[keyType][keyBlock][keyObject].value);
	for (std::string::const_iterator it = str.cbegin(); it != str.cend(); ++it)
		if (*it != LINK_VALUE)
			tmp[i].push_back(*it);
		else
			i++;
	return sf::Color(std::atoi(tmp[0].c_str()), std::atoi(tmp[1].c_str()), std::atoi(tmp[2].c_str()), std::atoi(tmp[3].c_str()));
}

template<> sf::Vector2f ParserSceneInfo::setInfo<>(const std::string &keyType, const std::string &keyBlock, const std::string &keyObject)
{
	std::string tmp[2];
	int i = 0;
	std::string str(_infoScene[keyType][keyBlock][keyObject].value);
	for (std::string::const_iterator it = str.cbegin(); it != str.cend(); ++it)
		if (*it != LINK_VALUE)
			tmp[i].push_back(*it);
		else
			i++;
	return sf::Vector2f(std::atof(tmp[0].c_str()), std::atof(tmp[1].c_str()));
}

template<> sf::Vector2i ParserSceneInfo::setInfo<>(const std::string &keyType, const std::string &keyBlock, const std::string &keyObject)
{
	std::string tmp[2];
	int i = 0;
	std::string str(_infoScene[keyType][keyBlock][keyObject].value);
	for (std::string::const_iterator it = str.cbegin(); it != str.cend(); ++it)
		if (*it != LINK_VALUE)
			tmp[i].push_back(*it);
		else
			i++;
	return sf::Vector2i(std::atoi(tmp[0].c_str()), std::atoi(tmp[1].c_str()));
}

template<> sf::Time ParserSceneInfo::setInfo<sf::Time>(const std::string &keyType, const std::string &keyBlock, const std::string &keyObject)
{
	std::string tmp[2];
	int i = 0;
	std::string str(_infoScene[keyType][keyBlock][keyObject].value);

	i = str.find("s");
	if (i == std::string::npos)
		return sf::seconds(0.f);
	else if (str[i - 1] != 'm')
		return sf::seconds(std::atof(_infoScene[keyType][keyBlock][keyObject].value.substr(0, i).c_str()));
	else
		return sf::milliseconds(std::atof(_infoScene[keyType][keyBlock][keyObject].value.substr(0, i - 1).c_str()));
}