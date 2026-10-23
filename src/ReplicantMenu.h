#ifndef REPLICANTMENU_H
#define REPLICANTMENU_H

#include <Point.h>
#include <SupportDefs.h>

class BView;

void sendDateReplicantMessage(uint32 what);
void showReplicantMenu(BView *owner, BPoint where,
	const char *deskbarItemName = NULL);

#endif /* REPLICANTMENU_H */
