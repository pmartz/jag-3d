#include <stdio.h>
#include <assert.h>
#include "ImageLoader_jpg.h"

#include <string.h>

#include <setjmp.h>
#ifdef __cplusplus
extern "C" {
#endif
#include <jpeglib.h>
#include <jerror.h>
#ifdef __cplusplus
}
#endif


namespace iiDB {

class ImageLoaderJpgInitiator 
{ 
    public: 
        ImageLoaderJpgInitiator() 
        { 
            // List all file extensions you want this image loader to 
            // support.
            const char *extensions[] = {
                "jpg",      // e.g. rgb
                0L
            };

            ImageLoader_ptr il( new ImageLoaderJpg );

            const char **ptr = extensions;

            while( *ptr )
            {
                ImageLoaderRegistry::instance()->registerImageLoader( *ptr, il );
                ptr++;
            }
        }
} __imageLoaderJpgInitiator; // This is an instance that will call the constructor when
                               // the shared object is loaded.

ImageLoaderJpg::ImageLoaderJpg() 
{
}

ImageLoaderJpg::~ImageLoaderJpg() 
{
}

#define ERR_NO_ERROR 0
#define ERR_OPEN     1
#define ERR_MEM      2
#define ERR_JPEGLIB  3

static int jpegerror = ERR_NO_ERROR;

/* CODE FOR READING/WRITING JPEG FROM STREAMS
 *  This code was taken directly from jdatasrc.c and jdatadst.c (libjpeg source)
 *  and modified to use a std::istream/ostream* instead of a FILE*
 */

/* Expanded data source object for stdio input */

typedef struct {
    struct jpeg_source_mgr pub;    /* public fields */
    std::istream * infile;        /* source stream */
    JOCTET * buffer;              /* start of buffer */
    boolean start_of_file;        /* have we gotten any data yet? */
} stream_source_mgr;

typedef struct
{
	struct jpeg_source_mgr pub;
	unsigned char *inbuf;
	unsigned long offset;
	int size;
	boolean start_of_file;
} buffer_SourceMgr;


typedef stream_source_mgr * stream_src_ptr;

#define INPUT_BUF_SIZE  4096    /* choose an efficiently fread'able size */
#define READ_CHUNK_SIZE 64*1024 /* choose an efficiently fread'able size */

void init_source (j_decompress_ptr cinfo)
{
  stream_src_ptr src = (stream_src_ptr) cinfo->src;
  src->start_of_file = TRUE;
}

boolean fill_input_buffer (j_decompress_ptr cinfo)
{
  stream_src_ptr src = (stream_src_ptr) cinfo->src;
  size_t nbytes;

  src->infile->read((char*)src->buffer,INPUT_BUF_SIZE);
  nbytes = src->infile->gcount();

  if (nbytes <= 0) {
    if (src->start_of_file)    /* Treat empty input file as fatal error */
      ERREXIT(cinfo, JERR_INPUT_EMPTY);
    WARNMS(cinfo, JWRN_JPEG_EOF);
    /* Insert a fake EOI marker */
    src->buffer[0] = (JOCTET) 0xFF;
    src->buffer[1] = (JOCTET) JPEG_EOI;
    nbytes = 2;
  }

  src->pub.next_input_byte = src->buffer;
  src->pub.bytes_in_buffer = nbytes;
  src->start_of_file = FALSE;

  return TRUE;
}

void buffer_InitSource (j_decompress_ptr cinfo)
{
	buffer_SourceMgr * src = (buffer_SourceMgr *)cinfo->src;

	src->pub.next_input_byte = NULL;
	src->pub.bytes_in_buffer = 0;

    src->offset = 0;
    src->start_of_file = TRUE;
}

boolean buffer_FillInputBuffer (j_decompress_ptr cinfo)
{
	buffer_SourceMgr * src = (buffer_SourceMgr *)cinfo->src;

    size_t chunk_size = READ_CHUNK_SIZE;
    if(src->size - src->offset > chunk_size)
    {
        chunk_size = src->size - src->offset;
    }

    if(chunk_size <= 0)
    {
        if (src->start_of_file)/* Treat empty input file as fatal error */
        {
          ERREXIT(cinfo, JERR_INPUT_EMPTY);
        }
        WARNMS(cinfo, JWRN_JPEG_EOF);
    }

	src->pub.next_input_byte = (JOCTET *)src->inbuf+src->offset;
	src->pub.bytes_in_buffer = (unsigned int)chunk_size;
    src->offset += chunk_size;
	src->start_of_file = FALSE;

	return TRUE;
}

void skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
  stream_src_ptr src = (stream_src_ptr) cinfo->src;

