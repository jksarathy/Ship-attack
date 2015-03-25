#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <inttypes.h>

#include "ships.h"

#define PLACE_SHIP ('+')   /* command to place a new ship */
#define ATTACK ('-')       /* command to attack a location */

int
main(int argc, char **argv)
{

    struct field *f; /* where we keep our ships */
    int command;     /* command char */
    struct ship s;   /* ship we are placing */
    struct position p; /* location to attack */
    int sank;        /* ship we sank */

    if(argc != 1) {
        fprintf(stderr, "Usage: %s\n", argv[0]);
        return 1;
    }
    
    
	//printf("about to create field!\n");
    f = fieldCreate();
   // printf("created field!\n");

    while((command = getchar()) != EOF) {
    	//printf("command: %c\n", command);
        switch(command) {
        //	printf("after switch\n");
            case PLACE_SHIP:
            
            //	printf("In place ship\n");
                if(scanf(" %" SCNu32 " %" SCNu32 "%d %u %c\n", &s.topLeft.x, &s.topLeft.y, &s.direction, &s.length, &s.name) != 5) { 
                    /* not enough args */
                    fprintf(stderr, "Not enough enough args to %c\n", PLACE_SHIP); 
                    return 1; 
                }
                /* else */
                scanf(" %" SCNu32 " %" SCNu32 "%d %u %c\n", &s.topLeft.x, &s.topLeft.y, &s.direction, &s.length, &s.name);
               // printf("Got command to place ship\n");
                
              //  printf("ship name: %c \n", s.name);

                /* fix the direction to match actual definitions */
                s.direction = s.direction ? VERTICAL : HORIZONTAL;

                fieldPlaceShip(f, s);
                sank = NO_SHIP_NAME;

                break;

            case ATTACK:
            	//printf("command to attack\n");
                if(scanf("%" SCNu32 " %" SCNu32 " ", &p.x, &p.y) != 2) {
                    fprintf(stderr, "Not enough enough args to %c\n", ATTACK);
                    return 1;
                }
                /* else */
                sank = fieldAttack(f, p);

                break;

            default:
                /* bad command */
                fprintf(stderr, "Bad command %c\n", command);
                return 1;
                break;
        }
        
        //printf("looped\n");

        printf("%c %zu\n", sank, fieldCountShips(f));
    }

    fieldDestroy(f);

    return 0;
    
}
