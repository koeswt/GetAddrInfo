#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

int main(int argc, char **argv)
{

    //-----------------------------------------
    // Declare and initialize variables
    WSADATA wsaData;
    unsigned long retval;
    int i = 1;
    
    struct addrinfo *result = NULL;
    struct addrinfo *ptr = NULL;
    struct addrinfo hints;

	struct sockaddr_in  *sockaddr_ipv4;
    struct sockaddr_in6 *sockaddr_ipv6;
    // struct sockaddr *sockaddr_ip;

    char ipstringbuffer[46];
    unsigned long ipbufferlength = 46;

    // Validate the parameters
    if (argc != 3) {
        printf("usage: %s <hostname> <servicename>\n", argv[0]);
        printf("getaddrinfo provides protocol-independent translation\n");
        printf("   from an ANSI host name to an IP address\n");
        printf("%s example usage\n", argv[0]);
        printf("   %s www.contoso.com 0\n", argv[0]);
        return 1;
    }

    // Initialize Winsock
    retval = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (retval != 0) {
        printf("WSAStartup failed: %d\n", retval);
        return 1;
    }

    //--------------------------------
    // Setup the hints address info structure
    // which is passed to the getaddrinfo() function
    memset( &hints, 0, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_CANONNAME;

    printf("Calling getaddrinfo with following parameters:\n");
    printf("\tnodename = %s\n", argv[1]);
    printf("\tservname (or port) = %s\n\n", argv[2]);
    
	//--------------------------------
	// Call getaddrinfo(). If the call succeeds,
	// the result variable will hold a linked list
	// of addrinfo structures containing response
	// information
    retval = getaddrinfo(argv[1], argv[2], &hints, &result);
    if ( retval != 0 ) {
        printf("getaddrinfo failed with error: %s\n", gai_strerror(retval));
        WSACleanup();
        return 1;
    }

    printf("getaddrinfo returned success\n");
    
    // Retrieve each address and print out the hex bytes
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        printf("getaddrinfo response %d\n", i++);
        printf("\tFamily: ");
        switch (ptr->ai_family) {
            case AF_UNSPEC:
                printf("Unspecified\n");
                break;
            case AF_INET:
                printf("AF_INET (IPv4)\n");
                sockaddr_ipv4 = (struct sockaddr_in *) ptr->ai_addr;
                // the inet_ntop function is available on Windows Vista and later
                printf("\tIPv4 address %s\n",
                    inet_ntop(AF_INET, &sockaddr_ipv4->sin_addr, ipstringbuffer, 46) );
                break;
            case AF_INET6:
                printf("AF_INET6 (IPv6)\n");
                sockaddr_ipv6 = (struct sockaddr_in6 *) ptr->ai_addr;
                printf("\tIPv6 address %s\n",
                    inet_ntop(AF_INET6, &sockaddr_ipv6->sin6_addr, ipstringbuffer, 46) );
                break;
            case AF_NETBIOS:
                printf("AF_NETBIOS (NetBIOS)\n");
                break;
            default:
                printf("Other %ld\n", ptr->ai_family);
                break;
        }
        printf("\tLength of this sockaddr: %d\n", ptr->ai_addrlen);
		if (ptr->ai_canonname)
			printf("\tCanonical name: %s\n", ptr->ai_canonname);
    }
    freeaddrinfo(result);
    WSACleanup();

    return 0;
}