  if (num_bytes > 0) {
    while (num_bytes > (long) src->pub.bytes_in_buffer) {
      num_bytes -= (long) src->pub.bytes_in_buffer;
      (void) fill_input_buffer(cinfo);
    }
    src->pub.next_input_byte += (size_t) num_bytes;
    src->pub.bytes_in_buffer -= (size_t) num_bytes;
  }
}

void buffer_SkipInputData (j_decompress_ptr cinfo, long num_bytes)
{
	buffer_SourceMgr * src = (buffer_SourceMgr *)cinfo->src;

	if ((unsigned)num_bytes >= src->pub.bytes_in_buffer)
	{
		num_bytes -= (long)src->pub.bytes_in_buffer;
        size_t chunk_size = READ_CHUNK_SIZE;
        if(src->size - src->offset > chunk_size)
        {
            chunk_size = src->size - src->offset;
        }
		int chunk = num_bytes & ~(chunk_size - 1);

        src->offset += chunk;
		src->pub.bytes_in_buffer = (size_t)chunk_size;
		src->pub.next_input_byte = (JOCTET *)(src->inbuf + num_bytes - chunk);
	}
	else
	{
		assert((src->pub.next_input_byte != NULL) && (src->pub.bytes_in_buffer != 0));
		src->pub.next_input_byte += num_bytes;
		src->pub.bytes_in_buffer -= num_bytes;
	}
}

boolean buffer_ResyncToRestart (j_decompress_ptr cinfo, int desired)
{
	return jpeg_resync_to_restart(cinfo, desired);
}

void term_source (j_decompress_ptr /*cinfo*/)
{
  /* no work necessary here */
}

void buffer_TermSource (j_decompress_ptr cinfo)
{
	buffer_SourceMgr * src = (buffer_SourceMgr *)cinfo->src;

	src->pub.next_input_byte = NULL;
	src->pub.bytes_in_buffer = 0;
}

void jpeg_istream_src(j_decompress_ptr cinfo, std::istream *infile)
{
    stream_src_ptr src;
    if (cinfo->src == NULL) {    /* first time for this JPEG object? */
        cinfo->src = (struct jpeg_source_mgr *)
            (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,sizeof(stream_source_mgr));
        src = (stream_src_ptr) cinfo->src;
        src->buffer = (JOCTET *)
            (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,INPUT_BUF_SIZE * sizeof(JOCTET));
    }

    src = (stream_src_ptr) cinfo->src;
    src->pub.init_source = init_source;
    src->pub.fill_input_buffer = fill_input_buffer;
    src->pub.skip_input_data = skip_input_data;
    src->pub.resync_to_restart = jpeg_resync_to_restart; /* use default method */
    src->pub.term_source = term_source;
    src->infile = infile;
    src->pub.bytes_in_buffer = 0; /* forces fill_input_buffer on first read */
    src->pub.next_input_byte = NULL; /* until buffer loaded */
}

int simage_jpeg_error(char * buffer, int buflen)
{
    switch (jpegerror)
    {
        case ERR_OPEN:
            strncpy(buffer, "JPEG loader: Error opening file", buflen);
            break;
        case ERR_MEM:
            strncpy(buffer, "JPEG loader: Out of memory error", buflen);
            break;
        case ERR_JPEGLIB:
            strncpy(buffer, "JPEG loader: Illegal jpeg file", buflen);
            break;
    }
    return jpegerror;
}


struct my_error_mgr
{
    struct jpeg_error_mgr pub;   /* "public" fields */

