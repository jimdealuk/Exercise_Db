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

    class Command {
    public:
        virtual ~Command() {
        }
        virtual void Execute() const = 0;
    };


    class DisplayTagsCommand : public Command {

    private:
        ExerciseDataApp::ExerciseData* m_dataSource;

    public:
        DisplayTagsCommand(ExerciseDataApp::ExerciseData* receiver);
        void Execute() const override;
    };


    class DisplayExercisesCommand : public Command {

    private:
        ExerciseDataApp::ExerciseData* m_dataSource;

    public:
        DisplayExercisesCommand(ExerciseDataApp::ExerciseData* receiver);
        /**
         * Commands can delegate to any methods of a receiver.
         */
        void Execute() const override;
    };


    class AddTagsCommand : public Command {

    private:
        ExerciseDataApp::ExerciseData* m_dataSource;

    public:
        AddTagsCommand(ExerciseDataApp::ExerciseData* receiver);
        /**
         * Commands can delegate to any methods of a receiver.
         */
        void Execute() const override;
    };


    class SaveDbToFileCommand : public Command {

    private:
        ExerciseDataApp::ExerciseData* m_dataSource;

    public:
        SaveDbToFileCommand(ExerciseDataApp::ExerciseData* receiver);
        /**
         * Commands can delegate to any methods of a receiver.
         */
        void Execute() const override;
    };


    class AddExerciseCommand : public Command {

    private:
        ExerciseDataApp::ExerciseData* m_dataSource;

    public:
        AddExerciseCommand(ExerciseDataApp::ExerciseData* receiver);
        /**
         * Commands can delegate to any methods of a receiver.
         */
        void Execute() const override;
    };


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
