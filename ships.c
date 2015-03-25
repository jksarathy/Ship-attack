#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "ships.h"
#include "field.h"


/* key operations */
static unsigned long posHash(const void *pos)
{
	unsigned long h;	
	h = ((struct position *)pos)->x * 137 + ((struct position *)pos)->y * 197;
	return h;
}

static int posEqual(const void *pos1, const void *pos2) 
{
    return ((struct position *)pos1)->x == ((struct position *)pos2)->x 
    	&& ((struct position *)pos1)->y == ((struct position *)pos2)->y;
}    

static void *posCopy(const void *pos) 
{
    struct position *pos2;

    pos2 = malloc(sizeof(struct position));
    pos2->x = ((struct position *)pos)->x;
    pos2->y = ((struct position *)pos)->y;
    return (void *) pos2;
}

static void posFree(void *pos) {
	if (pos) {
		free(pos);
	}
}

struct keyOperations posOps = {
    posHash,
    posEqual,
    *posCopy,
    posFree
};

/* value operations */
static void *shipCopy(const void *ship)
{
	struct ship *ship2;
	
	ship2 = (struct ship *) malloc(sizeof(struct ship));
	//ship2->topLeft = *(struct position *) malloc(sizeof(struct position));
	ship2->topLeft.x = ((struct ship *)ship)->topLeft.x;
	ship2->topLeft.y = ((struct ship *)ship)->topLeft.y;
	ship2->direction = ((struct ship *)ship)->direction;
	ship2->length = ((struct ship *)ship)->length;
	ship2->name = ((struct ship *)ship)->name;
	
	return (void *) ship2;
}

static void shipFree(void *ship) 
{
	//printf("freeing ship\n");
	if (ship) {
		/*if (((struct ship *)ship)->topLeft) {*/
		//printf("ship exists\n");
		if (&(((struct ship *)&ship)->topLeft)) {
			//printf("pos exists\n");
			//free(&( ((struct ship *)&ship)->topLeft));
			/*posFree( (void *) &( ((struct ship *)&ship)->topLeft));*/
			//printf("freed pos\n");
		} 
		
		free(ship);
		//printf("freed ship\n");
	}
}

struct valueOperations shipOps = {
	*shipCopy,
	shipFree
};


struct field *fieldCreate(void) {
	return fCreate(posOps, shipOps);
}

void fieldDestroy(struct field *f) {
	fDestroy(f); 
}

void fieldPlaceShip(struct field *f, struct ship s) {

	int i;
	struct position currentPos;
	const struct ship *oldShip;
	//printf("placing ship...\n");

	if (s.length > 0 
		&& s.length <= MAX_SHIP_LENGTH && s.name != NO_SHIP_NAME) {
		
		if (s.direction == HORIZONTAL && s.topLeft.x <= COORD_MAX - s.length + 1 && s.topLeft.y <= COORD_MAX) { 
			for (i = 0; i < s.length; i++) {			
				currentPos.x = s.topLeft.x + i;
				currentPos.y = s.topLeft.y;
				
				oldShip = (const struct ship *) fGet(f, (const void *) &currentPos); 
				if (oldShip) {
					//printf("ship position: %" SCNu32 " %" SCNu32 " name: %c p\n", currentPos.x, currentPos.y, oldShip->name);
				/*	struct position oldShipPos;
					oldShipPos.x = oldShip->topLeft.x;
					oldShipPos.y = oldShip->topLeft.y; 
					printf("removed %c\n", fieldAttack(f, oldShipPos));*/
					//printf("removed %c\n", fieldAttack(f, oldShip->topLeft));
					fieldAttack(f, oldShip->topLeft);
				}
				oldShip = NULL;		
				fSet(f, (const void *) &currentPos, (const void *) &s);
			}
			fAddShip(f);
		} else if (s.direction == VERTICAL && s.topLeft.x <= COORD_MAX && s.topLeft.y <= COORD_MAX - s.length + 1) {
			for (i = 0; i < s.length; i++) {
				currentPos.x = s.topLeft.x;
				currentPos.y = s.topLeft.y + i;
				
				oldShip = (const struct ship *) fGet(f, (const void *) &currentPos); 
				
				if (oldShip) {
					//printf("ship position: %" SCNu32 " %" SCNu32 " name: %c \n", currentPos.x, currentPos.y, oldShip->name);
					/*struct position oldShipPos;
					oldShipPos.x = oldShip->topLeft.x;
					oldShipPos.y = oldShip->topLeft.y; 
					printf("removed %c\n", fieldAttack(f, oldShipPos));*/
					//printf("removed %c\n", fieldAttack(f, oldShip->topLeft));
					fieldAttack(f, oldShip->topLeft);
				}		
				oldShip = NULL;
				fSet(f, (const void *) &currentPos, (const void *) &s);
			}
			fAddShip(f);
		}
	}	
}	

char fieldAttack(struct field *f, struct position p) {
	int i;
	const struct ship *oldShip;
	int length;
	char name;
	coord initX;
	coord initY;
	//printf("in field attack\n");
	
	//printf("position: %" SCNu32 " %" SCNu32, p.x, p.y);
	
	oldShip = (const struct ship *) fGet(f, (const void *) &p); /*shipCopy(fGet(f, (const void *) &p));*/
	
	if (oldShip) {
		//printf("old ship exists\n");
		//printf("old ship length: %u \n", oldShip->length);
		struct position oldShipPos;
		initX = oldShip->topLeft.x;
		initY = oldShip->topLeft.y; 
		length = oldShip->length;
		name = oldShip->name;
		
		//printf("oldship: position- %" SCNu32 " %" SCNu32 " length: %u name: %c\n", oldShipPos.x, oldShipPos.y, length, name);
		if (name == NO_SHIP_NAME) {return NO_SHIP_NAME;}
		
		if (oldShip->direction == HORIZONTAL) {
			for (i = 0; i < length; i ++) { //already freed this ship so it can't stop going through this loop
				oldShipPos.x = initX + (coord) i;
				oldShipPos.y = initY;
				//printf("oldship : %" SCNu32 " %" SCNu32 " : %c \n", oldShipPos.x, oldShipPos.y, name);
				fSet(f, (const void *) &oldShipPos, NULL); 
			}
			fDeleteShip(f);
		}
		
		else if (oldShip->direction == VERTICAL) {
			for (i = 0; i < length; i ++) {
				oldShipPos.x = initX;
				oldShipPos.y = initY + (coord) i;
				//printf("oldship : %" SCNu32 " %" SCNu32 " : %c \n", oldShipPos.x, oldShipPos.y, name);
				fSet(f, (const void *) &oldShipPos, NULL);
			}
			fDeleteShip(f);
		}
		//printf("deleted ship \n");
		//fDeleteShip(f);
		return name;
	}
	else {
		return NO_SHIP_NAME;
	}
}	

size_t fieldCountShips(const struct field *f){

	return fNumShips(f);

}