    jmp_buf setjmp_buffer;       /* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

int simage_jpeg_identify(const char *, const unsigned char *header, int headerlen)
{
    static unsigned char jpgcmp[] = {'J', 'F', 'I', 'F' };
    if (headerlen < 4) return 0;
    if (memcmp((const void*)&header[6],
        (const void*)jpgcmp, 4) == 0) return 1;
    return 0;
}

typedef struct my_error_mgr * my_error_ptr;

static void my_error_exit (j_common_ptr cinfo)
{
    /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
    my_error_ptr myerr = (my_error_ptr) cinfo->err;

    /* Always display the message. */
    /* We could postpone this until after returning, if we chose. */
    /*(*cinfo->err->output_message) (cinfo);*/

    /* Return control to the setjmp point */
    longjmp(myerr->setjmp_buffer, 1);
}


// Load from file
Image_ptr ImageLoaderJpg::load( const std::string &filename )
{
    Image_ptr image( new Image );

    // Fill it in here

    int width;
    unsigned int height;
    int ncomp;
    unsigned char *currPtr;

    struct jpeg_decompress_struct cinfo;

    JSAMPARRAY rowbuffer;
    int row_stride;

    struct my_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;

    if (setjmp(jerr.setjmp_buffer))
    {
        /* If we get here, the JPEG code has signaled an error.
         * We need to clean up the JPEG object, close the input file, and return.
         */
        //jpegerror = ERR_JPEGLIB;
        jpeg_destroy_decompress(&cinfo);
        //fclose(infile);
        //if (buffer) delete [] buffer;
        return Image_ptr();
    }


    jpeg_create_decompress(&cinfo);

    FILE *fp = fopen(filename.c_str(), "rb");
    if(fp == NULL)
    {
        return Image_ptr();
    }

    jpeg_stdio_src(&cinfo, fp);
        

    (void) jpeg_read_header(&cinfo, TRUE);

    if (cinfo.jpeg_color_space == JCS_GRAYSCALE)
    {
        ncomp = 1;
        cinfo.out_color_space = JCS_GRAYSCALE;
        image->setOpenGLFormat(GL_LUMINANCE);
    }
    else
    {
        ncomp = 3;
        cinfo.out_color_space = JCS_RGB;
        image->setOpenGLFormat(GL_RGB);
    }

    (void) jpeg_start_decompress(&cinfo);
    row_stride = cinfo.output_width * cinfo.output_components;
    rowbuffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
    width = cinfo.output_width;
    height = cinfo.output_height;

    size_t size = row_stride * height;

    image->data = new GLbyte[size];
    currPtr = (unsigned char *)image->data.get();
    currPtr += row_stride * (height-1);

    while (cinfo.output_scanline < height)
    {
        (void) jpeg_read_scanlines(&cinfo, rowbuffer, 1);
        memcpy(currPtr, rowbuffer[0], row_stride);
        //currPtr += row_stride;
        currPtr -= row_stride;
    }

    (void) jpeg_finish_decompress(&cinfo);

    jpeg_destroy_decompress(&cinfo);

    image->setWidth(width);
    image->setHeight(height);
    image->setDepth(1);
    image->setNumComponents(ncomp);
    image->setOpenGLType(GL_UNSIGNED_BYTE);
    image->compressed = false;

    return image;
}

