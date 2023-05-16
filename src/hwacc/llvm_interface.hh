#ifndef __HWACC_LLVM_INTERFACE_HH__
#define __HWACC_LLVM_INTERFACE_HH__

// C++ Includes
#include <algorithm>
#include <chrono>
#include <ctime>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <ratio>
#include <type_traits>
#include <typeinfo>

// LLVM Includes
#include <llvm-c/Core.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Dominators.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Transforms/Utils/Cloning.h>

// SALAM Includes
#include "bin_hierarchy.hh"
#include "hwacc/HWModeling/src/hw_interface.hh"
#include "hwacc/LLVMRead/src/basic_block.hh"
#include "hwacc/LLVMRead/src/debug_flags.hh"
#include "hwacc/LLVMRead/src/function.hh"
#include "hwacc/LLVMRead/src/operand.hh"
#include "hwacc/compute_unit.hh"
#include "params/LLVMInterface.hh"
#include "spad_interface.hh"

class LLVMInterface : public ComputeUnit {
private:
  std::string filename;
  std::string topName;
  uint32_t scheduling_threshold;
  int32_t clock_period;
  int cycle;
  int stalls;
  int waitCycles = 0;
  bool running;
  bool loadOpScheduled;
  bool storeOpScheduled;
  bool compOpScheduled;
  bool lockstep;
  bool dbg;
  bool hardwareEnabled = false;
  std::map<std::string, uint32_t> cost_map;
  std::map<std::string, uint64_t> value_map;

  std::chrono::duration<float> setupTime;
  std::chrono::duration<float> simTotal;
  std::chrono::duration<float> simTime;
  std::chrono::duration<float> schedulingTime;
  std::chrono::duration<float> queueProcessTime;
  std::chrono::duration<float> computeTime;
  std::chrono::duration<float> hwTime;
  std::chrono::high_resolution_clock::time_point simStop;
  std::chrono::high_resolution_clock::time_point setupStop;
  std::chrono::high_resolution_clock::time_point timeStart;
  public:
  class ActiveFunction {
    friend class LLVMInterface;

  private:
    LLVMInterface *owner;
    HWInterface *hw;
    std::shared_ptr<SALAM::Function> func;
    std::shared_ptr<SALAM::Instruction> caller;
    std::list<std::shared_ptr<SALAM::Instruction>> reservation;
    std::map<uint64_t, std::shared_ptr<SALAM::Instruction>> readQueue;
    std::map<MemoryRequest *, uint64_t> readQueueMap;
    std::map<uint64_t, std::shared_ptr<SALAM::Instruction>> writeQueue;
    std::map<MemoryRequest *, uint64_t> writeQueueMap;
    std::map<uint64_t, std::shared_ptr<SALAM::Instruction>> computeQueue;
    std::shared_ptr<SALAM::BasicBlock> previousBB;
    HW_Cycle_Stats hw_cycle_stats;
    uint32_t scheduling_threshold;
    bool returned = false;
    bool lockstep;
    bool dbg;
    bool found_invert = false;
    inline bool uidActive(uint64_t id) {
      return computeUIDActive(id) || readUIDActive(id) || writeUIDActive(id);
    }

    std::map<Addr, std::shared_ptr<SALAM::Instruction>> activeWrites;
    std::map<Addr, Tick> activeWriteCycle;

    inline void trackWrite(Addr writeAddr,
                           std::shared_ptr<SALAM::Instruction> writeInst) {
      activeWrites.insert({writeAddr, writeInst});
      activeWriteCycle.insert({writeAddr, owner->cycle});
    }
    inline void untrackWrite(uint64_t writeAddr) {
      auto it = activeWrites.find(writeAddr);
      if (it != activeWrites.end() && it->second->is_write) {
        owner->waitCycles += owner->cycle - activeWriteCycle.find(writeAddr)->second;
        activeWriteCycle.erase(writeAddr);
      }
      if (it != activeWrites.end())
        activeWrites.erase(it);
    }
    inline bool writeActive(uint64_t writeAddr) {
      return (activeWrites.find(writeAddr) != activeWrites.end());
    }

    inline std::shared_ptr<SALAM::Instruction>
    getActiveWrite(uint64_t writeAddr) {
      return activeWrites.find(writeAddr)->second;
    }
    // std::map<Addr, std::shared_ptr<SALAM::Instruction>> activeReads;
    // inline void trackRead(Addr readAddr, std::shared_ptr<SALAM::Instruction>
    // readInst) {
    //   activeReads.insert({readAddr, readInst});
    // }
    // inline void untrackRead(uint64_t readAddr) {
    //   auto it = activeReads.find(readAddr);
    //   if (it != activeReads.end()) activeReads.erase(it);
    // }
    // inline bool readActive(uint64_t readAddr) {
    //   return (activeReads.find(readAddr) != activeReads.end());
    // }
    // inline std::shared_ptr<SALAM::Instruction> getActiveRead(uint64_t
    // readAddr) {
    //   return activeReads.find(readAddr)->second;
    // }
    inline bool writeUIDActive(uint64_t uid) {
      return (writeQueue.find(uid) != writeQueue.end());
    }
    inline bool readUIDActive(uint64_t uid) {
      return (readQueue.find(uid) != readQueue.end());
    }
    inline bool computeUIDActive(uint64_t uid) {
      return (computeQueue.find(uid) != computeQueue.end());
    }

