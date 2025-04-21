#include "../include/AlgorithmHandler.hpp"

void AlogrithmHandler::initialization(uint8_t populationSize) {
	for (ID i = 0; i < populationSize; i++) {
		new Cat(
			Pos::SCREEN_CENTER,
			(uint)randInt(1, 10),
			(uint)randInt(50, 200),
			0
		);
	}
}