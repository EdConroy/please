//#include <glib.h>
#include <stdlib.h>
#include "vector.h"
#include "space.h"

struct Space_S
{
	//GList *bodyList;	/**<hash map of all bodies in space*/

	int steps;			/**<steps to take before update everything*/
	int stepsTaken;		/**<steps that have been taken since the last update*/

	float stepFactor;

	Vec3D gravity;		/**<um... gravity.*/
};

Space *space_init()
{
	Space *space;
	space = (Space *)calloc(1, sizeof(struct Space_S));
	return space;
}