#ifndef SPAD_INTERFACE_HH
#define SPAD_INTERFACE_HH

#include <vector>

#include "base/types.hh"
#include "hwacc/LLVMRead/src/instruction.hh"
#include "hwacc/LLVMRead/src/mem_request.hh"
#include "hwacc/comm_interface.hh"

class LLVMInterface;

enum Phase { FWD, REV };

// SpadInterface is the interface between compute and scratchpad.
// It issues tape read and write requests to the scratchpad and
// makes sure that the scratchpad is not overflown.
// Example usage:
class SpadInterface {
 public:
  SpadInterface(gem5::Addr start, size_t spad_limit_, gem5::Addr tape_ptr)
      : start_(start),
        end_(start),
        head_(start),
        spad_limit_(spad_limit_),
        tape_ptr_(tape_ptr),
        phase_(FWD) {}

  // Allocates a buffer in the scratchpad with the requested size.
  // Returns a memory request if spill is required.
  MemoryRequest *SpadAlloc(size_t request_size);

  MemoryRequest *CreateReadRequest(
      std::shared_ptr<SALAM::Instruction> inst) const;

  MemoryRequest *CreateWriteRequest(
      std::shared_ptr<SALAM::Instruction> inst) const;

  void SetPhase(Phase phase) { phase_ = phase; }

 private:
  gem5::Addr start_;
  gem5::Addr end_;
  gem5::Addr head_;
  size_t spad_limit_;
  gem5::Addr tape_ptr_;
  Phase phase_;
  std::vector<gem5::Addr> push_history_;
};

#endif  // SPAD_INTERFACE_HH