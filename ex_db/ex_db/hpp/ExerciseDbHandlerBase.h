// File associated with the Exercise_Db project of https://github.com/jimdealuk/

#ifndef ExerciseDbHandlerBase_H
#define ExerciseDbHandlerBase_H

#include <vector>

#include "CoreData.h"

namespace ExerciseDbHandling
{

    /* Base Class : ExerciseDbHandling
    *  Interface class to be inherited from for handling
    *  the exercise data source.
    *  This structure is based on the Dependency Injection
    *  design pattern.
    *  The idea here is that different storage mechanisms
    *  could be used to store the exercise data.
    */
    class ExerciseDbHandlerBase
    {
    public:
        virtual bool ReadExDb() = 0;
        virtual bool WriteExDb() = 0;

        virtual void GetExDb(std::vector<CoreData::BaseEx>& exDbHandle) = 0;
        virtual void GetExTags(std::vector<std::string>& tagsHandle) = 0;
        virtual void GetWorkout(std::vector<CoreData::Workout>& tagsHandle) = 0;

        virtual void SetExTags(std::unique_ptr<std::vector<std::string>> tags) = 0;
        virtual void SetExercises(std::unique_ptr<std::vector<CoreData::BaseEx>> tags) = 0;
        virtual void SetWorkout(std::unique_ptr<std::vector<CoreData::Workout>> tags) = 0;

        virtual ~ExerciseDbHandlerBase() = default;
    };

}

#endif /* !ExerciseDbHandlerBase_H */