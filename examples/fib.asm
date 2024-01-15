# Fibonacci up to 255

initialize:
    ldi A 1    # Load 1 to register A
    ldi B 1    # Load 1 to register B
    
loop:
    sta A 0    # Store register A into RAM address 0 
    add A      # Add A + B and store result in register A
    bc done    # if A + B > 255, branch to done
    lda B 0    # Load the previous value of A from RAM at address 0 into register B
    b loop     # Repeat the calculation

done: 
    lda A 0    # Recover register A from RAM - the highest Fibonacci number less than 255
    b done     # Endlessly loop