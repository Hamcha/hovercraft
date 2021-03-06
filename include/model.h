/*! \file model.h
 *  \brief Model creation and handling
 */

#ifndef _MODEL_H
#define _MODEL_H

#include <gctypes.h>
#include <gccore.h>

typedef struct {
	unsigned int vcount;  /*< Vertex count        */
	unsigned int ncount;  /*< Normal count        */
	unsigned int vtcount; /*< UV Coordinate count */
	unsigned int fcount;  /*< Face/Index count    */
} binheader_t;

typedef struct {
	u16 vertex;
	u16 uv;
	u16 normal;
} index_t;

typedef struct {
	GXTexObj* textureObject; /*< Texture Object	               */
	void*     modelList;     /*< Storage for the display lists */
	u32       modelListSize; /*< Real display list sizes       */

	u32  modelFaceCount; /*< Amount of triangles */
	f32* modelPositions;
	f32* modelNormals;
	f32* modelTexcoords;
	index_t* modelIndices;
} model_t;

/*! \brief Create a new model from mesh data
 *	\param model_bmb Mesh data generated by obj2bin
 *	\return Pointer to model struct
 */
model_t* MODEL_setup(const u8* model_bmb);

/*! \brief Destroy a model and free his allocated memory
 *	\param model Model to destroy
 */
void MODEL_destroy(model_t* model);

/*! \brief Render a model on screen (calls his Draw list)
 *	\param model Model to render
 */
void MODEL_render(model_t* model);

/*! \brief Set model's texture (1 texture per model currently supported)
 *  \param model Model to assign the texture to
 *  \param textureObject Texture object to assign
 */
void MODEL_setTexture(model_t* model, GXTexObj* textureObject);

#endif
