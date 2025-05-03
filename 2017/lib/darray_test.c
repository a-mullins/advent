#include <stdlib.h>
#include "darray.h"


int
main(void)
{
    struct darray *d = malloc(sizeof (struct darray));
    darray_init(d, sizeof (int));

    for(int i = 0; i < 5; i++)
        darray_push(d, &i);

    return 0;
}
