'
'  CFGEN.BAS  V1.43
'    V1.1  Improved algorithms thanks to TZ
'    V1.11 Tidy Up
'    V1.12 Fixed bug in symbol table loop (thanks Jim)
'    V1.2  Added getting static data from  .rodata
'    V1.3  deal with .dinit between .text and .rodata
'    V1.31  minor formatting issue on output
'    V1.4  Output symbol names of static data
'    V1.41 Correct minor formatting issues when .rodata not used
'    V1.42 Sort function name output, block output in CONSTANTS declared in Join mode
'    V1.43 Cnumsymbols2 version
'    V1.43.unofficial.1 Unofficial tweaks by Epsilon.
'
'  Program to convert a .ELF file to Basic statements formated for use as CFunctions
'  Runs under MMBasic for DOS V4.5
'
'  It is reconumsymbolsended that the .ELF file is copied to the same directory as this file
'  and that the filename is shortened to make life easier
'
'  To run open a DOS box on the computer
'  navigate to the directory containing the MMBasic.exe file and this program
'  run MMBasic and you will get the normal ">" prompt
'  Load this program
'  Type run
'
'  Then give the filename of the .elf file including the extension
'  Choose "join" or "merge" to define the output format
'      "join" is typically used for individual CFunctions, each function in the .ELF file is given
'      its own header, all the CFunctions are output to a single file but can then be copied and
'      pasted as required. When "join is chosen the "main" function is a dunumsymbolsy and is not included in the output
'      "merge" is used for CFunctions such as drivers where one C function can be called from another
'
'  Choose the name of the file for the Basic output. Do not include ".BAS", this will be appended automatically
'
'  You can run MMBasic for DOS from MMEDIT, but in this case you will need to specify the correct directory information for the .ELF file
'
dim section_header, section_header_size, section_header_num
dim string_table_sector, string_table, text_section, text_position, text_size, text_link, symtab_section, strtab_section
dim symtab_offset, symtab_num, symstr_offset, first

DIM rodata_position, rodata_size, rodata_section,rodata_address$
DIM dinit_section, dinit_address$, pad=0

dim text_add$ length 31
dim symbols$(100,2) length 31
'************************************************************************ change 1
dim symbolsort$(100) length 45
'************************************************************************

'== EPSILON added ==>
dim cmdLine$

'If no arguments are passed in, the INSTR below will return 0.
IF INSTR(MM.CMDLINE$, " ") = 0 THEN
  cmdLine$ =""
ELSE
  cmdLine$ = RIGHT$(MM.CMDLINE$, LEN(MM.CMDLINE$) - INSTR(MM.CMDLINE$, " "))
ENDIF
'<== EPSILON added

'EPSILON replaced:
'line input ".ELF filename ? ",inname$
'with:
inname$ = cmdLine$

'EPSILON deleted:
'if ucase$(right$(inname$,4))<>".ELF" then inname$=inname$+".elf"
'do
''   line input "mode(join, merge) ? ",m$
'loop while (m$<>"join") and (m$<>"merge") and (m$<>"j") and (m$<>"m")
mode =1

'EPSILON replaced:
'if m$="merge" or m$="m" then mode=1
'line input "CFunction name ? ",cname$
'with:
cname$ = LEFT$(inname$, LEN(inname$)-4)+".BAS"

'EPSILON deleted:
'if ucase$(right$(cname$,4))<>".BAS" then cname$=cname$+".bas"
print "Created "+cname$
open cname$ for output as #2
PRINT #2,"'File "+cname$ written "+DATE$+" "+Time$
first=1 'first time into the various loops
print "processing ",inname$
open inname$ for random as 1
zeroseek 1, 0
'
dat$ = INPUT$(32, 1) ' Get the .elf identification and unused header
dat$=input$(4,1)
section_header=getnum4(dat$) 
dat$=input$(10,1) 'step past the flags ,header size, and program header info
dat$=input$(2,1) 
section_header_size=getnum2(dat$)
dat$=input$(2,1) 
section_header_num=getnum2(dat$)
dat$=input$(2,1) 
string_table_sector=getnum2(dat$)
'
i=section_header+ (Section_header_size*string_table_sector) + 16 'get to the offset in the string table sector
zeroseek 1,i
dat$=input$(4,1) 
string_table= getnum4(dat$) 'get the offset to the string table
'************************************************************* change 2
' Delete from here: moved to after test for invalid mode usage
'Print "Functions found:"
'print "Address",chr$(9)+"Function"
'*************************************************************
for j=0 to section_header_num-1
    i=section_header+ (Section_header_size*j) 'get to the next sector header
    zeroseek 1,i 'point to the .text section header
    dat$=input$(4,1) 'get the pointer to the sector name string
    k=getnum4(dat$)
    zeroseek 1,k+string_table 'point to the name string for this sector
    s$=""
    d$=input$(1,1)
    do while asc(d$)<>0 'read in the zero terminated string
        S$=S$+D$
        d$=input$(1,1)
    loop
    if s$=".text" and text_section=0 then 'store text sector number
      text_section=j 
    endif
    if s$=".symtab" and symtab_section=0 then 'store symbol table sector number
      symtab_section=j 
    endif
    if s$=".strtab" and strtab_section=0 then 'store symbol table strings sector number
      strtab_section=j 
    endif      

    if s$=".rodata" and rodata_section=0 then 'store rodata sector number
      rodata_section=j 
    endif
    if s$=".dinit" and dinit_section=0 then 'store rodata sector number
      dinit_section=j 
    endif

