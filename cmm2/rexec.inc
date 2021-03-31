CONST REXEC_VERSION$ = "0.1"

CONST REXEC_KEY$ = "REXEC07101972 "
CONST REXEC_DONE_KEY$ = "REXEC07101972_DONE"
CONST XMODEM_SEND_CMD$ = "xmodem_send_to_pc"
CONST XMODEM_RECEIVE_CMD$ = "xmodem_receive_from_pc"
CONST SHELL_CMD$ = "shell_cmd_to_pc"
CONST SHELL_RET$ = "shell_ret_from_pc"
CONST SHELL_OUT$ = "shell_out_from_pc"
CONST SHELL_ERR$ = "shell_err_from_pc"
CONST QUIT_CMD$ =  "quit"

CONST MAX_NUM_REXEC_ARGS% = 20
CONST MAX_CHUNK_SIZE% = 128

'Check this after running a rexec command
'0 is OK. Non-zero is error.
DIM rexec_return_code% = 0

'Main entry point. Pass in rexec command + arguments as a string.
SUB rexec(rexecCmd$)
  LOCAL rexecArgs$(MAX_NUM_REXEC_ARGS%)
  LOCAL nRexecArgs%

  rexec_parseCmdLine(rexecCmd$, rexecArgs$(), nRexecArgs%)

  LOCAL action$ = UCASE$(rexecArgs$(0))
  IF (action$ = "UPLOAD") THEN
    rexec_handleUpload(rexecArgs$(), nRexecArgs%)
  ELSEIF (action$ = "DOWNLOAD") THEN  
    rexec_handleDownload(rexecArgs$(), nRexecArgs%)
  ELSEIF (action$ = "SHELL") THEN  
    rexec_handleShell(rexecArgs$(), nRexecArgs%)
  ELSEIF (action$ = "QUIT") THEN  
    rexec_handleQuit(rexecArgs$(), nRexecArgs%)
  ELSE
    rexec_usage
    EXIT SUB
  ENDIF
END SUB

SUB rexec_handleUpload(rexecArgs$(), nRexecArgs%)
  IF nRexecArgs% < 2 THEN
    rexec_usage
    EXIT SUB
  ENDIF

  LOCAL localFilePath$ = rexecArgs$(1)
  LOCAL remoteFilePath$
  
  'Use remote filepath if provide. 
  'Default to local filepath if not.
  IF nRexecArgs% >= 3 THEN
    remoteFilePath$ = rexecArgs$(2)
  ELSE
    remoteFilePath$ = localFilePath$
  ENDIF

  'Check if local file exists
  IF DIR$(localFilePath$) = "" THEN
    PRINT "File not found: "+localFilePath$
    rexec_usage
    EXIT SUB
  ENDIF
  
  rexec_upload localFilePath$, remoteFilePath$  
END SUB

SUB rexec_handleDownload(rexecArgs$(), nRexecArgs%)
  IF nRexecArgs% < 2 THEN
    rexec_usage
    EXIT SUB
  ENDIF

  LOCAL remoteFilePath$ = rexecArgs$(1)
  LOCAL localFilePath$

  'Use remote filepath if provide. 
  'Default to local filepath if not.  
  IF nRexecArgs% >= 3 THEN
    localFilePath$ = rexecArgs$(2)
  ELSE
    localFilePath$ = remoteFilePath$
  ENDIF
  
  rexec_download remoteFilePath$, localFilePath$
END SUB

SUB rexec_handleShell(rexecArgs$(), nRexecArgs%)
  IF nRexecArgs% < 2 THEN
    rexec_usage
    EXIT_SUB
  ENDIF

  'Combine all arguments into one string
  LOCAL shellCmd$=""
  LOCAL ii%
  
  FOR ii%=1 TO nRexecArgs%-1
    shellCmd$ = shellCmd$+" "+rexecArgs$(ii%)
  NEXT ii%
    
  rexec_shell shellCmd$
END SUB

