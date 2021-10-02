#include "Config.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string Config::configPath = "default.conf";
std::string Config::archiveImagePath = "archive/";
std::string Config::archiveFilePath = "archive/file/";
std::string Config::imagePath = "images/";

int Config::width = 1600;
int Config::height = 900;
int Config::numberOfBlocksRow = 16;
int Config::numberOfBlocksColumn = 9;
int Config::timeLimitation = 300;
int Config::propFrequency = 10;
int Config::playerNumber = 1;
int Config::hintDuration = 10;
int Config::freezeDuration = 3;
int Config::dizzyDuration = 10;
int Config::animationDuration = 1;
int Config::fps = 24;
int Config::flashDuration = 5;

int Config::menuWidth = 800;
int Config::menuHeight = 600;
int Config::menuButtonWidth = 200;
int Config::menuButtonHeight = 50;
QFont Config::menuButtonFont = QFont("宋体",20);

void Config::load(std::string path)
{
    std::ifstream file(path);
    if(file.is_open()){
        std::string variable;
        int value;
        std::string content;
        std::stringstream contentStream;
        while(std::getline(file,content)){
            contentStream.clear();
            contentStream<<content;
            contentStream>>variable;
            contentStream>>value;
            if(variable == "width"){
                width = value;
                continue;
            }
            if(variable == "height"){
                height = value;
                continue;
            }
            if(variable == "numberOfBlocksRow"){
                numberOfBlocksRow = value;
                continue;
            }
            if(variable == "numberOfBlocksColumn"){
                numberOfBlocksColumn = value;
                continue;
            }
            if(variable == "propFrequency"){
                propFrequency = value;
                continue;
            }
            if(variable == "timeLimitation"){
                timeLimitation = value;
                continue;
            }
            if(variable == "playerNumber"){
                playerNumber = value;
                continue;
            }
            if(variable == "hintDuration"){
                hintDuration = value;
                continue;
            }
            if(variable == "animationDuration"){
                animationDuration = value;
                continue;
            }
            if(variable == "flashDuration"){
                flashDuration = value;
                continue;
            }
            if(variable == "fps"){
                fps = value;
                continue;
            }
            if(variable == "freezeDuration"){
                freezeDuration = value;
                continue;
            }
            if(variable == "dizzyDuration"){
                dizzyDuration = value;
                continue;
            }

        }
    }
}

void Config::save(std::string path)
{
    std::ofstream f(path);
    f<<"width "<<width<<std::endl;
    f<<"height "<<height<<std::endl;
    f<<"numberOfBlocksRow "<<numberOfBlocksRow<<std::endl;
    f<<"numberOfBlocksColumn "<<numberOfBlocksColumn<<std::endl;
    f<<"timeLimitation "<<timeLimitation<<std::endl;
    f<<"propFrequency "<<propFrequency<<std::endl;
    f<<"playerNumber "<<playerNumber<<std::endl;
    f<<"hintDuration "<<hintDuration<<std::endl;
    f<<"freezeDuration "<<freezeDuration<<std::endl;
    f<<"dizzyDuration "<<dizzyDuration<<std::endl;
    f<<"animationDuration "<<animationDuration<<std::endl;
    f<<"fps "<<fps<<std::endl;
    f<<"flashDuration "<<flashDuration<<std::endl;
}
