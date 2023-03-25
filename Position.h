#include <math.h>

class Position {
public:
	float x = 0.0;
	float y = 0.0;
	float z = 0.0;

	Position(); // Constructor
	~Position(); // Destructor

	Position(const Position &current);
	Position(float x, float y, float z);
};
