// File associated with the Exercise_Db project of https://github.com/jimdealuk/

#ifndef ExercideDbUI_H
#define ExercideDbUI_H

#include "ExerciseDb.h"
#include "WorkoutBuilder.h"


/* Creates Application "UI"
*  all commands to interact with exercise database
*  invoker that handles the creation of base "UI"
*  creation and execution of the commands
*
*  ALL commands classes implement Command base class.
*/
namespace ExercideDbUI
{
    /* Command class
    *  Describes operations that all commands have to implement
    */
    class Command {
    public:
        virtual ~Command() {
        }
        virtual void Execute() const = 0;
    };

    /* DisplayTagsCommand class
    *  This class displays a list of the tags in the tags database
    */
    class DisplayTagsCommand : public Command {

    private:
        ExerciseDbClass::ExerciseDb* m_dataSource;

    public:
        DisplayTagsCommand(ExerciseDbClass::ExerciseDb* receiver);
        void Execute() const override;
    };


    /* DisplayExercisesCommand class
    *  This class displays a list of the exercises in the exercises database
    */
    class DisplayExercisesCommand : public Command {

    private:
        ExerciseDbClass::ExerciseDb* m_dataSource;

    public:
        DisplayExercisesCommand(ExerciseDbClass::ExerciseDb* receiver);
        void Execute() const override;
    };


    /* AddTagsCommand class
    *  This class handles adding a new tag to the tags database
    */
    class AddTagsCommand : public Command {

    private:
        ExerciseDbClass::ExerciseDb* m_dataSource;

    public:
        AddTagsCommand(ExerciseDbClass::ExerciseDb* receiver);
        void Execute() const override;
    };


    /* SaveDbToFileCommand class
    *  This class handles saving the tag and exercise storage containers
    *  to the file in which they are taken from at start up
    */
    class SaveDbToFileCommand : public Command {

    private:
        ExerciseDbClass::ExerciseDb* m_dataSource;

    public:
        SaveDbToFileCommand(ExerciseDbClass::ExerciseDb* receiver);
        void Execute() const override;
    };


    /* AddExerciseCommand class
    *  This class handles adding a new exercise to the exercise database
    */
    class AddExerciseCommand : public Command {

    private:
        ExerciseDbClass::ExerciseDb* m_dataSource;

    public:
        AddExerciseCommand(ExerciseDbClass::ExerciseDb* receiver);
        void Execute() const override;
    };

    /* GetExerciseForTagCommand class
    *  This class handles getting a list of exercises for a tag
    */
    class GetExerciseForTagCommand : public Command {

    private:
        ExerciseDbClass::ExerciseDb* m_dataSource;

    public:
        GetExerciseForTagCommand(ExerciseDbClass::ExerciseDb* receiver);
        void Execute() const override;
    };

    /* CreateWorkoutCommand class
    *  This class handles getting creating a workout
    */
    class CreateWorkoutCommand : public Command {

    private:
        ExerciseDbClass::ExerciseDb* m_dataSource;
        WorkoutBuilder::BuildWorkoutImpl* m_workoutBuilder;

    public:
        CreateWorkoutCommand(ExerciseDbClass::ExerciseDb* receiver, WorkoutBuilder::BuildWorkoutImpl* workoutBuilder);
        void Execute() const override;
    };


    /* Invoker class
    *  This class handles the user interface
    *  It - gives options on adding and listing tags & exercises to the
    *  user and then allows then to update the storage with new tags and
    *  exercises.
    *  In order to do this it creates and executes the relevant commands
    */
    class Invoker {
        std::shared_ptr <ExerciseDbClass::ExerciseDb> m_exDb;

    public:
        Invoker() = default;
        Invoker(Invoker& inv) = delete;
        Invoker(std::shared_ptr<ExerciseDbClass::ExerciseDb> receiver);

        void StartUI();
    };

}

#endif /* !ExercideDbUI_H */
