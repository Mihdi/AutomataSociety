#include "networking.h"

void setHintsTCPIPAgnostic(struct addrinfo *hints)
{
	memset(hints, 0, sizeof (struct addrinfo));

	hints->ai_family = AF_UNSPEC; //IPv4 || IPv6, who cares?
	hints->ai_socktype = SOCK_STREAM; //TCP
	hints->ai_flags = AI_PASSIVE; //needed to accept wildcard addresses (so to accept everything)
}

void fillAddrinfoTCPIPAgnostic( struct addrinfo **res)
{
	char *port = NET__NBPORT;

    struct addrinfo hints; //used to specify some criteria that limit the set of sock_addr returned by getaddrinfo
    setHintsTCPIPAgnostic(&hints);

  	//fills an addrinfo structure containing the internet address that can be "bind" or "connect" 
    int addrinfo_error = getaddrinfo(NULL, port, &hints, res);
    if(addrinfo_error)
    {
        errx(EX_OSERR, "Fail gettng address on port %s: %s", port, gai_strerror(addrinfo_error));
    }
}

int getAndBindSocket(struct addrinfo *filled)
{
	int out;
	struct addrinfo *rp;
    //iterate through the gotten addresses trying to bind with one
    for (rp = filled; rp; rp = rp->ai_next)
    {
        out = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

        if (out == -1) continue;

        int value = 1;
        int sSockOptErr = setsockopt(out, SOL_SOCKET, SO_REUSEADDR, &value, sizeof (int));
        if(sSockOptErr == -1) errx(EXIT_FAILURE, "setsockopt error");

        if ( !bind(out, rp->ai_addr, rp->ai_addrlen)) break;

        close(out);
    }
    //obvsly, check errors
    if (!rp) errx(EXIT_FAILURE, "no opened socket");
    
    return out;

}

int getBoundSockTCPIPAgnostic()
{
	int out;

	struct addrinfo *result; //stores the result of getaddrinfo
	fillAddrinfoTCPIPAgnostic(&result);

   	out = getAndBindSocket(result);

    freeaddrinfo(result);

    return out;
}

void servTCPIPAgnostic(void (*lambda)(int, int))
{

    int cnx = getBoundSockTCPIPAgnostic();

    //listen to the socket
    int listeningError = listen(cnx, 5);
    if (listeningError) errx(EXIT_FAILURE, "listen failed");

    struct sockaddr peer_addr;
    socklen_t sizeSockaddr = sizeof(struct sockaddr);

    while(TRUE)
    {
        int acc = accept(cnx, &peer_addr, &sizeSockaddr);
        if (acc == -1) errx(EXIT_FAILURE, "failed to accept()");

        lambda(cnx, acc);

        close(acc);
    }

    close(cnx);
}