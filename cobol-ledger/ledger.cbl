       IDENTIFICATION DIVISION.
       PROGRAM-ID. LEDGER.

       DATA DIVISION.
       WORKING-STORAGE SECTION.
       01  INPUT-FUNCTION       PIC X(50).
       01  ARG1                 PIC X(50).
       01  ARG2                 PIC X(50).
       01  ARG3                 PIC X(50).

       01  MAX-ACCOUNTS         PIC 9(4) VALUE 1000.
       01  ACCOUNTS OCCURS 1000.
           02  ACCOUNT-ID       PIC X(32).
           02  BALANCE          PIC 9(10).

       01  I                    PIC 9(4).
       01  TRANSACTION-AMOUNT   PIC 9(10).
       01  WS-RETURN-CODE       PIC 9(4).

       PROCEDURE DIVISION.
       MAIN-PARA.
           * Use ARGUMENT-NUMBER to read command-line args directly.
           ACCEPT INPUT-FUNCTION FROM ARGUMENT-NUMBER 1
           ACCEPT ARG1           FROM ARGUMENT-NUMBER 2
           ACCEPT ARG2           FROM ARGUMENT-NUMBER 3
           ACCEPT ARG3           FROM ARGUMENT-NUMBER 4

           EVALUATE INPUT-FUNCTION
               WHEN "INIT"
                   PERFORM INIT-LEDGER
                   MOVE 0 TO WS-RETURN-CODE
               WHEN "GET_BAL"
                   PERFORM GET-BALANCE
               WHEN "APPLY_TX"
                   PERFORM APPLY-TRANSACTION
               WHEN OTHER
                   MOVE 99 TO WS-RETURN-CODE
           END-EVALUATE

           STOP RUN.

       INIT-LEDGER.
           PERFORM VARYING I FROM 1 BY 1
               UNTIL I > MAX-ACCOUNTS
               MOVE SPACES TO ACCOUNT-ID(I)
               MOVE 0      TO BALANCE(I)
           END-PERFORM
           DISPLAY "Ledger initialized."
           EXIT.

       GET-BALANCE.
           MOVE 0 TO WS-RETURN-CODE
           PERFORM VARYING I FROM 1 BY 1
               UNTIL I > MAX-ACCOUNTS
               IF ACCOUNT-ID(I) = ARG1
                   DISPLAY "BAL=" BALANCE(I)
                   EXIT PERFORM
               END-IF
           END-PERFORM
           EXIT.

       APPLY-TRANSACTION.
           MOVE FUNCTION NUMVAL(ARG3) TO TRANSACTION-AMOUNT
           IF TRANSACTION-AMOUNT <= 0
               MOVE 1 TO WS-RETURN-CODE
               DISPLAY "ERROR: Invalid amount"
               EXIT PARAGRAPH
           END-IF

           MOVE 0 TO WS-RETURN-CODE
           PERFORM VARYING I FROM 1 BY 1
               UNTIL I > MAX-ACCOUNTS
               IF ACCOUNT-ID(I) = ARG1
                   IF BALANCE(I) >= TRANSACTION-AMOUNT
                       SUBTRACT TRANSACTION-AMOUNT FROM BALANCE(I)
                   ELSE
                       MOVE 2 TO WS-RETURN-CODE
                       DISPLAY "ERROR: Insufficient funds"
                       EXIT PERFORM
                   END-IF
               END-IF
           END-PERFORM

           IF WS-RETURN-CODE NOT = 0
               EXIT PARAGRAPH
           END-IF

           PERFORM VARYING I FROM 1 BY 1
               UNTIL I > MAX-ACCOUNTS
               IF ACCOUNT-ID(I) = ARG2
                   ADD TRANSACTION-AMOUNT TO BALANCE(I)
                   EXIT PERFORM
               END-IF
               IF ACCOUNT-ID(I) = SPACES
                   MOVE ARG2 TO ACCOUNT-ID(I)
                   MOVE TRANSACTION-AMOUNT TO BALANCE(I)
                   EXIT PERFORM
               END-IF
           END-PERFORM

           DISPLAY "Transaction applied: " ARG1
               " -> " ARG2
               " : " ARG3
           EXIT.
