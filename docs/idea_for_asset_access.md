The Problem
===========

- One asset might be accessed on various positions by multiple workers.
- The asset might be large.
- The amount of usable file handles is limited.

Idea
====

Create an asset class, wich holds the file handle as well as a memory
mapping to it, as well as a reference count of interested workers.
An asset object is created the first time the asset is requested. Then
the file is opened and memory mapped and the refcount is set to 1.
The asset is than stored into a hash indexed by the filename.
The next time the asset is requested it is found in the hash and the
refcount is increased.
The asset object might be an observer of each worker it uses, so the
worker can inform the asset object when they are done, so that the
reference count can be decreased.
If the reference count goes to zero, the asset object is removed from the
hash and freed.
Each worker in turn has to know how much of the asset is already processed,
so that it can ask for the position it wants the next data from.
As each request is assigned one worker and one request can only access one
asset the worker has to know only one position.
