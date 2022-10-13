// File associated with the Exercise_Db project of https://github.com/jimdealuk/

#ifndef ExercideDbUI_H
#define ExercideDbUI_H

#include "../hpp/ExerciseData.h"

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
        ExerciseDataApp::ExerciseData* m_dataSource;

    public:
        DisplayTagsCommand(ExerciseDataApp::ExerciseData* receiver);
        void Execute() const override;
    };


    /* DisplayExercisesCommand class
    *  This class displays a list of the exercises in the exercises database
    */
    class DisplayExercisesCommand : public Command {

    private:
        ExerciseDataApp::ExerciseData* m_dataSource;

    public:
        DisplayExercisesCommand(ExerciseDataApp::ExerciseData* receiver);
        void Execute() const override;
    };


    /* AddTagsCommand class
    *  This class handles adding a new tag to the tags database
    */
    class AddTagsCommand : public Command {

    private:
        ExerciseDataApp::ExerciseData* m_dataSource;

    public:
        AddTagsCommand(ExerciseDataApp::ExerciseData* receiver);
        void Execute() const override;
    };


    /* SaveDbToFileCommand class
    *  This class handles saving the tag and exercise storage containers
    *  to the file in which they are taken from at start up
    */
    class SaveDbToFileCommand : public Command {

    private:
        ExerciseDataApp::ExerciseData* m_dataSource;

    public:
        SaveDbToFileCommand(ExerciseDataApp::ExerciseData* receiver);
        void Execute() const override;
    };


    /* AddExerciseCommand class
    *  This class handles adding a new exercise to the exercise database
    */
    class AddExerciseCommand : public Command {

    private:
        ExerciseDataApp::ExerciseData* m_dataSource;

    public:
        AddExerciseCommand(ExerciseDataApp::ExerciseData* receiver);
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
        std::shared_ptr <ExerciseDataApp::ExerciseData> m_exDb;

    public:
        Invoker() = default;
        Invoker(Invoker& inv) = delete;
        Invoker(std::shared_ptr<ExerciseDataApp::ExerciseData> receiver);

        void StartUI();
    };

}

#endif /* !ExercideDbUI_H */
