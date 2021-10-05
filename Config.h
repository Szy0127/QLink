#ifndef CONFIG_H
#define CONFIG_H

#endif // CONFIG_H

#include <string>
#include <QFont>
class Config final
{


public:
    static std::string configPath;
    static std::string archiveFilePath;
    static std::string archiveImagePath;
    static std::string imagePath;
    static int fps;
    static int width;
    static int height;

    static int blockSize;
    static int numberOfBlocksRow;
    static int numberOfBlocksColumn;

    static int playerNumber;

    static int addTime;
    static int timeLimitation;//seconds
    static int propFrequency;//seconds

    static int hintDuration;//seconds
    static int animationDuration;//seconds

    static int flashDuration;//seconds

    static int freezeDuration;
    static int dizzyDuration;

    static int menuWidth;
    static int menuHeight;
    static int menuButtonWidth;
    static int menuButtonHeight;

    static QFont menuButtonFont;
    static QFont settingLabelFont;

    static int settingWidth;
    static int settingHeight;

    static int gameoverID;
    static int settingSubmitID;

    static void load(std::string path);
    static void save(std::string path);
};
