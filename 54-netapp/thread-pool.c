Design a thread pool library!
1. No code required
2. Only discuss the design and write the struct and function definition
3. It important to get the matrix, so that other teams/users will be
   able to analyis the systems load, thread unitilzation.
4. max number of threads is N, at the beginig pool will start with  some
   const number of threads lets say 5, and according to the load the nu-
   mber of threads will increase or decreaase.
6. I suggested design where one thread will be the leader thread, which
   will be responsible from direct intraction with the user (submit task
   , get the matix, etc) and will responsilbe for the scaling the pool
   size, and decreasing the pool size.
7. tasks was - function poiter & arg pointer.
