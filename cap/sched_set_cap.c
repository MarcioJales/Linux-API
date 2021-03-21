/* Listing 35-2 */

/* sched_set.c

   Usage: sched_set policy priority pid...

   Sets the policy and priority of all process specified by the 'pid' arguments.

   See also sched_view.c.

   The distribution version of this code is slightly different from the code
   shown in the book in order to better fix a bug that was present in the code
   as originally shown in the book. See the erratum for page 743
   (http://man7.org/tlpi/errata/index.html#p_743).
*/

/* Chapter 39, exercise 1
 *
 * Modify the program in Listing 35-2 (sched_set.c, on page 743) to use file capabilities, so that it can be used by an unprivileged user.
 */

#include <sched.h>
#include <sys/capability.h>
#include "tlpi_hdr.h"

// "static" key work was used to stop warning about prototype
static int
modifyCap(int capability, int cap_op)
{
    cap_t caps;
    const cap_value_t cap_array[] = { capability };

    caps = cap_get_proc();
    if (caps == NULL)
        return -1;

    if (cap_set_flag(caps, CAP_EFFECTIVE, 1, cap_array, cap_op) == -1) {
        cap_free(caps);
        return -1;
    }

    if (cap_set_proc(caps) == -1) {
        cap_free(caps);
        return -1;
    }

    if (cap_free(caps) == -1)
        return -1;
    
    return 0;
}

int
main(int argc, char *argv[])
{
    int j, pol;
    struct sched_param sp;

    if (argc < 3 || strchr("rfo"
#ifdef SCHED_BATCH              /* Linux-specific */
                "b"
#endif
#ifdef SCHED_IDLE               /* Linux-specific */
                "i"
#endif
                , argv[1][0]) == NULL)
        usageErr("%s policy priority [pid...]\n"
                "    policy is 'r' (RR), 'f' (FIFO), "
#ifdef SCHED_BATCH              /* Linux-specific */
                "'b' (BATCH), "
#endif
#ifdef SCHED_IDLE               /* Linux-specific */
                "'i' (IDLE), "
#endif
                "or 'o' (OTHER)\n",
                argv[0]);

    pol = (argv[1][0] == 'r') ? SCHED_RR :
                (argv[1][0] == 'f') ? SCHED_FIFO :
#ifdef SCHED_BATCH              /* Linux-specific, since kernel 2.6.16 */
                (argv[1][0] == 'b') ? SCHED_BATCH :
#endif
#ifdef SCHED_IDLE               /* Linux-specific, since kernel 2.6.23 */
                (argv[1][0] == 'i') ? SCHED_IDLE :
#endif
                SCHED_OTHER;

    sp.sched_priority = getInt(argv[2], 0, "priority");

    if (modifyCap(CAP_SYS_NICE, CAP_SET) == -1)
        fatal("raiseCap() failed");

    for (j = 3; j < argc; j++)
        if (sched_setscheduler(getLong(argv[j], 0, "pid"), pol, &sp) == -1)
            errExit("sched_setscheduler");
    
    if (modifyCap(CAP_SYS_NICE, CAP_CLEAR) == -1)
        fatal("raiseCap() failed");

    exit(EXIT_SUCCESS);
}
