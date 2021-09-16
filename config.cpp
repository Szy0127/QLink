#include "Config.h"
#include <fstream>
#include <sstream>
//#include <iostream>
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

}
