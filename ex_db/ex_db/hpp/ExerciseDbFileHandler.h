#ifndef ExerciseDbFileHandler_H
#define ExerciseDbFileHandler_H

#include <vector>

#include "CoreData.h"
#include "ExerciseDbHandlerBase.h"

/* Manipulate the exercise data source
*  In the implementation given - to & from a local file
*  It's intended that the ExerciseDbHandlerBase class
*  could be extended to use an SQL database (for example)
*/

namespace ExerciseDbHandling
{

    /* Class : ExerciseDbFileHandler
    *  Implementation of ExerciseDbHandlerBase
    *  This class handles accessing & storing data from a file.
    */
    class ExerciseDbFileHandler : public ExerciseDbHandlerBase
    {
        std::unique_ptr<std::vector<CoreData::BaseEx>> m_exercises;
        std::unique_ptr<std::vector<std::string>> m_tags;
        std::unique_ptr<std::vector<std::string>> m_workoutSectiontags;

    public:
        ExerciseDbFileHandler();
        ExerciseDbFileHandler(ExerciseDbFileHandler& exDb) = delete;
        virtual bool ReadExDb();
        virtual bool WriteExDb();

        virtual void GetExDb(std::vector<CoreData::BaseEx>& exDbHandle);
        virtual void GetExTags(std::vector<std::string>& tagsHandle);
        virtual void GetWorkoutSection(std::vector<std::string>& tagsHandle);

        virtual void SetExTags(std::unique_ptr<std::vector<std::string>> tags);
        virtual void SetExercises(std::unique_ptr<std::vector<CoreData::BaseEx>> tags);
        virtual void SetWorkoutSection(std::unique_ptr<std::vector<std::string>> tags);

        virtual ~ExerciseDbFileHandler() = default;

    };
}

#endif /* !ExerciseDbFileHandler_H */