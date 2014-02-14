#include "lecteur.h"

// Load an image from .BMP file
int ImageLoad(char *filename, Image *image) {
  FILE *file;
  unsigned long size;                 // size of the image in bytes.
  unsigned long i;                    // standard counter.
  unsigned short int planes;          // number of planes in image (must be 1) 
  unsigned short int bpp;             // number of bits per pixel (must be 24)
  char temp;                          // used to convert bgr to rgb color.
  
  // Make sure the file exists
  if ((file = fopen(filename, "rb"))==NULL)
    {
      printf("File Not Found : %s\n",filename);
      return 0;
    }
  
  // Skip to bmp header
  fseek(file,18, SEEK_CUR);
  
  // read width
  if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
    printf("Error reading width from %s.\n", filename);
    return 0;
  }
  printf("Width of %s: %lu\n",filename, image->sizeX);
  
  //read the height
  if ((i = fread(&image->sizeY,4,1,file)) != 1) {
    printf("Error reading height from %s.\n", filename);
    return 0;
  }
  printf("Height of %s: %lu\n", filename, image->sizeY);
  
  // calculate the size (assuming 24 bpp)
  size = image->sizeX * image->sizeY * 3;
  
  // read the planes
  if ((fread(&planes, 2, 1, file)) != 1) {
    printf("Error reading planes from %s. \n", filename);
    return 0;
  }
  
  if (planes != 1) {
    printf("Planes from %s is not 1: %u\n", filename, planes);
    return 0;
  }
  
  // read the bpp
  if ((i = fread(&bpp, 2, 1, file)) != 1) {
    printf("Error reading bpp from %s. \n", filename);
    return 0;
  }
  
  if (bpp != 24) {
    printf("Bpp from %s is not 24: %u\n", filename, bpp);
    return 0;
  }
  
  // seek past the rest of the bitmap header
  fseek(file, 24, SEEK_CUR);
  
  // Read the data
  image->data = (char *) malloc(size);
  if (image->data == NULL) {
    printf("Error allocating memory for colour-corrected image data");
    return 0;
  }
  
  if ((i = fread(image->data,size,1,file)) != 1) {
    printf("Error reading image data from %s.\n", filename);
    return 0;
  }
  
  // reverse all of the colours bgr => rgb)
  for (i=0;i<size;i+=3) {
    temp = image->data[i];
    image->data[i] = image->data[i+2];
    image->data[i+2] = temp;
  }
  

  for (i=0; i<12; i++)
    printf("%x ",image->data[i]);
  cout << endl;

  // Thats all folks
  return 1;
  
}


struct gl_texture_t *ReadPNGFromFile (const char *filename) 
{
  
  FILE *fp;
  png_byte magic[8];
  
  /* Open image file */
  fp = fopen (filename, "rb");
  if (!fp) {
    fprintf (stderr, "error: couldn't open \"%s\"!\n", filename);
    return NULL;
  }
  
  /* Read magic number */
  fread (magic, 1, sizeof (magic), fp);
  
