Issues I ran into:

In the loop; I wasn't really paying attention to what it was doing; therefore I didn't notice
that it was outputing to stdout. Because of this I spent a couple of hours wondering why the
program was acting weird after I implemented my version that did the loop in parallel. After
a while; I decided to check exactly what it was doing and then saw that it was printing to
stdout. I then decided to make an array to store the float numbers and create another loop in
the main program outside of the function that contained the parallelized loop. 
Basically it would do all the calculations in parallel; store it in an array when it was done
with the calculations and then wait for all the other threads to finish before executing the
next loop. The next loop simply outputted the array. 

My implementation could be a little better because it uses a global variable for the 
array and instead of passing the vectors in as arguments from the structure I made, it
is declared as a global variable. It should work fine because the vectors are never
changed in the code. The reason this could be bad is that it might take more time to
read and write(it would only write for the array) from a global variable then if it was
passed as a part of the structure. 

thread = 1

real    1m24.823s
user    1m24.805s
sys     0m0.000s

thread = 2

real    0m43.292s
user    1m24.733s
sys     0m0.004s

thread = 4

real    0m28.579s
user    1m24.713s
sys     0m0.003s

thread = 8
 
real    0m18.288s
user    1m27.374s
sys     0m0.003s
 
My implementation improves SRT considerably; it goes around 4.667 times faster with 8 threads compared
to 1 thread.

