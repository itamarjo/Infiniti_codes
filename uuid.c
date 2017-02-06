#include <stdlib.h>
#include <stddef.h>		/* size_t */
#include <sys/time.h>


#include "uuid.h"

static size_t count = 0;

uuid_t UIDCreate(void)
{
	uuid_t uuid;

	if (gettimeofday(&(uuid.create_time), 0) == -1)
	{
		return(UIDGetInvalid());
	}

	uuid.pid = getpid();
	uuid.counter = count;
	++count;

	return(uuid);
}

int UIDIsSameId(uuid_t uid_a, uuid_t uid_b)
{
	return((uid_a.pid == uid_b.pid) && (uid_a.counter == uid_b.counter)
		&& !(timercmp(&uid_a.create_time, &uid_b.create_time, !=)));
}

uuid_t UIDGetInvalid(void)
{
	uuid_t uuid;

	uuid.pid = 0;
	timerclear(&uuid.create_time);
	uuid.counter = 0;

	return(uuid);
}
