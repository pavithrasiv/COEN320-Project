#include "Position.h"

Position::Position() {
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

Position::Position(const Position &current) {
	x = current.x;
	y = current.y;
	z = current.z;
}

Position::Position(float x1, float y1, float z1) {
	x = x1;
	y = y1;
	z = z1;
}

Position::~Position() {

}