  /* Check for valid magic number */
  if (!png_check_sig (magic, sizeof (magic))) {
    fprintf (stderr, "error: \"%s\" is not a valid PNG image!\n",filename);
    fclose (fp);
    return NULL;
  }
  
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr) {
    fclose (fp);
    return NULL;
  }
  
  /* Create a png info struct */
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    fclose (fp);
    png_destroy_read_struct (&png_ptr, NULL, NULL);
    return NULL;
  }

  /* Create our OpenGL texture object */
  struct gl_texture_t *texinfo = (struct gl_texture_t *) malloc (sizeof (struct gl_texture_t));

  /* Initialize the setjmp for returning properly after a libpng
     error occured */
  if (setjmp (png_jmpbuf(png_ptr))) {
    fclose (fp);
    png_destroy_read_struct (&png_ptr, &info_ptr, NULL);
    
    if (texinfo) {
      if (texinfo->texels)
	free (texinfo->texels);      
      free (texinfo);
    }    
    return NULL;
  }

  /* Setup libpng for using standard C fread() function
     with our FILE pointer */
  png_init_io (png_ptr, fp);
  
  /* Tell libpng that we have already read the magic number */
  png_set_sig_bytes (png_ptr, sizeof (magic));
  
  /* Read png info */
  png_read_info (png_ptr, info_ptr);

  int bit_depth, color_type;
  
  /* Get some usefull information from header */
  bit_depth = png_get_bit_depth (png_ptr, info_ptr);
  color_type = png_get_color_type (png_ptr, info_ptr);
  
  /* Convert index color images to RGB images */
  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb (png_ptr);

  /* Convert 1-2-4 bits grayscale images to 8 bits
     grayscale. */
  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_gray_1_2_4_to_8 (png_ptr);
  
  if (png_get_valid (png_ptr, info_ptr, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha (png_ptr);
  if (bit_depth == 16)
    png_set_strip_16 (png_ptr);
  else if (bit_depth < 8)
    png_set_packing (png_ptr);

  /* Update info structure to apply transformations */
  png_read_update_info (png_ptr, info_ptr);

  /* Retrieve updated information */
  png_uint_32 w, h;
  
  png_get_IHDR (png_ptr, info_ptr, &w, &h, &bit_depth,&color_type, NULL, NULL, NULL);
  texinfo->width = w;
  texinfo->height = h;
  switch (color_type) {
  case PNG_COLOR_TYPE_GRAY:
    texinfo->format = GL_LUMINANCE;
    texinfo->internalFormat = 1;
    break;
    
  case PNG_COLOR_TYPE_GRAY_ALPHA:
    texinfo->format = GL_LUMINANCE_ALPHA;
    texinfo->internalFormat = 2;
    break;
    
  case PNG_COLOR_TYPE_RGB:
    texinfo->format = GL_RGB;
    texinfo->internalFormat = 3;
    break;

  case PNG_COLOR_TYPE_RGB_ALPHA:
    texinfo->format = GL_RGBA;
    texinfo->internalFormat = 4;
    break;

  default:
    /* Badness */
    break;
  }
  /* We can now allocate memory for storing pixel data */
  texinfo->texels = (GLubyte *)malloc (sizeof (GLubyte)*texinfo->width*texinfo->height*texinfo->internalFormat);
  
  png_bytep *row_pointers;
  
  /* Setup a pointer array.  Each one points at the begening of a row. */
  row_pointers = (png_bytep *)malloc (sizeof (png_bytep) * texinfo->height);
  
  for (int i = 0; i < texinfo->height; ++i) 
    row_pointers[i] = (png_bytep)(texinfo->texels + ((texinfo->height - (i + 1)) * texinfo->width * texinfo->internalFormat));
  
  /* Read pixel data using row pointers */
  png_read_image(png_ptr, row_pointers);
  
  /* We don't need row pointers anymore */
  free (row_pointers);
  
  return(texinfo);
  
}

GLubyte* glmReadPPM(char* filename, int* width, int* height)
{
    FILE* fp;
    int i, w, h, d;
    unsigned char* image;
    char head[70];          /* max line <= 70 in PPM (per spec). */
    
    fp = fopen(filename, "rb");
    if (!fp) {
        perror(filename);
        return NULL;
    }
    
    /* grab first two chars of the file and make sure that it has the
       correct magic cookie for a raw PPM file. */
    fgets(head, 70, fp);
    if (strncmp(head, "P6", 2)) {
        fprintf(stderr, "%s: Not a raw PPM file\n", filename);
        return NULL;
    }
    
    /* grab the three elements in the header (width, height, maxval). */
    i = 0;
    while(i < 3) {
        fgets(head, 70, fp);
        if (head[0] == '#')     /* skip comments. */
            continue;
        if (i == 0)
            i += sscanf(head, "%d %d %d", &w, &h, &d);
        else if (i == 1)
            i += sscanf(head, "%d %d", &h, &d);
        else if (i == 2)
            i += sscanf(head, "%d", &d);
    }
    
    /* grab all the image data in one fell swoop. */
    image = (unsigned char*)malloc(sizeof(unsigned char)*w*h*3);
    fread(image, sizeof(unsigned char), w*h*3, fp);
    fclose(fp);
    
    *width = w;
    *height = h;
    return image;
}