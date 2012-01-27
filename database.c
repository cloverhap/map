#include "database.h"
#include "controller.h"
#include <stdio.h>

/***
 * Reads the character database
 * @pre The format for the database is:
 *      first non-comment, non-empty line is the heading
 *      ignore comments (//) and empty lines
 ****/
static int read_chars() {
    // read db/chars/chars.txt and store data in chars[]
    FILE *fp;
    GLbyte l[MAX_DB_LINE_LENGTH];
    GLint field_count = 5;  // how many fields there are in a row, TODO: use for flexible db reading
    GLint counter = 0;

    fp = fopen("db\\chars\\chars.txt", "r");
    if (fp == 0) {
        fputs("Does character database exist?\n",stderr);
        glutDestroyWindow(glutGetWindow());
        return 1;
    }

    // read chars db into chars array
    while (fgets(l, MAX_DB_LINE_LENGTH, fp) != 0) {
#ifdef DEBUG_VALUE
        //printf("%s", l);
#endif
        // Comments and whitespaces
        if (l[0] == '\0' || l[0] == '\n' || l[0] == ' ' ||
            (l[0] == '/' && l[1] == '/') ||
            (l[0] == 'i' && l[1] == 'd'))
        {
            continue;
        }
        player *p = (player*)malloc(sizeof(player));
        if (p == 0) fputs("Memory Allocation error. read_chars()\n", stderr);
        const char *lp = l;

        // Reading attributes
        sscanf(l, "%d %s %s %lf,%lf,%lf %d",
               &(p->_id),
               p->name,
               p->tag,
               &(p->position[0]),&(p->position[1]),&(p->position[2]),
               &(p->map_id));
        chars[counter++] = p;
#ifdef DEBUG_VALUE
        printf("\nCharacter %d ID: %d Name: %s Tag: %s Position: %lf,%lf,%lf Map ID: %d\n",counter,p->_id,p->name,p->tag,p->position[0],p->position[1],p->position[2],p->map_id);
#endif
    }
#ifdef DEBUG_VALUE
    printf("Character Database parsed.\n");
#endif

    if (fclose(fp) == 0) {
        return 0;
    } else return 1;
}

static int read_maps() {
    // read db/maps/maps.txt and store data in chars[]
    FILE *fp;
    GLbyte l[MAX_DB_LINE_LENGTH];
    GLint field_count = 4;  // how many fields there are in a row
    GLint counter = 0;

    fp = fopen("db\\maps\\maps.txt", "r");
    if (fp == 0) {
        fputs("Does map database exist?\n",stderr);
        glutDestroyWindow(glutGetWindow());
        return 1;
    }

    // read chars db into chars array
    while (fgets(l, MAX_DB_LINE_LENGTH, fp) != 0) {
#ifdef DEBUG_VALUE
        //printf("%s", l);
#endif
        // Comments and whitespaces
        if (l[0] == '\0' || l[0] == '\n' || l[0] == ' ' ||
            (l[0] == '/' && l[1] == '/') ||
            (l[0] == 'i' && l[1] == 'd'))
        {
            continue;
        }

        area *a = (area*)malloc(sizeof(area));
        if (a == 0) fputs("Memory Allocation error. read_maps()\n", stderr);

        // Reading attributes
        sscanf(l, "%d %s %lf,%lf,%lf %d",
               &(a->_id),
               a->name,
               &(a->dimension[0]),&(a->dimension[0]),&(a->dimension),
               &(a->location));
        areas[counter++] = a;
#ifdef DEBUG_VALUE
        printf("\nArea %d ID: %d Name: %s Dimension: %lf,%lf,%lf Location: %d\n",counter,a->_id,a->name,a->dimension[0],a->dimension[1],a->dimension[2],a->location);
#endif
    }
#ifdef DEBUG_VALUE
    printf("Map Database parsed.\n");
#endif

    if (fclose(fp) == 0) {
        return 0;
    } else return 1;
}

static int read_objs() {
    // read db\objs\objs.txt and store data in chars[]
    FILE *fp;
    GLbyte l[MAX_DB_LINE_LENGTH];
    GLint field_count = 5;  // how many fields there are in a row
    GLint counter = 0;

    fp = fopen("db\\objs\\objs.txt", "r");
    if (fp == 0) {
        fputs("Does object database exist?\n",stderr);
        glutDestroyWindow(glutGetWindow());
        return 1;
    }

    // read chars db into chars array
    while (fgets(l, MAX_DB_LINE_LENGTH, fp) != 0) {
#ifdef DEBUG_VALUE
        //printf("%s", l);
#endif
        // Comments and whitespaces
        if (l[0] == '\0' || l[0] == '\n' || l[0] == ' ' ||
            (l[0] == '/' && l[1] == '/') ||
            (l[0] == 'i' && l[1] == 'd'))
        {
            continue;
        }

        object *o = (object *)malloc(sizeof(object));
        if (o == 0) fputs("Memory Allocation error. read_objs()\n", stderr);

        // Reading attributes
        sscanf(l, "%d %s %lf,%lf,%lf %lf,%lf,%lf %d",
               &(o->_id),
               o->name,
               &(o->dimension[0]),&(o->dimension[1]),&(o->dimension[2]),
               &(o->position[0]),&(o->position[1]),&(o->position[2]),
               &(o->map_id));
        objects[counter++] = o;
        printf("\nObject %d ID: %d Name: %s Dimension: %lf,%lf,%lf Position: %lf,%lf,%lf Location: %d\n",counter,o->_id,o->name,o->dimension[0],o->dimension[1],o->dimension[2],o->position[0],o->position[1],o->position[2],o->map_id);
    }
    printf("Object Database parsed.\n");

    if (fclose(fp) == 0) {
        return 0;
    } else return 1;
}

void read_db() {
#ifdef DEBUG_VALUE
    //char cur_dir[256];
    //GetCurrentDirectory(256, cur_dir);
    //printf("in read_db(): %s\n", cur_dir);
#endif
    GLint charsRead = read_chars();
    GLint mapsRead = read_maps();
    GLint objsRead = read_objs();

    if (charsRead || mapsRead || objsRead) {
        printf("Error reading database!");
        exit_glut();
    }
}
