#ifndef CONFIG_H
#define CONFIG_H

#endif // CONFIG_H

#include <string>
#include <QFont>
class Config final
{


public:
    static std::string defaultPath;
    static int fps;
    static int width;
    static int height;
    static int numberOfBlocksRow;
    static int numberOfBlocksColumn;

    static int playerNumber;

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
    static void load(std::string path);
    static void save(std::string path);
};
