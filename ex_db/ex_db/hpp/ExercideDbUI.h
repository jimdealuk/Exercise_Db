#ifndef ExercideDbUI_H
#define ExercideDbUI_H

namespace ExercideDbUI
{

    class Command {
    public:
        virtual ~Command() {
        }
        virtual void Execute() const = 0;
    };

}

#endif /* !ExercideDbUI_H */
