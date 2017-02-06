#ifndef __UUID_H__
#define __UUID_H__
#include <sys/time.h> 	/* timeval */
#include <sys/types.h> 	/* pid */

typedef struct uuid
{
	pid_t 	pid;
	struct 	timeval create_time; /* time of creation of task */
	size_t 	counter; /* serial number */
}uuid_t;

uuid_t UIDCreate(void);

int UIDIsSameId(uuid_t uid_a, uuid_t uid_b);

uuid_t UIDGetInvalid(void);

#endif /*__UUID_H__*/