next j
'
i=section_header+ (Section_header_size*text_section) 'get to the .text offset 
zeroseek 1,i 'point to the .text section header
dat$=input$(12,1) 
dat$=input$(4,1)
text_add$=getadd$(dat$)
dat$=input$(4,1)
text_position=getnum4(dat$)
dat$=input$(4,1)
text_size= getnum4(dat$)
dat$=input$(4,1)
text_link= getnum4(dat$)
'
i=section_header+ 16+ (Section_header_size*(symtab_section)) 
zeroseek 1,i 'point to the symbol table section header
dat$=input$(4,1) 'get the offset to the symbol table
symtab_offset=getnum4(dat$)
dat$=input$(4,1)
symtab_num=getnum4(dat$)/16
'
i=section_header+ 16+ (Section_header_size*(strtab_section)) 
zeroseek 1,i 'point to the string table section header
dat$=input$(4,1) 'get the offset to the symbol table
symstr_offset=getnum4(dat$)

'

i=section_header+ (Section_header_size*(rodata_section)) 
zeroseek 1,i 'point to the string table section header
dat$=input$(12,1) 
dat$=input$(4,1)
rodata_address$=getadd$(dat$)
dat$=input$(4,1)
rodata_position=getnum4(dat$)
dat$=input$(4,1)
rodata_size= getnum4(dat$)

if rodata_size<>0 and mode=0 then
  Print "Error: Constant data not allowed in JOIN mode"
  close #2
  end
endif
i=section_header+ (Section_header_size*(dinit_section)) 
zeroseek 1,i 'point to the string table section header
dat$=input$(12,1) 
dat$=input$(4,1)
dinit_address$=getadd$(dat$)
dat$=input$(4,1)
dat$=input$(4,1)
dinit_size= getnum4(dat$)
'
if (rodata_address$>dinit_address$) and (dinit_address$>test_add$) and rodata_size<>0 then
   print dinit_size," bytes padding needed for dinit"
   pad=dinit_size
endif

'

numsymbols=0 'counter for the stored symbols array
for i=0 to symtab_num-1 'loop through all the symbols
    zeroseek 1,symtab_offset+(16*i) 'point to the next symbol in the .symtab section 
    dat$=input$(4,1)
	k=getnum4(dat$)
	dat$=input$(4,1)
	a$=getadd$(dat$)
	dat$=input$(4,1)
	dat$=input$(1,1)
	l=asc(dat$) 
	zeroseek 1,k+symstr_offset 'point to the symbol name
  s$=""
  d$=input$(1,1)
  do while asc(d$)<>0
    S$=S$+D$
    d$=input$(1,1)
  loop
	if (l=18 and left$(s$,1)<>"_") or l=1 then 'Only store function names or data
	  symbols$(numsymbols,1)=a$
	  symbols$(numsymbols,2)=s$
    
'**************************************************************************************** change 3
'  now puts symbols and addresses into an array sorting as we go for output in the correct order
'
		if l<>1 then 
      insertsort(numsymbols,symbols$(numsymbols,1)+CHR$(9)+symbols$(numsymbols,2))
    endif
		numsymbols=numsymbols+1
		if s$="main" then main_offset$=a$
	endif
next i
'
Print "Functions found:"
print "Address",chr$(9)+"Function"
for i=0 to numsymbols-1
print symbolsort$(i)
'  print hex$(val(("&H"+left$(symbolsort$(i),8)))-1),right$(symbolsort$(i),len(symbolsort$(i))-8)
next i
'****************************************************************************************
'
main$= right$("00000000" + hex$(subtract(right$(main_offset$,5), right$(text_add$,5))), 8) 'calculate the offset  
'
zeroseek 1,text_position
k=0
currentfunction=0
s$=text_add$
for i=1 to text_size step 4
    for j=0 to numsymbols-1
        if val("&H"+s$)=val("&H"+symbols$(j,1))-1 then 'address matches the start of a function
	        currentfunction=j
	        if k<>0 and mode then print #2,"" 'The last function didn't end with a newline so output one
	        if k<>0  and (not mode) and symbols$(j,2)<>"main" then print #2,"" 
	        k=0
	        if first and mode then ' First time in for a merged file output the Cfunction name
	   	        print #2,"CSUB "+cname$
  	            print #2,chr$(9)+main$
	        endif
	        if mode then print #2,chr$(9)+"'"+symbols$(j,2)
	        if not mode and symbols$(j,2)<>"main" then
	        if not first then 
		        print #2,"End CSUB"
		        print #2,"'"
		    endif
	   	    print #2,"CSUB "+symbols$(j,2)
   	        print #2,chr$(9)+"00000000"
 	    endif
	    first=0
    endif
    next j
    if k=0 then print #2,chr$(9);
    dat$=getadd$(input$(4,1))
    if not mode and symbols$(currentfunction,2)<>"main" then print #2, dat$+" ";
    if mode then print #2, dat$+" ";
    k=k+1
    if k=8 then 'deal with line feed every eight words
        if not mode and symbols$(currentfunction,2)<>"main" then print #2,""
        if mode then print #2,""
	    k=0
    endif
    increment4(s$) 'step on the program counter 