  public:
    ActiveFunction(LLVMInterface *_owner,
                   std::shared_ptr<SALAM::Function> _func,
                   std::shared_ptr<SALAM::Instruction> _caller)
        : owner(_owner), func(_func), caller(_caller), previousBB(nullptr) {
      scheduling_threshold = owner->getSchedulingThreshold();
      lockstep = (owner->getLockstepStatus());
      dbg = owner->debug();
    }

    // ADDED BY ME
    bin::FixedQueue<bin::SyncInfo, 2> sync_queue_;
    void handlePushPopDependency(std::shared_ptr<SALAM::Instruction> inst);
    void handleTloadPopDependency(std::shared_ptr<SALAM::Instruction> inst);

    void handlePop(SALAM::Instruction *inst);
    void handlePush(SALAM::Instruction *inst);
    void handleSPADAlloc(SALAM::Instruction *inst);
    
    void handleMallocCall(int size);
    bool isMalloc(SALAM::Value *val) {
      return val->getIRStub().find("malloc") != std::string::npos;
    }
    bool isRealloc(SALAM::Value *val) {
      return val->getIRStub().find("realloc") != std::string::npos;
    }
    void readCommit(MemoryRequest *req);
    void writeCommit(MemoryRequest *req);
    void findDynamicDeps(std::shared_ptr<SALAM::Instruction> inst);
    void scheduleBB(std::shared_ptr<SALAM::BasicBlock> bb);
    void processQueues();
    void launch();
    inline bool queuesClear() {
      return readQueue.empty() && writeQueue.empty() && computeQueue.empty();
    }
    inline bool lockstepReady() { return !lockstep || queuesClear(); }
    inline bool canReturn() {
      return queuesClear() && reservation.front()->isReturn();
    }

    // Sends a non-blocking prefetch request to the memory system based on the size of the input barrier instruction.
    // Prefetch only happens in REV when encountering a "pop" barrier.
    // Example usage:
    // SALAM::BarrierInstruction *barrier = new SALAM::BarrierInstruction();
    // launchBarrier(barrier);
    void launchBarrier(std::shared_ptr<SALAM::Instruction> inst);

    void launchRead(std::shared_ptr<SALAM::Instruction> readInst);

    void launchWrite(std::shared_ptr<SALAM::Instruction> writeInst);
    
    bool hasReturned() { return returned; }
  };

  std::list<ActiveFunction> activeFunctions;
  std::map<MemoryRequest *, ActiveFunction *> globalReadQueue;
  std::map<MemoryRequest *, ActiveFunction *> globalWriteQueue;

  std::vector<std::shared_ptr<SALAM::Function>> functions;
  std::vector<std::shared_ptr<SALAM::Value>> values;

protected:
  // const std::string name() const { return comm->getName() + ".compute"; }
  virtual bool debug() { return comm->debug(); }
  // virtual bool debug() { return true; }
public:
  // ADDED BY ME
  SpadInterface *spad_;
  bool binning = false;
  bin::BinHierarchy *bin_hierarchy = nullptr;
  std::map<std::string, std::vector<uint64_t>> address_map;
  void readAddressMap();
  int getCycle() { return cycle; }

  // Launches a DMA transfer from src to dst. The `dma.h` header must be included in the 
  // accelerator's source code.
  void launchDMAFunction(gem5::Addr dst, gem5::Addr src, size_t byte_size);

  PARAMS(LLVMInterface);
  LLVMInterface(const LLVMInterfaceParams &p);
  void tick();
  void constructStaticGraph();
  void startup();
  void initialize();
  void finalize();
  void debug(uint64_t flags);
  bool getLockstepStatus() { return lockstep; }
  void readCommit(MemoryRequest *req);
  void writeCommit(MemoryRequest *req);
  void dumpModule(llvm::Module *m);
  void printResults();
  void launchFunction(std::shared_ptr<SALAM::Function> callee,
                      std::shared_ptr<SALAM::Instruction> caller);
  void launchTopFunction();
  void endFunction(ActiveFunction *afunc);
  void launchRead(MemoryRequest *memReq);
  void launchRead(MemoryRequest *memReq, ActiveFunction *func);
  
  void launchWrite(MemoryRequest *memReq);
  void launchWrite(MemoryRequest *memReq, ActiveFunction *func);
  std::shared_ptr<SALAM::Instruction> createInstruction(llvm::Instruction *inst,
                                                        uint64_t id);
  void dumpQueues();
  uint32_t getSchedulingThreshold() { return scheduling_threshold; }
  void addSchedulingTime(std::chrono::duration<float> timeDelta) {
    schedulingTime = schedulingTime + timeDelta;
  }
  void addQueueTime(std::chrono::duration<float> timeDelta) {
    queueProcessTime = queueProcessTime + timeDelta;
  }
  void addComputeTime(std::chrono::duration<float> timeDelta) {
    computeTime = computeTime + timeDelta;
  }
  void addHWTime(std::chrono::duration<float> timeDelta) {
    hwTime = hwTime + timeDelta;
  }
};

#endif //__HWACC_LLVM_INTERFACE_HH__
