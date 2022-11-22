// File associated with the Exercise_Db project of https://github.com/jimdealuk/

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
    private:
        // the exercise, tag and section containers populated from the 
        // exercises database
        std::unique_ptr<std::vector<CoreData::BaseEx>> m_exercises;
        std::unique_ptr<std::vector<std::string>> m_tags;
        std::unique_ptr<CoreData::WorkoutComponent> m_workoutComponents;

        bool ReadExerciseComponentsFromSection(std::string& sectionStr, CoreData::WorkoutComponent& sections);
        bool ReadExerciseComponentsFromWorkout(std::string& sectionStr, CoreData::WorkoutComponent& sections);

        bool WriteExercisesToFile(std::fstream& fs);
        bool WriteTagsToFile(std::fstream& fs);

        bool WriteExerciseComponentsToFile(std::fstream& fs);

    public:
        ExerciseDbFileHandler();
        ExerciseDbFileHandler(ExerciseDbFileHandler& exDb) = delete;
        virtual ~ExerciseDbFileHandler() = default;

        virtual bool ReadExDb();
        virtual bool WriteExDb();

        // Get the exercises, tags and section containers from the database
        virtual void GetExDb(std::vector<CoreData::BaseEx>& exDbHandle);
        virtual void GetExTags(std::vector<std::string>& tagsHandle);

        // Get list of workouts
        virtual void GetWorkouts(CoreData::WorkoutComponent& workoutHandle);

        // Set the exercises, tags and section containers in the database
        virtual void SetExTags(std::unique_ptr<std::vector<std::string>> tags);
        virtual void SetExercises(std::unique_ptr<std::vector<CoreData::BaseEx>> tags);

        // Set the workouts in the database
        virtual void SetWorkouts(std::unique_ptr<CoreData::WorkoutComponent> workouts);


    };
}

#endif /* !ExerciseDbFileHandler_H */