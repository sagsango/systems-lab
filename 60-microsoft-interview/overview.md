https://jobs.careers.microsoft.com/global/en/job/1836063

1. debug the c program
    structure packing
    pointers, 
2. C Code review
3. Write code
    sytemcall : alarm()

    you have to call back some function after given amount of time.
    ex:



    use will give : time_in_sec  call_back_function_pointer()







    min_heap<int, call_back_function> pq;





    
    void add_alarm(int sec, void (*callback)()) {
          for (;;) {
              lock();
              top_elem = pq.top(); pq.pop();
              unlock();
                10
              alarm(top_elem.sec);
              top_elem.callback();
          }

    }

    void add_alarm(int sec, void (*callback)()) {
        lock();
        5
        pq.insert ({sec, callback});
        unlock();
    }

4. deep copy of linked list with random pointers
