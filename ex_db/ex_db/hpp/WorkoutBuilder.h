// File associated with the Exercise_Db project of https://github.com/jimdealuk/

#ifndef WorkoutBuilder_H
#define WorkoutBuilder_H

#include <string>


#include "CoreData.h"


namespace WorkoutBuilder
{
    class WorkoutBuilder
    {
    public:
        virtual void BuildWorkout(std::string& workoutName) = 0;
        virtual void BuildSections(std::string& workoutName, CoreData::Component* section) = 0;
        virtual void BuildExerciseList(std::string& sectionName, CoreData::Component* ex) = 0;
    };

    class BuildWorkoutImpl :public WorkoutBuilder
    {
    private:
        std::shared_ptr<CoreData::WorkoutComponent> m_workouts;

    public:

        BuildWorkoutImpl();
        BuildWorkoutImpl(BuildWorkoutImpl& b) = delete;
        ~BuildWorkoutImpl() = default;

        virtual void BuildWorkout(std::string& workoutName);
        virtual void BuildSections(std::string& workoutName, CoreData::Component* section);
        virtual void BuildExerciseList(std::string& sectionName, CoreData::Component* ex);
    };



    class BuildFullWorkout :public BuildWorkoutImpl
    {
    private:
        std::shared_ptr<BuildWorkoutImpl> m_builder;

    private:
        BuildFullWorkout() = default;
        BuildFullWorkout(BuildFullWorkout& b) = delete;
        ~BuildFullWorkout() = default;

        void BuildWorkout(CoreData::Component& workout);
        void BuildSections(CoreData::Component& section, CoreData::Component& workout);
        void BuildExerciseList(std::string& sectionName, CoreData::Component* ex);
    };

    class BuildQuickWorkout :public BuildWorkoutImpl
    {
    private:
        std::shared_ptr<BuildWorkoutImpl> m_builder;

    private:
        BuildQuickWorkout() = default;
        BuildQuickWorkout(BuildQuickWorkout& b) = delete;
        ~BuildQuickWorkout() = default;

        void BuildWorkout(CoreData::Component& workout);
        void BuildExerciseList(std::string& sectionName, CoreData::Component* ex);
    };




}


#endif /* !WorkoutBuilder_H */

