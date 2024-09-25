#include "gamedata.h"

bool Map::CoinGen() {
	srand(time(0));
	
	for (int y = 1; y < height+1; y++) {
		for (int x=1; x < width+1; x++) {
			if ((x-1 == 0) && (y-1 == 0)) continue; // Skip player position
			if (arrMap[y][x] == COIN) continue; // Skip if coin already placed
			if (arrMap[y][x] != SPACE) continue; // ENSURES ONLY SPACES ARE TAKEN
			
			//=====================================
			// CHECKS SURROUNDING COINS
			//=====================================
			int surrounding_coins = 0;
			// Counts surrounding coins
			for (int sy=-3; sy<=3; sy++) {
				for (int sx=-3; sx<=3; sx++){
					if (sx == 0 && sy == 0) continue; // skips area which we asses
					
					int object = getObject(y+sy, x+sx);
					if (object == COIN) {
						++surrounding_coins;
					}

				}
			}	

			// Calculates probability of coin placed based on surrounding coins
			int probability = surrounding_coins+11;
			probability = rand() % probability;
			// Ensures coin generation not too rare
			if (probability > 200) {
				probability = (rand() % 201);
			}	

			// Randomly places coins		
			if (probability == 1) {
				++coinsLeft;
				arrMap[y][x] = COIN;
			}

			if (coinsLeft == coins) return true; // If all coins generated we exit
		}
	}

	return false; // Not all coins where generated
}

void Map::pickCoin() {
	--coinsLeft;
	
	//Message("Coins: " + std::to_string(coins) + "\nCoins Left: " + std::to_string(coinsLeft));
}
