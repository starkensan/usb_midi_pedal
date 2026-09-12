#include "semaphore.h"

bool semaphore_give(semaphore_t *semaphore)
{
    if ((semaphore == NULL) || (semaphore->context == NULL) || (semaphore->operations == NULL)
        || (semaphore->operations->give == NULL)) {
        return false;
    }

    return semaphore->operations->give(semaphore->context);
}

bool semaphore_take(semaphore_t *semaphore, uint32_t timeout_ms)
{
    if ((semaphore == NULL) || (semaphore->context == NULL) || (semaphore->operations == NULL)
        || (semaphore->operations->take == NULL)) {
        return false;
    }

    return semaphore->operations->take(semaphore->context, timeout_ms);
}
