#ifndef ExerciseDbFileHandlerFactory_H
#define ExerciseDbFileHandlerFactory_H

#include <vector>

#include "CoreData.h"

#include "ExerciseDbFactoryBase.h"

namespace ExerciseDbFileHandlerFactory
{
	/* Class : ExerciseDbFileHandlerFactory
	*  Implementation of ExerciseDbFactoryBase
	* 
	*  In this case the factory would return a "database" based on using file
	*  as the source of workout / exercise data.
	*/
	class ExerciseDbFileHandlerFactory : public ExerciseDbFactoryBase::ExerciseDbFactoryBase
	{
	public:

	public:
		ExerciseDbFileHandlerFactory() = default;
		ExerciseDbFileHandlerFactory(ExerciseDbFileHandlerFactory& exDb) = default;
		~ExerciseDbFileHandlerFactory() = default;

		std::unique_ptr<ExerciseDbHandling::ExerciseDbHandlerBase> FactoryMethod() override;

	};
}
#endif /* !ExerciseDbFileHandlerFactory_H */
