#include <string.h>
#include <stdlib.h>
#include <SDL.h>
#include <GL\glew.h>
#include "mgl_callback.h"
#include "collisions.h"
#include "entity.h"

MglCallback mgl_callback(void (*function)(void *data,void *context),void *data)
{
    MglCallback cb;
    cb.function = function;
    cb.data = data;
    return cb;
}

void mgl_callback_copy(MglCallback *dst,MglCallback src)
{
    if (!dst)return;
    dst->function = src.function;
    dst->data = src.data;
}

void mgl_callback_set(MglCallback *cb,void (*function)(void *data,void *context),void *data)
{
    if (!cb)return;
    cb->function = function;
    cb->data = data;
}

MglCallback *mgl_callback_new()
{
    MglCallback *cb;
    cb = (MglCallback *)malloc(sizeof(MglCallback));
    memset(cb,0,sizeof(MglCallback));
    return cb;
}

void mgl_callback_free(MglCallback **cb)
{
    if (!cb)return;
    if (!*cb)return;
    free(*cb);
    *cb = NULL;
}

void mgl_callback_delete(MglCallback *cb)
{
    if (!cb)return;
    free(cb);
}

/* actual callback function???
void touch_callback(void *data, void *context)
{
    struct Entity_S *me,*other;
    struct Body_S *obody;
    if ((!data)||(!context))return;
    me = (struct Entity_S *)data;
    obody = (struct Body_S *)context;
    if (entity_is_entity(obody->touch.data))
    {
        other = (struct Entity_S *)obody->touch.data;
        //slog("%s is ",other->name);
    }
    //slog("touching me.... touching youuuuuuuu");
}

// move back!
void move_callback(void* data, void *context)
{
	Entity *me, *other;
	Body *obody;

	if((!data)||(!context))return;

	me = (Entity *)data;
	obody = (Body *)context;

	if(entity_is_entity(obody->touch.data))
	{
		other = (Entity *)obody->touch.data;
		//slog("%s", me->name);
		//other->body.velocity.x = 0.5;
	}

}
*/

/*eol@eof*/
