// Simple loopback nanoservice
entry:
  // First register context ID with NIC
  csrwi	lcurcontext, 0
  csrwi	lcurpriority, 0
  csrwi	lniccmd, 1

// Wait for a message to arrive
wait_msg:
  csrr	a5, lmsgsrdy
  bnez	a5, loopback_plus1_16B
idle:
  csrwi	lidle, 1
  csrr	a5, lmsgsrdy
  beqz	a5, idle

// Loopback and increment 16B message
loopback_plus1_16B:
  // Copy application header from head to tail
  mv t6, t5
  // Increment first data word
  addi t6, t5, 1
  // Increment second data word
  addi t6, t5, 1
  // Wait for the next message
  j wait_msg
