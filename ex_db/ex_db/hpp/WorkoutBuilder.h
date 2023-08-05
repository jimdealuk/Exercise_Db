// File associated with the Exercise_Db project of https://github.com/jimdealuk/

#ifndef WorkoutBuilder_H
#define WorkoutBuilder_H

#include <string>

#include "CoreData.h"


namespace WorkoutBuilder
{

    /* Interface Class : WorkoutBuilder
    * Interface for a class to build workouts out of workout sections.
    * This interface uses the CoreData::Component class in order to
    * create / build up workouts.
    * 
    * A workout has a name and a list of sections (CoreData::Component)
    * A section has a name and a list of exercises (CoreData::Component)
    * An exercise has a name and a number of attributes
    */
    class WorkoutBuilder
    {
    public:
        virtual void BuildWorkout(std::string& workoutName) = 0;
        virtual void BuildSections(std::string& workoutName, CoreData::Component* section) = 0;
        virtual void BuildExerciseList(std::string& sectionName, CoreData::Component* ex) = 0;
    };


    /* Class : BuildWorkoutImpl
    *  Implementation of WorkoutBuilder
    *  Base case of building a workout up
    *  Create & name a workout
    *  Add section(s) to a workout
    *  Add exercise(s) to a section
    */
    class BuildWorkoutImpl :public WorkoutBuilder
    {
    private:
        CoreData::WorkoutComponent* m_workouts;

    public:

        BuildWorkoutImpl();
        BuildWorkoutImpl(BuildWorkoutImpl& b) = delete;
        ~BuildWorkoutImpl() = default;

        virtual void BuildWorkout(std::string& workoutName);
        virtual void BuildSections(std::string& workoutName, CoreData::Component* section);
        virtual void BuildExerciseList(std::string& sectionName, CoreData::Component* ex);

        CoreData::WorkoutComponent* GetCreatedWorkout();
    };


    /* Class : BuildFullWorkout
    *  Use BuildWorkoutImpl to build a workout that contains
    *  A "default" workout, containing warmups, exercises and stretches
    */
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

    /* Class : BuildRehabWorkout
    *  Use BuildWorkoutImpl to build a workout that contains
    *  a cut down workout with no sections.
    * for example: this could be used to contain only rehab exercises
    */
    class BuildRehabWorkout :public BuildWorkoutImpl
    {
    private:
        std::shared_ptr<BuildWorkoutImpl> m_builder;

    private:
        BuildRehabWorkout() = default;
        BuildRehabWorkout(BuildRehabWorkout& b) = delete;
        ~BuildRehabWorkout() = default;

        void BuildWorkout(CoreData::Component& workout);
        void BuildExerciseList(std::string& sectionName, CoreData::Component* ex);
    };
}


#endif /* !WorkoutBuilder_H */

