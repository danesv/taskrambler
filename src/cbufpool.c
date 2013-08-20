/**
 * As the initializations of cbufs is complicated and time consuming
 * with all this shared memory initialization stuff and each cbuf
 * always is the same (at least for this application) we don't free
 * them after they are not used anymore.
 * Instead we store the in this pool and reuse then the next time
 * they are needed.
 *
 * Well thats the idea of this.
 */
