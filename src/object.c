#include <stdio.h>
#include <stdlib.h>
#include "object.h"
#include "simple_logger.h"


#define __obj_max 1024
#define __model_max 1024

static Obj ObjList[__obj_max];

static void obj_deInit_all();

static Model ModelList[__model_max];

static void model_close();

void obj_init_all()
{
    memset(ObjList,0,sizeof(Obj)*__obj_max);
    atexit(obj_deInit_all);
}

Obj *obj_init()
{
    int i;
    for (i = 0; i < __obj_max; i++)
    {
		if (ObjList[i].inuse == 0)
        {
            memset(&ObjList[i],0,sizeof(Obj));
			ObjList[i].inuse = 1;
            return &ObjList[i];
        }
    }
    return NULL;
}

Obj *obj_get_by_filename(char *filename)
{
    int i;
    for (i = 0; i < __obj_max; i++)
    {
		if ((ObjList[i].inuse != 0) &&
            (strcmp(ObjList[i].filename,filename) == 0))
        {
            return &ObjList[i];
        }
    }
    return NULL;
}

static void obj_delete(Obj *obj)
{
    if (!obj)return;
    
    if (obj->vertex_array)
    {
        free(obj->vertex_array);
    }
    if (obj->texel_array)
    {
        free(obj->texel_array);
    }
    if (obj->normal_array)
    {
        free(obj->normal_array);
    }
    if (obj->triangle_array)
    {
        free(obj->triangle_array);
    }
    memset(obj,0,sizeof(Model)); // ???
}

void obj_free(Obj *obj)
{
    if (!obj)return;
	obj->inuse--;
	if (obj->inuse > 0)return;
    obj_delete(obj);
}

static void obj_deInit_all()
{
    int i;
    for (i = 0; i < __obj_max; i++)
    {
		if (ObjList[i].inuse)
        {
            obj_delete(&ObjList[i]);
        }
    }
}

void obj_file_get_counts(Obj* model, FILE* file)
{
    char buf[1024];
    int  numvertices = 0;
    int  numnormals = 0;
    int  numtexcoords = 0;
    int  numfaces = 0;
    
    if ((file == NULL) ||
        (model == NULL))
    {
        return;
    }
    while(fscanf(file, "%s", buf) != EOF)
    {
        switch(buf[0])
        {
            case 'v':
                switch(buf[1])
                {
                    case '\0':
                        fgets(buf, sizeof(buf), file);
                        numvertices++;
                        break;
                    case 'n':
                        fgets(buf, sizeof(buf), file);
                        numnormals++;
                        break;
                    case 't':
                        fgets(buf, sizeof(buf), file);
                        numtexcoords++;
                        break;
                    default:
                        break;
                }
                break;
            case 'f':
                fgets(buf, sizeof(buf), file);
                numfaces++;
                break;
            default:
                fgets(buf, sizeof(buf), file);
                break;
        }
    }
    model->num_tris = numfaces;
    model->num_vertices = numvertices;
    model->num_texels = numtexcoords;
    model->num_normals = numnormals;
}

void obj_allocate(Obj *model) // ???
{
    if (!model)return;
    if (model->num_vertices)
    {
        model->vertex_array = malloc(sizeof(float)*3*model->num_vertices);
        if (model->vertex_array)
        {
            memset(model->vertex_array,0,sizeof(float)*3*model->num_vertices);
        }
    }
    if (model->num_normals)
    {
        model->normal_array = malloc(sizeof(float)*3*model->num_normals);
        if (model->normal_array)
        {
            memset(model->normal_array,0,sizeof(float)*3*model->num_normals);
        }
    }
    if (model->num_texels)
    {
        model->texel_array = malloc(sizeof(float)*2*model->num_texels);
        if (model->texel_array)
        {
            memset(model->texel_array,0,sizeof(float)*2*model->num_texels);
        }
    }
    if (model->num_tris)
    {
        model->triangle_array = malloc(sizeof(ObjTriangle)*model->num_tris);
        if (model->triangle_array)
        {
            memset(model->triangle_array,0,sizeof(ObjTriangle)*model->num_tris);
        }
    }
}

