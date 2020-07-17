#ifndef NETWORKING_H
#define NETWORKING_H

#define NET__NBPORT "1970"

#include <err.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "misc.h"

void servTCPIPAgnostic();

#endif