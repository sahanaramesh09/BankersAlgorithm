Banker’s Algorithm
For this project,  a multithreaded program implements the banker’s algorithm. Several customers request and release resources from the bank. The bankerwill grant a request only if it leaves the system in a safe state. A request that leaves the system in an unsafe state
will be denied. This programming assignment combines three separate topics:
(1) multithreading, (2) preventing race conditions, and (3) deadlock avoidance.

The Banker
The banker will consider requests from n customers for m resources types. The banker will keep track of the resources using the
following data structures:

/* these may be any values >= 0 */
#define NUMBER OF CUSTOMERS 5
#define NUMBER OF RESOURCES 3

/* the available amount of each resource */
int available[NUMBER OF RESOURCES];

/*the maximum demand of each customer */
int maximum[NUMBER OF CUSTOMERS][NUMBER OF RESOURCES];

/* the amount currently allocated to each customer */
int allocation[NUMBER OF CUSTOMERS][NUMBER OF RESOURCES];

/* the remaining need of each customer */
int need[NUMBER OF CUSTOMERS][NUMBER OF RESOURCES];

The Customers
n customer threads created that request and release resources from the bank. The customers will continually loop, requesting and then releasing random numbers of resources. The customers’ requests for resources will be bounded by their respective values in the need array. The banker will grant a request if it satisfies the safety algorithm. If a request does not leave the system in a safe state, the banker will deny it. Function prototypes for requesting and releasing resources are as follows:

int request resources(int customer num, int request[]);

int release resources(int customer num, int release[]);

These two functions return 0 if successful (the request has been granted) and –1 if unsuccessful. Multiple threads (customers) will concurrently controlled through mutex locks to prevent race conditions. Both the Pthreads and Windows APIs provide mutex locks. 

Implementation
You should invoke your program by passing the number of resources of each type on the command line. For example, if there were three resource types, with ten instances of the first type, five of the second type, and seven of the third type, you would invoke your program follows:

./a.out 10 5 7

The available array would be initialized to these values. 