SUB rexec_handleQuit(rexecArgs$(), nRexecArgs%)
  'Send quit command to server.
  PRINT
  PRINT REXEC_KEY$+QUIT_CMD$
  'PRINT "Done."
END SUB

SUB rexec_upload(localFilePath$, remoteFilePath$)
  rexec_return_code% = 0
  
  'Note that we also send the filesize so the receive can correct
  'the xmodem transmission for filesize.
  PRINT
  PRINT REXEC_KEY$+XMODEM_SEND_CMD$+" "+remoteFilePath$+" "+STR$(MM.INFO(FILESIZE localFilePath$))
  XMODEM SEND localFilePath$
  'PRINT "Done."
END SUB

SUB rexec_download(remoteFilePath$, localFilePath$)
  LOCAL l$

  rexec_return_code% = 0
  
  PRINT
  PRINT REXEC_KEY$+XMODEM_RECEIVE_CMD$+" "+remoteFilePath$

  LINE INPUT l$
  LOCAL filesize% = VAL(l$)

  'A received filesize value of -1 means remote file not found.
  IF filesize% = -1 THEN
    PRINT "Remote file not found: "+remoteFilePath$
    rexec_return_code% = 1
    EXIT SUB
  ENDIF
  
  XMODEM RECEIVE localFilePath$+".tmp"
  OPEN localFilePath$+".tmp" FOR INPUT AS #1
  OPEN localFilePath$ FOR OUTPUT AS #2

  'Correct received xmodem transmission for filesize.

  LOCAL chunkLen% = 0, inFileLoc% = 0
  LOCAL chunk$
 
  DO WHILE (NOT EOF(#1)) AND (inFileLoc% < filesize%)
    chunkLen% = MIN(filesize%-inFileLoc%, MAX_CHUNK_SIZE%)  
    inFileLoc% = inFileLoc% + chunkLen%
    chunk$ = INPUT$(chunkLen%, #1)
    PRINT #2, chunk$;
  LOOP

  CLOSE #2
  CLOSE #1

  KILL localFilePath$+".tmp"
  
  'PRINT "Done."
END SUB

SUB rexec_shell(cmd$)
  LOCAL l$

  rexec_return_code% = 0
  
  PRINT
  PRINT REXEC_KEY$+SHELL_CMD$+" "+cmd$
  PRINT    
  PRINT REXEC_KEY$+SHELL_RET$
  
  LINE INPUT l$
  rexec_return_code% = VAL(l$)
  
  DO WHILE l$ <> REXEC_DONE_KEY$
    LINE INPUT l$
  LOOP
  
  PRINT REXEC_KEY$+SHELL_OUT$
  
  l$=""
  DO WHILE l$ <> REXEC_DONE_KEY$
    LINE INPUT l$
  LOOP
  
  PRINT REXEC_KEY$+SHELL_ERR$

  l$=""
  DO WHILE l$ <> REXEC_DONE_KEY$
    LINE INPUT l$
  LOOP
  
  PRINT
  'PRINT "Done."
END SUB

SUB rexec_parseCmdLine(cmdLine$, cmdLineArgs$(), nArgs%)
  LOCAL curPos%=1, startPos%
  LOCAL inWhiteSpace%=1
  LOCAL curArg%=0
  
  DO WHILE (curPos%<=LEN(cmdLine$)) AND (curArg%<MAX_NUM_REXEC_ARGS%)
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
  
  IF (inWhiteSpace%=0) AND (curArg% < MAX_NUM_REXEC_ARGS%) THEN
    cmdLineArgs$(curArg%) = MID$(cmdLine$, startPos%)
    INC curArg%
  ENDIF
  
  nArgs% = curArg%
END SUB


SUB rexec_usage
  PRINT "REXEC client V"+REXEC_VERSION$+" by Epsilon."
  PRINT
  PRINT "rexec upload <local file path> [remote file path]"
  PRINT "rexec download <remote file path> [local file path]"
  PRINT "rexec shell <remote shell command>"
  PRINT "rexec quit: Request REXEC server to quit."
END SUB

