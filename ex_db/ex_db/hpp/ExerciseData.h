#ifndef ExerciseData_H
#define ExerciseData_H

#include <memory>
#include <vector>

#include "ExerciseDb.h"
#include "ExerciseDbHandlerBase.h"

namespace ExerciseDataApp
{
    /* Wrapper round the exercise database :
    *  creates database and handles commands to-
    *  load / initialise the database
    *  handle commands to extract, update & save exercises
    */
    class ExerciseData
    {
    private:
        std::unique_ptr <ExerciseDbClass::ExerciseDb> m_exDb;

    public:
        ExerciseData() = default;
        ExerciseData(std::unique_ptr< ExerciseDbHandling::ExerciseDbHandlerBase> dbHandle);
        bool InitialiseExerciseDb();
        bool SaveExerciseDb();

        void GetTagsDb(std::vector<std::string>& tags);
        void GetExDb(std::vector<CoreData::BaseEx>& dbHandle);

        bool AddTag(std::string tag);

        bool CheckExerciseExists(std::string ex);
        bool CheckTagsExists(std::string ex);
        bool AddExercise(CoreData::BaseEx ex);
    };

}


#endif /* !ExerciseData_H */
