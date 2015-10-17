#include <glib.h>
#include <stdio.h>
#include "phys.h"
#include "collisions.h"
#include "simple_logger.h"

// rename 'class'?

GList* __bodyList;

// add bodies to physics
void physics_add_body(Body *body)
{
	if (!body) return;
	__bodyList = g_list_append(__bodyList, body);
}

void physics_remove_body(Body *body)
{
	if (!body) return;
	__bodyList = g_list_remove(__bodyList, body);
}

// check all collisions, calls touch functions
void physics_collision(Body *body)
{
	GList *it;

	Cube a,b; // 3d bounding boxes
	Body *other;

	// vec3d_cpy(a, a, body->position.x + body->bounds)
	a.x = body->position.x + body->bounds.x;
	a.y = body->position.y + body->bounds.y;
    a.z = body->position.z + body->bounds.z;

	a.w = body->bounds.w;
    a.h = body->bounds.h;
    a.d = body->bounds.d;

	for (it = __bodyList; it != NULL; it = g_list_next(it))
	{
		if (!it->data) continue;
		if (it->data == body) continue;

		// cast data as a body, because it is from __bodyList
		other = (Body*) it->data;

		vec3d_cpy(b, other->position);

		b.w = other->bounds.w;
        b.h = other->bounds.h;
        b.d = other->bounds.d;
		
		vec3d_add(b,b,other->bounds);

		// bounding box collision detection
		if (cube_cube_intersection(a, b))
		{
			if (other->owner)
			{
				// touch/callback 1 for floor collision
				if (strcmp(other->owner->name, "floor1") == 0)
					if (strcmp(body->owner->name, "player") == 0)
					{
						body->done = 1;
					}

				// touch/callback 2 for #knife attack
				if (strcmp(other->owner->name, "obstacle1") == 0)
					if (strcmp(body->owner->name, "player") == 0)
					{
						if (body->owner->inventory[0].attack)
							ent_free(other->owner);
					}

				// touch/callback 3 for firearm attack
				if (strcmp(other->owner->name, "obstacle1") == 0)
					if (strcmp(body->owner->name, "player") == 0)
					{
						if (body->owner->inventory[1].attack)
							ent_free(other->owner);
					}

				// touch/callback 3 for throw attack
				if (strcmp(other->owner->name, "obstacle1") == 0)
					if (strcmp(body->owner->name, "player") == 0)
					{
						if (body->owner->inventory[2].attack)
							other->owner->think = thnk_push;
					}
			}
		}

	}
}