void obj_file_parse(Obj * model, FILE* file)
{
    int  numvertices = 0;
    int  numnormals = 0;
    int  numtexcoords = 0;
    int  numfaces = 0;
    char buf[128];
    float x,y,z;
    GLuint t1,t2,t3,v1,v2,v3,n1,n2,n3;
        
    if (model == NULL)
    {
        return;
    }
    if (file == NULL)
    {
        return;
    }
    rewind(file);
    while(fscanf(file, "%s", buf) != EOF)
    {
        switch(buf[0])
        {
            case 'v':
                switch(buf[1])
                {
                    case '\0':
                        fscanf(
                            file,
                            "%f %f %f",
                            &x,
                            &y,
                            &z
                        );
                        model->vertex_array[numvertices++] = x;
                        model->vertex_array[numvertices++] = y;
                        model->vertex_array[numvertices++] = z;
                        break;
                    case 'n':
                        fscanf(
                            file,
                            "%f %f %f",
                            &x,
                            &y,
                            &z
                        );
                        model->normal_array[numnormals++] = x;
                        model->normal_array[numnormals++] = y;
                        model->normal_array[numnormals++] = z;
                        break;
                    case 't':
                        fscanf(
                            file,
                            "%f %f",&x,&y
                        );
                        model->texel_array[numtexcoords++] = x;
                        model->texel_array[numtexcoords++] = 1 - y;
                        break;
                    default:
                        break;
                }
                break;
            case 'f':
                fscanf(
                    file,
                    "%u/%u/%u %u/%u/%u %u/%u/%u",
                    &v1,
                    &t1,
                    &n1,
                    &v2,
                    &t2,
                    &n2,
                    &v3,
                    &t3,
                    &n3);
                model->triangle_array[numfaces].p[0].v = v1 - 1;
                model->triangle_array[numfaces].p[0].t = t1 - 1;
                model->triangle_array[numfaces].p[0].n = n1 - 1;
                model->triangle_array[numfaces].p[1].v = v2 - 1;
                model->triangle_array[numfaces].p[1].t = t2 - 1;
                model->triangle_array[numfaces].p[1].n = n2 - 1;
                model->triangle_array[numfaces].p[2].v = v3 - 1;
                model->triangle_array[numfaces].p[2].t = t3 - 1;
                model->triangle_array[numfaces].p[2].n = n3 - 1;
                numfaces++;
                break;
            default:
                /*chomp the line*/
                fgets(buf, sizeof(buf), file);
                break;
        }
    }
}

int obj_va_search(float *v,float *a,int count,float vtarget[3],float atarget[6])
{
    int i;
    if (!v || !a)
    {
        slog("error, missing a buffer to search");
        return -1;/*not found*/
    }
    for (i = 0; i < count;i++)
    {
        if (memcmp(&v[i * 3],vtarget,sizeof(float)*3) != 0)
        {
            /*not equal, keep searching*/
            continue;
        }
        if (memcmp(&a[i * 3],atarget,sizeof(float)*6) != 0)
        {
            /*not equal, keep searching*/
            continue;
        }
        return i;
    }
    return -1;/*not found*/
}

int obj_file_convert_to_model(Obj *objFile,Model *model)
{
    int i,j,count = 0;
    int search = 0;
    float *vertices; /**<working vertex buffer*/
    float *attributes; /**<working attribute buffer*/
    float *vtarget,atarget[6];
    if ((!model) || (!objFile))
    {
        slog("passed null data!");
        return -1;
    }
    /*go through each triangle, building one synchronous vertex buffer and one attribute buffer*/
    vertices = (float *)malloc(sizeof(float) * objFile->num_tris * 3 * 3);
    attributes = (float *)malloc(sizeof(float) * objFile->num_tris * 3 * 3 * 2);/*packing in normals and uvs*/
    if ((!vertices) || (!attributes))
    {
        slog("failed to alloacte vertex / attribute buffers!");
        if (vertices)free(vertices);
        if (attributes)free(attributes);
        return -1;
    }
    
    memset(atarget,0,sizeof(float)*6);
    if (model_allocate_triangle_buffer(model, objFile->num_tris) != 0)
    {
        return -1;
    }
    
    /*for each triangle...*/
    for (i = 0; i < objFile->num_tris;i++)
    {
        /*for each triangle vertex...*/
        for (j = 0;j < 3;j++)
        {
            /*get a pointer to the vertex data*/
            vtarget = &objFile->vertex_array[objFile->triangle_array[i].p[j].v];

            /*copy the normal data*/
            memcpy(atarget,&objFile->normal_array[objFile->triangle_array[i].p[j].n],sizeof(float)*3);

            /*copy the texture data*/
            memcpy(&atarget[3],&objFile->texel_array[objFile->triangle_array[i].p[j].t],sizeof(float)*2);
            
            /*check if we already had this vertex/normal/uv set before*/
            search = obj_va_search(vertices,attributes,count,vtarget,atarget);
            if (search == -1)
            {
                /*new index*/
                memcpy(&vertices[count*3],vtarget,sizeof(float)*3);
                memcpy(&attributes[count*6],atarget,sizeof(float)*6);
                model->triangle_array[(i * 3)+j] = (GLuint)count;
                count++;
            }
            else
            {
                model->triangle_array[(i * 3)+j] = (GLuint)search;
            }
        }   
    }
    return 0;
}

