CONST CSUBGEN_VERSION$ = "0.1"

CONST CSUBGEN_MAX_NUM_CMDLINE_ARGS% = 20

'Main entry point. Pass in csub command + arguments as a string.
SUB csubgen(csubgenCmd$)
  LOCAL csubGenCmdLineArgs$(CSUBGEN_MAX_NUM_CMDLINE_ARGS%)
  LOCAL csubGenNumArgs%

  csubgen_parseCmdLine(csubgenCmd$, csubGenCmdLineArgs$(), csubGenNumArgs%)
  
  IF csubGenNumArgs% < 2 THEN
    csubgen_usage
    EXIT SUB
  ENDIF
  
  LOCAL action$ = UCASE$(csubGenCmdLineArgs$(0))
  IF (action$ <> "JOIN") AND (action$ <> "MERGE") THEN
    csubgen_usage
    EXIT SUB
  ENDIF
  
  LOCAL sourceFiles$(CSUBGEN_MAX_NUM_CMDLINE_ARGS%-1)

  'First file should be a .c file  
  sourceFiles$(0) = csubgenCmdLineArgs$(1)
  
  IF RIGHT$(sourceFiles$(0), 2) <> ".c" THEN
    PRINT "Expected .c file but got "+sourceFiles$(0)
    csubgen_usage
    EXIT SUB
  ENDIF
  
  'Remaining files, if any, should be .h
  LOCAL ii%
  
  FOR ii%=2 TO csubGenNumArgs%-1
    sourceFiles$(ii%-1) = csubGenCmdLineArgs$(ii%)
  
    IF RIGHT$(sourceFiles$(ii%-1), 2) <> ".h" THEN
      PRINT "Expected .h file but got "+sourceFiles$(ii%-1)
      csubgen_usage
      EXIT SUB
    ENDIF
  NEXT ii%
  
  'csubGenNumArgs%-1 because first argument is join or merge.
  csubgen_uploadFiles sourceFiles$(), csubGenNumArgs%-1
  csubgen_build sourceFiles$(0), action$
  
  'Check if build was successful before attempting download
  IF rexec_return_code% = 0 THEN
    PRINT "Remote compile OK."
    
    csubgen_downloadBas sourceFiles$(0)
  ELSE
    PRINT "Remote compile failed."
  ENDIF
  
  csubgen_delRemoteFiles sourceFiles$(0), csubGenNumArgs%-1
END SUB

SUB csubgen_uploadFiles fileList$(), numFiles%
  LOCAL ii%
  
  FOR ii%=0 TO numFiles%-1
    rexec "upload "+fileList$(ii%)
  NEXT ii%
END SUB

SUB csubgen_build(sourceFile$, action$)
  'Create basename, without .c extension
  LOCAL sourceFileBase$ = LEFT$(sourceFile$, LEN(sourceFile$)-2)
  rexec "shell make "+sourceFileBase$+" "+LCASE$(action$)
END SUB

SUB csubgen_downloadBas(sourceFile$)
  LOCAL bas$ = LEFT$(sourceFile$, LEN(sourceFile$)-2)+".bas"
  rexec "download "+bas$
END SUB

SUB csubgen_delRemoteFiles(sourceFiles$(), numFiles%)
  IF numFiles% <= 0 THEN
    EXIT SUB
  ENDIF
  
  LOCAL sourceFileBase$ = LEFT$(sourceFiles$(0), LEN(sourceFiles$(0))-2)

  rexec "shell del /F "+sourceFileBase$+".*"
  
  LOCAL ii%
  FOR ii%=1 TO numFiles%-1
    rexec "shell del /F "+sourceFiles$(ii%)
  NEXT ii%
END SUB

SUB csubgen_parseCmdLine(cmdLine$, cmdLineArgs$(), nArgs%)
  LOCAL curPos%=1, startPos%
  LOCAL inWhiteSpace%=1
  LOCAL curArg%=0
  
  DO WHILE (curPos%<=LEN(cmdLine$)) AND (curArg%<CSUBGEN_MAX_NUM_CMDLINE_ARGS%)
    IF inWhiteSpace% THEN
      IF MID$(cmdLine$, curPos%, 1) <> " " THEN
        startPos% = curPos%
        inWhiteSpace% = 0
      ENDIF
    ELSE
      IF MID$(cmdLine$, curPos%, 1) = " " THEN
        cmdLineArgs$(curArg%) = MID$(cmdLine$, startPos%, curPos%-startPos%)
        INC curArg%
        inWhiteSpace% = 1
      ENDIF
    ENDIF
    INC curPos%
  LOOP
  
  IF (inWhiteSpace%=0) AND (curArg% < CSUBGEN_MAX_NUM_CMDLINE_ARGS%) THEN
    cmdLineArgs$(curArg%) = MID$(cmdLine$, startPos%)
    INC curArg%
  ENDIF
  
  nArgs% = curArg%
END SUB

SUB csubgen_usage
  PRINT "CSUBGEN client V"+CSUBGEN_VERSION$+" by Epsilon."
  PRINT
  PRINT "Remotely compile given C file+optional headers and generate CSUB"
  PRINT
  PRINT "csubgen join/merge <C file> [header files]"
END SUB

