*******************************************************************
******                      Readme File                      ******
******                        fPLScsi                        ******
******                for DOS/Windows/Linux/Mac              ******
******                      version 1.0                      ******
******                                                       ******
******                       July 2004                       ******
*******************************************************************



************
* Contents *
************

This is a forked version of PLScsi (hence the name) using the  "old
style" PLScsi API with the added bonuses of:
- Mac OSX support (stuc.cpp)
- DOS 32 bits support (DPMI/DJGPP) (dos32aspi.cpp)
- No more dependancies on the Microsoft DDK for  SPTX  (ntddscsi.h, 
  ntiocaps.h)

With these modifications, fPLScsi offers *true*  CORE  (Code  Once, 
Run Everywhere) for SCSI passthru device access  on  DOS,  Windows,
Linux and now Mac platforms.

For  an  example  of  the  fPLScsi  API  or  the  "Code  Once,  Run 
Everywhere" paradigm in action, have a look at the "dvdzone" source 
which should be available for download on the same page  where  you
downloaded this file.



***********
* Credits *
***********

The original code of PLScsi was found  on  the  web  and  the  only 
things we added to it were the DOS32/DJGPP part (by >NIL:), and the
brand new Mac part (by xvi) as well a minor changes here and there.
So really, we do not take any credit for  this  forked  version  of 
PLScsi. All the original work was developed by somebody else.



*************
* Copyright *
*************

The copyright is rather fuzzy to say the least, but assumed at this
stage to be public domain.

On the copyright issue of the  original  PLScsi  code,  its  author
originally  said  that  "(it  is  c)onsciously  unstated,   because 
unclear". However, there now exist strong indications  that  nobody
else but the original author can claim the rights to  the  original 
and/or derivative works.
In this eventuality, the original author clearly indicated his "own
preference to dedicate (his) work to the public domain".

It is therefore assumed, until  proven  otherwise,  that  the  code 
published here is public domain, and that  any  mistake  in  making 
that assumption is an honest mistake from us - the authors  of  the 
derivative works. Neither us - the authors of the derivative works,
nor the author of the original works, nor the sites  hosting  these 
works  should  be  held  responsible  if  this  assumption   proves 
incorrect.

Note however that the original  author  was  not  consulted  before
this derivative work was released.

Still on the subject of copyrights, the  original  dependancies  on 
copyrighted Microsoft DDK files (ntddscsi.h, ntiocaps.h) have  been
eliminated and these files have  been  replaced  respectively  with 
ntddscsi.hpp of gccscsi, and part of ScsiIf-nt.cc of cdrdao (GPL).
To  the  best  of  our  knowledge,  this  makes  the  whole  source 
copyright free.

Finally, please note that ALL the tools you need  to  compile  your 
applications on any of the platforms can be obtained for free ;)