Obj *obj_load(char *filename)
{
    FILE *file;
    Obj *objFile;
    
    objFile = obj_get_by_filename(filename);
    if (objFile)
    {
		objFile->inuse++;
        return objFile;
    }
    
	objFile = obj_init();
    if (!objFile)
    {
        return NULL;
    }
    
    file = fopen(filename,"r");
    if (file == NULL)
    {
        slog("failed to open file %s",filename);
        return NULL;
    }
    
    
    obj_file_get_counts(objFile,file);
    
    slog("vertices: %i",objFile->num_vertices);
    slog("normals: %i",objFile->num_normals);
    slog("texels: %i",objFile->num_texels);
    slog("faces: %i",objFile->num_tris);
    
    obj_allocate(objFile);
    obj_file_parse(objFile, file);
    
    fclose(file);
    
    return objFile;
}

void obj_draw(
    Obj *obj,
    Vec3D position,
    Vec3D rotation,
    Vec3D scale,
    Vec4D color,
    Sprite *texture
             )
{
    int i;
    ObjTriangle* triangle;
    float trans[4];
    
    if (obj == NULL)
    {
        slog("cannot draw obj, none specified!");
        return;
    }
    glPushMatrix();

    glEnable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glEnable(GL_NORMALIZE);
    glColorMaterial(GL_FRONT,GL_DIFFUSE);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    if(texture != NULL)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,texture->texture);
    }
    
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,trans);
    glDepthFunc(GL_LEQUAL);
    
    glTranslatef(position.x,position.y,position.z);
    glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
    glScalef(scale.x,scale.y,scale.z);
    
    glColor3f(color.x,color.y,color.z);
    glBegin(GL_TRIANGLES);
    for (i = 0; i < obj->num_tris; i++)
    {
        triangle = &obj->triangle_array[i];
        if (obj->normal_array)
        {
            glNormal3f(
                obj->normal_array[triangle->p[0].n * 3],
                obj->normal_array[triangle->p[0].n * 3 + 1],
                obj->normal_array[triangle->p[0].n * 3 + 2]
            );
        }
        if (obj->texel_array)
        {
            glTexCoord2f(
                obj->texel_array[triangle->p[0].t * 2],
                obj->texel_array[triangle->p[0].t * 2 + 1]
            );
        }
        glVertex3f(
            obj->vertex_array[triangle->p[0].v * 3],
            obj->vertex_array[triangle->p[0].v * 3 + 1],
            obj->vertex_array[triangle->p[0].v * 3 + 2]
        );
        
        if (obj->normal_array)
        {
            glNormal3f(
                obj->normal_array[triangle->p[1].n * 3],
                obj->normal_array[triangle->p[1].n * 3 + 1],
                obj->normal_array[triangle->p[1].n * 3 + 2]
            );
        }
        if (obj->texel_array)
        {
            glTexCoord2f(
                obj->texel_array[triangle->p[1].t * 2],
                obj->texel_array[triangle->p[1].t * 2 + 1]
            );
        }
        glVertex3f(
            obj->vertex_array[triangle->p[1].v * 3],
            obj->vertex_array[triangle->p[1].v * 3 + 1],
            obj->vertex_array[triangle->p[1].v * 3 + 2]);
        
        if (obj->normal_array)
        {
            glNormal3f(
                obj->normal_array[triangle->p[2].n * 3],
                obj->normal_array[triangle->p[2].n * 3 + 1],
                obj->normal_array[triangle->p[2].n * 3 + 2]
            );
        }
        if (obj->texel_array)
        {
            glTexCoord2f(
                obj->texel_array[triangle->p[2].t * 2],
                obj->texel_array[triangle->p[2].t * 2 + 1]
            );
        }
        glVertex3f(
            obj->vertex_array[triangle->p[2].v * 3],
            obj->vertex_array[triangle->p[2].v * 3 + 1],
            obj->vertex_array[triangle->p[2].v * 3 + 2]);
        
    }
    glEnd();
    
    glColor4f(1,1,1,1);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glDisable(GL_COLOR_MATERIAL);
    if(texture != NULL)
    {
        glDisable(GL_TEXTURE_2D);
    }    
    glPopMatrix();
}

Model *obj_load_model(char *filename)
{
    Model *model;
    Obj *objFile;
    model = model_get_by_filename(filename);
    if (model)
    {
        return model;
    }    
    objFile = obj_load(filename);
    if (!objFile)
    {
        return NULL;
    }
    
	model = model_init();
    if (!model)
    {
        return NULL;
    }
        
    if (obj_file_convert_to_model(objFile,model) != 0)
    {
        model_free(model);        
        return NULL;
    }
    
    return model;
}