next i

if pad<>0 then
if k<>0 then print #2,""
print #2,chr$(9)+"'.dinit"
k=0
for i=1 to pad step 4
  if k=0 then print #2,chr$(9);
  dat$="00000000"
  print #2,dat$+" ";
  k=k+1
  if k=8 then
    print #2,""
    k=0
  endif
next i

endif
'
if k<>0 then print #2,""
if rodata_size<>0 then print #2,chr$(9)+"'.rodata"
zeroseek 1,rodata_position
k=0
s$=rodata_address$
for i=1 to rodata_size step 4
    for j=0 to numsymbols-1
        if s$=symbols$(j,1) then 
	        if k<>0 and mode then print #2,"" 'The last function didn't end with a newline so output one
          dat$=symbols$(j,2)
          m=instr(dat$,".")
          dat$=left$(dat$,numsymbols-1)
          print #2,chr$(9)+"'"+dat$
          k=0
        endif
    next j
  if k=0 then print #2,chr$(9);
  dat$=getadd$(input$(4,1))
  print #2,dat$+" ";
  k=k+1
  if k=8 then
    print #2,""
    k=0
  endif
  increment4(s$) 'step on the program counter 
next i
if k<>0 then print #2,""

print #2,"End CSUB"
close 1
close #2
'
QUIT
end
'******************************************************************************** change 4
sub insertsort(n,new$)
  local i,j
  i=1
  do while new$> symbolsort$(i-1) and i<=n
    i=i+1
  loop
  if i-1<n then
    for j=n+1 to i+1 step -1
      symbolsort$(j-1)=symbolsort$(j-2)
    next j
  endif
  symbolsort$(i-1)=new$
end sub
'************************************************************************************
function subtract(b$,c$)
  local f,g
   f=val("&H"+b$)
   g=VAL("&H"+c$)
  subtract=(f-g-1)/4
end function

function getjumpaddress$(a$)
  local f
  f=VAL("&H"+a$)*4+1048576
  getjumpaddress$=left$(text_add$,3)+right$(hex$(f),5)
end function

function getadd$(e$)
   local a,b,c,d
   a=asc(left$(e$,1))+256
   b=asc(mid$(e$,2,1))+256
   c=asc(mid$(e$,3,1))+256
   d=asc(right$(e$,1))+256
   getadd$=right$(hex$(d),2) +right$(hex$(c),2) +right$(hex$(b),2) +right$(hex$(a),2)
end function

function getnum4(e$)
   local a,b,c,d
   a=asc(left$(e$,1))
   b=asc(mid$(e$,2,1))
   c=asc(mid$(e$,3,1))
   d=asc(right$(e$,1))
   getnum4=((((d*256)+c)*256)+b)*256+a
end function

function getnum2(d$)
   local a,b
   a=asc(left$(d$,1))
   b=asc(right$(d$,1))
   getnum2=b*256+a
end function

sub increment4(value$) 
    Local V$(7) LENGTH 1, i, NewValue$ LENGTH 8 
    local vv
    for i= 0 to 7: V$(i) = mid$(value$, i+1, 1): next 
    V$(7) = Chr$(Asc(V$(7)) + 4) 
    for i = 7 to 1 step -1 
       vv = Asc(V$(i))  
       if vv > 70 then ' "F" 
          V$(i) = Chr$(vv - 23): V$(i-1) = Chr$(Asc(V$(i-1)) + 1) 
       else 
         if vv >57 and vv < 65 then V$(i) = Chr$(vv + 7) ' "9" "A" 
       endif 
       NewValue$ = V$(i)+ NewValue$ 
    next 
    value$ = V$(0) + NewValue$ 
end sub 

function subaddress$(p$,b)
   local a
   a=val("&H"+RIGHT$(p$,4))+65536-b
   subaddress$=left$(p$,4)+right$(hex$(a),4)
end function

sub zeroseek(i,j)
	seek 1,j+1
end sub