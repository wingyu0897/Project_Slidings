#include "BranchPath.h"

BranchPath::BranchPath(int life, POINT pathStart) : life{ life }, path{ pathStart }
{
	prevPath = path;
}

BranchPath::~BranchPath()
{
}
