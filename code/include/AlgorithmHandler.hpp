#include "Cat.hpp"

/** The informations to give to the fitness function about a `Cat` */
struct CatInfos {
	ID catID;
	uint64_t secondAlive;
};

/** An interface to run the genetic algorithm */
class AlogrithmHandler {
private:
	inline static constexpr float mutationProbability = .5;

public:
	static void	initialization(uint8_t);		//population size
	static void nextGen(void);

	static float fitnessFunction(CatInfos);
	static float evaluateAll(void);			//return sum of all fitnesses
	
	static Cat crossover(Cat, Cat);
	static Cat Mutate(Cat);
};