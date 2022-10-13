#ifndef CoreData_H
#define CoreData_H

#include <string>

namespace CoreData
{
    /* These are strings that are used throughout the application
    */
    const std::string filePath = { "../exercisedb/" };
    const std::string file = { "../exercisedb/exercises.db" };

    const std::string dataIn = { "{" };
    const std::string dataOut = { "}" };
    const std::string lineSep = { "\n" };

    const std::string sepIn = { "(" };
    const std::string sepOut = { ")" };
    const std::string exSep = { ":" };
    const std::string tagSep = { "," };

    const std::string tagBase = { "Exercises" };
    const std::string tagTags = { "Tags" };
    const std::string sectionTags = { "Sections" };

    /* Data Type : BaseEx
    *  Structure containing exercise name and it's associated tags
    *  Tags here can be considered categories : what kind of workouts
    *  would you associate this exercise with.
    *  ex: the Squat would be associated with "Power" and "General" types
    *  of workout
    */
    struct BaseEx
    {
        std::string exName;
        std::vector<std::string> exTags;
    };

    struct ExDescription
    {
        std::string exName;
        int exSets;
        int exWeight;
        int exReps;
    };



}

#endif /* !CoreData_H */