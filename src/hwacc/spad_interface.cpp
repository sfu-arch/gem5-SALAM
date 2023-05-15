#include "spad_interface.hh"

MemoryRequest *SpadInterface::SpadAlloc(size_t request_size) {
  size_t region_size = end_ - start_;
  std::cerr << "***************\n***************\nSpadAlloc "
               "Info\n***************\n***************\n";
  std::cerr << "region_size: " << region_size << ", req_size: " << request_size
            << ", limit: " << spad_limit_ << std::endl;
  MemoryRequest *req = nullptr;
  switch (phase_) {
    case FWD:
      if (region_size + request_size >= spad_limit_) {
        std::cerr << "Push: " << region_size << ", tape_ptr: " << tape_ptr_
                  << std::endl;
        req = new MemoryRequest(tape_ptr_, new char[region_size], region_size);
        push_history_.push_back(region_size);
        tape_ptr_ += region_size;
        end_ = start_;
      }
      head_ = end_;
      end_ += request_size;
      break;
    case REV:
      if (end_ <= start_) {
        if (push_history_.empty()) {
          std::cerr << "Error: push_history is empty" << std::endl;
        } else {
          size_t read_size = push_history_.back();
          std::cerr << "pop: " << read_size << ", tape_ptr: " << tape_ptr_
                    << std::endl;
          req = new MemoryRequest(
              tape_ptr_, std::max(region_size, static_cast<size_t>(8)));
          push_history_.pop_back();
          tape_ptr_ -= read_size;
          end_ = start_ + read_size;
        }
      }
      end_ -= request_size;
      head_ = std::max(end_, start_);
      break;
  }
  std::cerr << "after::: end: " << end_ << ", head: " << head_ << std::endl;
  return req;
}

MemoryRequest *SpadInterface::CreateReadRequest(
    std::shared_ptr<SALAM::Instruction> inst) const {
  gem5::Addr addr = head_ + inst->index * 8;
  return new MemoryRequest(addr, 8);  // 8 bytes
}

MemoryRequest *SpadInterface::CreateWriteRequest(
    std::shared_ptr<SALAM::Instruction> inst) const {
  gem5::Addr addr = head_ + inst->index * 8;
  return std::static_pointer_cast<SALAM::Store>(inst)->createSPMMemoryRequest(
      addr);
}