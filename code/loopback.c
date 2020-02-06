
#define register_context(contextID, priority) { \
  write_csr(lcurcontext, contextID); \
  write_csr(lcurpriority, priority); \
  write_csr(lniccmd, 1); \
}

#define lnic_wait() while (read_csr(0x052) == 0) { write_csr(0x056, 1); }

#define lnic_read() ({ uint64_t __tmp; \
  asm volatile ("mv %0, "LREAD  : "=r"(__tmp)); \
  __tmp; })
#define lnic_copy() asm volatile ("mv "LWRITE", "LREAD)
#define lnic_write(val) asm volatile ("mv "LWRITE", %0" : /*no outputs*/ : "r"(val))

void loopback_plus1_16B() {
  while (1) {
    // wait for a pkt to arrive
    lnic_wait();
    // read request application hdr
    app_hdr = lnic_read();
    // write response application hdr
    lnic_write(app_hdr);
    // extract msg_len
    msg_len = (uint16_t)app_hdr;
    num_words = msg_len/LNIC_WORD_SIZE;
    if (msg_len % LNIC_WORD_SIZE != 0) { num_words++; }
    // copy msg words back into network
    for (i = 0; i < num_words; i++) {
      lnic_copy();
    }
}
}