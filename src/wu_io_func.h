//wu_io_func.h

typedef struct {
	float num;
// adnan - need to redefine this as a wu type to avoid conflicts with math.h
	//} float_t;
} wu_float_t;

typedef struct {
	short num;
} short_t;

float getf( FILE *file );
int gets( FILE *file );
void putf( float number, FILE *file );
void puts( int number, FILE *file );
void StringSet( char *in, const char *msg, ... );