////////////////////////////////////////////////////////////

void model_init_all()
{
    memset(ModelList,0,sizeof(Model)*__model_max);
    atexit(model_close);
}

static void model_delete(Model *model)
{
    if (!model)return;
    
	sprite_free(model->texture);
    if (model->vertex_array)
    {
        free(model->vertex_array);
    }
    if (model->attribute_array)
    {
        free(model->attribute_array);
    }
    if (model->triangle_array)
    {
        free(model->triangle_array);
    }
    memset(model,0,sizeof(Model));
}

void model_free(Model *model)
{
    if (!model)return;
    model->used--;
    if (model->used > 0)return;
    model_delete(model);
}

static void model_close()
{
    int i;
    for (i = 0; i < __model_max; i++)
    {
        if (ModelList[i].used)
        {
            model_delete(&ModelList[i]);
        }
    }
}

Model *model_init()
{
    int i;
    for (i = 0; i < __model_max; i++)
    {
        if (ModelList[i].used == 0)
        {
            memset(&ModelList[i],0,sizeof(Model));
            ModelList[i].used = 1;
            return &ModelList[i];
        }
    }
    return NULL;
}

Model *model_get_by_filename(char *filename)
{
    int i;
    for (i = 0; i < __model_max; i++)
    {
        if ((ModelList[i].used != 0) &&
            (strcmp(ModelList[i].filename,filename) == 0))
        {
            return &ModelList[i];
        }
    }
    return NULL;
}

void model_assign_texture(Model *model,char *texture)
{
    Sprite *sprite;
    if (!model)return;
	sprite = sprite_load(texture,-1,-1);
    if (!sprite)return;
    model->texture = sprite;
}

int model_allocate_triangle_buffer(Model *model, GLuint triangles)
{
    if (!model)
    {
        slog("no model specified");
        return -1;
    }
    if (model->triangle_array != NULL)
    {
        slog("model %s already has a triangle buffer");
        return -1;
    }
    if (!triangles)
    {
        slog("cannot allocate 0 triangles!");
        return -1;
    }
    model->triangle_array = (GLuint *)malloc(sizeof(GLuint)*3*triangles);
    if (!model->triangle_array)
    {
        slog("failed to allocate triangle buffer");
        return -1;
    }
    memset(model->triangle_array,0,sizeof(GLuint)*3*triangles);
    model->num_tris = triangles;
    return 0;
}

int model_set_vertex_buffer(Model *model, float *vertex_buffer, GLuint count)
{
    if (model_allocate_vertex_buffer(model, count) != 0)
    {
        return -1;
    }
    memcpy(model->vertex_array,vertex_buffer,sizeof(float)*count *3);
    return 0;
}

int model_set_attribute_buffer(Model *model, float *attribute_buffer, GLuint count)
{
    if (model_allocate_attribute_buffer(model, count) != 0)
    {
        return -1;
    }
    memcpy(model->attribute_array,attribute_buffer,sizeof(float)*count *6);
    return 0;
}


int model_allocate_vertex_buffer(Model *model, GLuint vertices)
{
    if (!model)
    {
        slog("no model specified");
        return -1;
    }
    if (model->vertex_array != NULL)
    {
        slog("model %s already has a vertex buffer");
        return -1;
    }
    if (!vertices)
    {
        slog("cannot allocate 0 vertices!");
        return -1;
    }
    model->vertex_array = (float *)malloc(sizeof(float)*3*vertices);
    if (!model->vertex_array)
    {
        slog("failed to allocate vertex buffer");
        return -1;
    }
    memset(model->vertex_array,0,sizeof(float)*3*vertices);
    model->num_vertices = vertices;
    return 0;
}

int model_allocate_attribute_buffer(Model *model, GLuint attributes)
{
    if (!model)
    {
        slog("no model specified");
        return -1;
    }
    if (model->attribute_array != NULL)
    {
        slog("model %s already has a vertex buffer");
        return -1;
    }
    if (!attributes)
    {
        slog("cannot allocate 0 attributes!");
        return -1;
    }
    model->attribute_array = (float *)malloc(sizeof(float)*6*attributes);
    if (!model->attribute_array)
    {
        slog("failed to allocate vertex buffer");
        return -1;
    }
    memset(model->attribute_array,0,sizeof(float)*6*attributes);
    return 0;
}



size_t model_get_triangle_buffer_size(Model *model)
{
    if (!model)return 0;
    return (sizeof(GLshort)*model->num_tris*3);
}

size_t model_get_vertex_buffer_size(Model *model)
{
    if (!model)return 0;
    return (sizeof(float)*3*model->num_vertices);
}