/**
 * As the initializations of cbufs is complicated and time consuming
 * with all this shared memory initialization stuff and each cbuf
 * always is the same (at least for this application) we don't free
 * them after they are not used anymore.
 * Instead we store the in this pool and reuse then the next time
 * they are needed.
 *
 * Well thats the idea of this.
 *
 * OK, after review the code...it has been some time since I wrote it,
 * I realize that only one cbuf is used right now.
 * Each connection holds their unprocessed data in another heap reagion
 * and reinitializes the ringbuffer each time with this.
 *
 * This seems both inefficient and error prone. So I will change this.
 * The only question is, how large should our circular buffer be and
 * how many connections do we expect in paralell...
 *
 * We need to have some handling to not accept any more connection
 * when we reached the maximum amount for cbuffers and none is left in
 * the pool.
 */
