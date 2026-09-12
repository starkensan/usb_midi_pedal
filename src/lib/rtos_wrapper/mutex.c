#include "mutex.h"

bool mutex_lock(mutex_t *mutex, uint32_t timeout_ms)
{
    if ((mutex == NULL) || (mutex->context == NULL) || (mutex->operations == NULL)
        || (mutex->operations->lock == NULL)) {
        return false;
    }

    return mutex->operations->lock(mutex->context, timeout_ms);
}

bool mutex_unlock(mutex_t *mutex)
{
    if ((mutex == NULL) || (mutex->context == NULL) || (mutex->operations == NULL)
        || (mutex->operations->unlock == NULL)) {
        return false;
    }

    return mutex->operations->unlock(mutex->context);
}
