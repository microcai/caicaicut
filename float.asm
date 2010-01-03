;	.686
;	.XMM
	CPU 387
;	.XMM
;	.model	flat
	BITS 32
	SECTION .data USE32

	GLOBAL	_ftol2_sse

_ftol2_sse:
_ftol2_pentium4:
	push        ebp  
	mov         ebp,esp 
	sub         esp,8 
	and         esp,0FFFFFFF8h 
	fstp        qword ptr [esp] 
	cvttsd2si   eax,mmword ptr [esp] 
	leave            
	ret              
_ftol2_sse_excpt:
	sub         esp,4 
	fnstc 	    word ptr [esp] 
	pop         eax  
	and         ax,7Fh 
	cmp         ax,7Fh 
	je          _ftol2_sse
	int         3

   