// Load from istream
Image_ptr ImageLoaderJpg::load( const std::istream &fin )
{
    Image_ptr image( new Image );

    // Fill it in here

    int width;
    unsigned int height;
    int ncomp;
    unsigned char *currPtr;

    struct jpeg_decompress_struct cinfo;

    JSAMPARRAY rowbuffer;
    int row_stride;

    struct my_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;

    if (setjmp(jerr.setjmp_buffer))
    {
        /* If we get here, the JPEG code has signaled an error.
         * We need to clean up the JPEG object, close the input file, and return.
         */
        //jpegerror = ERR_JPEGLIB;
        jpeg_destroy_decompress(&cinfo);
        //fclose(infile);
        //if (buffer) delete [] buffer;
        return Image_ptr();
    }


    jpeg_create_decompress(&cinfo);

    jpeg_istream_src(&cinfo,(std::istream *)&fin);

    (void) jpeg_read_header(&cinfo, TRUE);

    if (cinfo.jpeg_color_space == JCS_GRAYSCALE)
    {
        ncomp = 1;
        cinfo.out_color_space = JCS_GRAYSCALE;
        image->setOpenGLFormat(GL_LUMINANCE);
    }
    else
    {
        ncomp = 3;
        cinfo.out_color_space = JCS_RGB;
        image->setOpenGLFormat(GL_RGB);
    }

    (void) jpeg_start_decompress(&cinfo);
    row_stride = cinfo.output_width * cinfo.output_components;
    rowbuffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
    width = cinfo.output_width;
    height = cinfo.output_height;

    size_t size = row_stride * height;

    image->data = new GLbyte[size];
    currPtr = (unsigned char *)image->data.get();
    currPtr += row_stride * (height-1);

    while (cinfo.output_scanline < height)
    {
        (void) jpeg_read_scanlines(&cinfo, rowbuffer, 1);
        memcpy(currPtr, rowbuffer[0], row_stride);
        //currPtr += row_stride;
        currPtr -= row_stride;
    }

    (void) jpeg_finish_decompress(&cinfo);

    jpeg_destroy_decompress(&cinfo);

    image->setWidth(width);
    image->setHeight(height);
    image->setDepth(1);
    image->setNumComponents(ncomp);
    image->setOpenGLType(GL_UNSIGNED_BYTE);
    image->compressed = false;

    return image;
}

Image_ptr ImageLoaderJpg::load(const char *inbuffer, size_t insize)
{
    Image_ptr image( new Image );

    // Fill it in here

    int width;
    unsigned int height;
    int ncomp;
    unsigned char *currPtr;

    struct jpeg_decompress_struct cinfo;

    JSAMPARRAY rowbuffer;
    int row_stride;

    struct my_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;

    if (setjmp(jerr.setjmp_buffer))
    {
        /* If we get here, the JPEG code has signaled an error.
         * We need to clean up the JPEG object, close the input file, and return.
         */
        //jpegerror = ERR_JPEGLIB;
        jpeg_destroy_decompress(&cinfo);
        //fclose(infile);
        //if (buffer) delete [] buffer;
        return Image_ptr();
    }


    jpeg_create_decompress(&cinfo);

    buffer_SourceMgr jsrc;

    // setup the input buffer and input buffer size
    jsrc.inbuf = (unsigned char *)inbuffer;
    jsrc.size = insize;

    // setup function pointers for reading from a buffer
    jsrc.pub.init_source = buffer_InitSource;
    jsrc.pub.fill_input_buffer = buffer_FillInputBuffer;
    jsrc.pub.skip_input_data = buffer_SkipInputData;
    jsrc.pub.resync_to_restart = buffer_ResyncToRestart;
    jsrc.pub.term_source = buffer_TermSource;

    cinfo.src = &jsrc.pub;

    (void) jpeg_read_header(&cinfo, TRUE);

    if (cinfo.jpeg_color_space == JCS_GRAYSCALE)
    {
        ncomp = 1;
        cinfo.out_color_space = JCS_GRAYSCALE;
        image->setOpenGLFormat(GL_LUMINANCE);
    }
    else
    {
        ncomp = 3;
        cinfo.out_color_space = JCS_RGB;
        image->setOpenGLFormat(GL_RGB);
    }

    (void) jpeg_start_decompress(&cinfo);
    row_stride = cinfo.output_width * cinfo.output_components;
    rowbuffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
    width = cinfo.output_width;
    height = cinfo.output_height;

    size_t size = row_stride * height;

    image->data = new GLbyte[size];
    currPtr = (unsigned char *)image->data.get();
    currPtr += row_stride * (height-1);

    while (cinfo.output_scanline < height)
    {
        (void) jpeg_read_scanlines(&cinfo, rowbuffer, 1);
        memcpy(currPtr, rowbuffer[0], row_stride);
        //currPtr += row_stride;
        currPtr -= row_stride;
    }

    (void) jpeg_finish_decompress(&cinfo);

    jpeg_destroy_decompress(&cinfo);

    image->setWidth(width);
    image->setHeight(height);
    image->setDepth(1);
    image->setNumComponents(ncomp);
    image->setOpenGLType(GL_UNSIGNED_BYTE);
    image->compressed = false;

    return image;
}


}
