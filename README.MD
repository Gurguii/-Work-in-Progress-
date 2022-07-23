**Work in Progress**  
This is my fuzzing tool project, I have to better understand sockets, build functions etc. I just wanna have this here so I won't accidently rm it :D and so I can practice git.  
**Update 23/07/2022**  
- Big problem: when I try this locally it reads files super quick, testing it on HTB went super slow. It was like 2 tries per second. At first I tried multithreading(my own code so probably not the best efficiency) but it wasn't faster(plus locally it was singlethreaded and did 4600 words in a few secs), and I'm not sure how to fix this. I think it has to be something related with the server getting way too many requests, will try some delay in-between requests or I don't know. 
- Another problem: my argparser code isn't working properly and I can't find the problem. Might be something silly I can't see now, my main problem here is that I'm building a slow fuzzing tool.
