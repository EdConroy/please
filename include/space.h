#ifndef	__SPACE_H__
#define __SPACE_H__

// private class:
typedef struct Space_S Space;

Space *space_new();

/**
 * @brief remove space from memory
 * @param space to remove
 */
void space_free(Space *space);

/**
 * @brief set num for how many times space updates per frame
 * @param space the new number applies to
 * @param step number to use
 */
void space_set_steps(Space *space,int steps);

/**
 * @brief run the set number of steps, then update
 * @param space we are running
 */
void space_do_step(Space *space);

/**
 * @brief add a body to the list of bodies already in the space
 * @param space we are adding the new body to
 * @param body we are adding to the space
 */
void space_add_body(Space *space, struct Body_S *body);

#endif