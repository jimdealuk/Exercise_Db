#include "..\hpp\ExercideDbFileHandlerFactory.h"
#include "..\hpp\ExerciseDbFileHandler.h"


namespace ExerciseDbFileHandlerFactory
{

	std::unique_ptr<ExerciseDbHandling::ExerciseDbHandlerBase> ExerciseDbFileHandlerFactory::FactoryMethod()
	{
		return std::unique_ptr<ExerciseDbHandling::ExerciseDbHandlerBase>(new ExerciseDbHandling::ExerciseDbFileHandler());
	}

}