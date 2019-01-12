#include "parser.h"

ParserSceneInfo::ParserSceneInfo()
{
	std::cout << "create class ParserSceneInfo" << std::endl;
}

ParserSceneInfo::~ParserSceneInfo()
{
	//
}

void ParserSceneInfo::getInfoState(ParserSceneInfo &parser, int memGetter, const std::string &bufferFile)
{
	GET_STATE_INFO(parser, _gettersStateInfo[_currentState])(bufferFile);
}

std::vector<std::string> ParserSceneInfo::getAllIdsObjectFromBlock(const std::string & keyBlock)
{
	std::vector<std::string> allKeyBlock;
	
	for (auto it = _infoScene[keyBlock].begin(); it != _infoScene[keyBlock].end(); it++)
		allKeyBlock.push_back(it->first);

	return allKeyBlock;
}

bool ParserSceneInfo::init(const std::string &nameFile)
{
	//setting all Info State ps: letter, number and underscore are always meaning char, name is on SNAKE format except on one case, for the MORE_THAN_ONE_CHAR_FLAG like :{ for exemple
	_allStateInfo[STATE::NAME_BLOCK] = StateInfo{ "_", "\"", "\":{"};
	_gettersStateInfo[STATE::NAME_BLOCK] = &ParserSceneInfo::getNameBlock;
	_allStateInfo[STATE::NAME_INFO] = StateInfo{ "_", "\"", "\":{"};
	_gettersStateInfo[STATE::NAME_INFO] = &ParserSceneInfo::getNameInfo;
	_allStateInfo[STATE::TYPE_INFO] = StateInfo{ "_", "\"", "\","};
	_gettersStateInfo[STATE::TYPE_INFO] = &ParserSceneInfo::getTypeInfo;
	_allStateInfo[STATE::VALUE_INFO] = StateInfo{ "_ ,.;!", "\"", "\"}"};
	_gettersStateInfo[STATE::VALUE_INFO] = &ParserSceneInfo::getValueInfo;
	_currentState = STATE::NAME_BLOCK;

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

void ParserSceneInfo::getNameBlock(const std::string &bufferFile)
{
	_infoBlock.block = goToEndFlag(&_cursor, _allStateInfo[_currentState].endFlag, bufferFile, _allStateInfo[_currentState].meaningChar);
	_infoScene.insert(std::pair< std::string, std::map<std::string, InfoParser > >(_infoBlock.block, std::map<std::string, InfoParser >()));
	_currentState = STATE::NAME_INFO;
}

void ParserSceneInfo::getNameInfo(const std::string &bufferFile)
{
	_infoBlock.info = goToEndFlag(&_cursor, _allStateInfo[_currentState].endFlag, bufferFile, _allStateInfo[_currentState].meaningChar);
	_infoScene[_infoBlock.block].insert(std::pair<std::string, InfoParser >(_infoBlock.info, InfoParser()));
	_currentState = STATE::TYPE_INFO;
}

void ParserSceneInfo::getTypeInfo(const std::string &bufferFile)
{
	std::string tmpType;
	tmpType = goToEndFlag(&_cursor, _allStateInfo[_currentState].endFlag, bufferFile, _allStateInfo[_currentState].meaningChar);
	_infoScene[_infoBlock.block][_infoBlock.info].type = tmpType;
	_currentState = STATE::VALUE_INFO;
}

void ParserSceneInfo::getValueInfo(const std::string &bufferFile)
{
	std::string tmpInfo;
	tmpInfo = goToEndFlag(&_cursor, _allStateInfo[_currentState].endFlag, bufferFile, _allStateInfo[_currentState].meaningChar);
	_infoScene[_infoBlock.block][_infoBlock.info].value = tmpInfo;

	bool isLastInfo = false;
	while (bufferFile[_cursor] != LINK_VALUE)
	{
		_cursor++;
		if (bufferFile[_cursor] == END_BLOCK)
			isLastInfo = true;
		else if (bufferFile[_cursor] == FLAG_END_FILE)
		{
			_currentState = STATE::END_FILE;
			return;
		}
	}
	_currentState = (isLastInfo) ? STATE::NAME_BLOCK : STATE::NAME_INFO;
}

void ParserSceneInfo::setInfo(const std::string &keyBlock, const std::string &keyObject, float *value)
{
	(*value) = std::atof(_infoScene[keyBlock][keyObject].value.c_str());
}

void ParserSceneInfo::setInfo(const std::string &keyBlock, const std::string &keyObject, int *value)
{
	(*value) = std::atoi(_infoScene[keyBlock][keyObject].value.c_str());
}

void ParserSceneInfo::setInfo(const std::string &keyBlock, const std::string &keyObject, std::string *value)
{
	(*value) = _infoScene[keyBlock][keyObject].value;
	std::cout << (*value) << " parser\n";
}

void ParserSceneInfo::setInfo(const std::string &keyBlock, const std::string &keyObject, sf::Color *value)
{
	std::string tmp[4];
	int i = 0;
	std::string str(_infoScene[keyBlock][keyObject].value);
	for (std::string::const_iterator it = str.cbegin(); it != str.cend(); ++it)
		if (*it != LINK_VALUE)
			tmp[i].push_back(*it);
		else
			i++;
	(*value).r = std::atoi(tmp[0].c_str());
	(*value).g = std::atoi(tmp[1].c_str());
	(*value).b = std::atoi(tmp[2].c_str());
	(*value).a = std::atoi(tmp[3].c_str());
}

void ParserSceneInfo::setInfo(const std::string &keyBlock, const std::string &keyObject, sf::Vector2f *value)
{
	std::string tmp[2];
	int i = 0;
	std::string str(_infoScene[keyBlock][keyObject].value);
	for (std::string::const_iterator it = str.cbegin(); it != str.cend(); ++it)
		if (*it != LINK_VALUE)
			tmp[i].push_back(*it);
		else
			i++;
	(*value).x = std::atof(tmp[0].c_str());
	(*value).y = std::atof(tmp[1].c_str());
}

void ParserSceneInfo::setInfo(const std::string &keyBlock, const std::string &keyObject, sf::Vector2i *value)
{
	std::string tmp[2];
	int i = 0;
	std::string str(_infoScene[keyBlock][keyObject].value);
	for (std::string::const_iterator it = str.cbegin(); it != str.cend(); ++it)
		if (*it != LINK_VALUE)
			tmp[i].push_back(*it);
		else
			i++;
	(*value).x = std::atoi(tmp[0].c_str());
	(*value).y = std::atoi(tmp[1].c_str());
}

void ParserSceneInfo::setInfo(const std::string &keyBlock, const std::string &keyObject, sf::Time *value)
{
	std::string tmp[2];
	int i = 0;
	std::string str(_infoScene[keyBlock][keyObject].value);

	i = str.find("s");
	if (i == std::string::npos)
		(*value) = sf::seconds(0.f);
	else if (str[i - 1] != 'm')
		(*value) = sf::seconds(std::atof(_infoScene[keyBlock][keyObject].value.substr(0, i).c_str()));
	else
		(*value) = sf::milliseconds(std::atof(_infoScene[keyBlock][keyObject].value.substr(0, i - 1).c_str()));
}