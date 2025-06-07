/*
 *

===================================================================
IPC Mechanism Comparison: pipe vs FIFO vs POSIX Message Queue (mq)
===================================================================

1. OVERVIEW
-----------

| Feature                     | pipe()                         | FIFO (mkfifo)                    | POSIX Message Queue (mq_*)        |
|-----------------------------|--------------------------------|----------------------------------|-----------------------------------|
| Type                        | Anonymous pipe (in-memory)     | Named pipe (visible in fs)       | Message queue (fs + memory)       |
| Visible in filesystem       | No                             | Yes                              | Yes (/dev/mqueue/)                |
| Communication direction     | Unidirectional (usually)       | Unidirectional or bidirectional  | Bidirectional                     |
| Process relation required   | Yes (e.g., parent-child)       | No (any process can access path) | No(any process can open queue)    |
| Data format                 | Byte stream                    | Byte stream                      | Message boundaries preserved      |
| Blocking behavior           | Yes (default)                  | Yes (default)                    | Yes (can be non-blocking)         |
| Preserves message boundary  | No                             | No                               | Yes                               |
| Supports priorities         | No                             | No                               | Yes                               |
| Max message size            | Limited by pipe buffer         | Same as pipe                     | Configurable (e.g., 8K, 10 msgs)  |
| Performance                 | Fast (memory-only)             | Medium (fs + memory)             | Medium (overhead for metadata)    |
| Setup complexity            | Very simple                    | Simple                           | Moderate                          |
| File descriptor required    | Yes                            | Yes                              | Message queue descriptor          |
| Clean-up required           | No                             | Must unlink() manually           | mq_close() and mq_unlink()        |

2. CODE FILES AND BEHAVIOR
--------------------------

A. pipe_test.c
--------------
- Creates anonymous pipe
- Forks process
- Parent writes, child reads

Sample Output:
  Child received: Hello from parent!

Limitations:
- Only works for related processes.
- No message boundary.

------------------------------------------------------------

B. fifo_writer.c and fifo_reader.c
----------------------------------
- Uses mkfifo("/tmp/myfifo", 0666)
- One process writes, another reads

Sample Output:
  Reader received: Hello via FIFO!

Pros:
- Works across unrelated processes

Cons:
- No message boundary
- Manual FIFO cleanup needed (unlink)

------------------------------------------------------------

C. mq_sender.c and mq_receiver.c
--------------------------------
- Uses mq_open, mq_send, mq_receive
- Can specify priorities and message size

Sample Output:
  Received message: Hello MQ! (prio=5)

Pros:
- Message boundaries and priority support
- Works for unrelated processes

Cons:
- Requires linking with `-lrt`
- Slightly more complex API
- Must manually cleanup with mq_unlink()

3. SUMMARY
----------

Use Cases:

- pipe(): lightweight IPC between related processes (parent-child)
- FIFO: simple communication channel for unrelated processes
- mq: advanced messaging with priority and boundaries, better for structured IPC


          */
