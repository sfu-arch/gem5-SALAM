// LLVMInterface Includes
#include "hwacc/llvm_interface.hh"

#include <iostream>
#include <fstream>

uint64_t last_id = 0;
LLVMInterface::LLVMInterface(const LLVMInterfaceParams &p):
    ComputeUnit(p),
    filename(p.in_file),
    topName(p.top_name),
    scheduling_threshold(p.sched_threshold),
    clock_period(p.clock_period),
    lockstep(p.lockstep_mode) {
  
    int bin_count = 0;
    std::vector<int> nums;
    std::string my_text;
    std::ifstream MyReadFile("/localhome/mha157/gem5-SALAM/src/hwacc/bin_config.txt");

    std::string delimiter = ",";
    size_t pos = 0;

    while (getline (MyReadFile, my_text)) {
        std::cerr << "########### TEXT #########\n" << my_text << std::endl;
        while ((pos = my_text.find(delimiter)) != std::string::npos) {
            std::string token = my_text.substr(0, my_text.find(delimiter));
            std::cerr << "########### TOKEN #########\n" << token << std::endl;
            nums.push_back(std::stoi(token));
            my_text.erase(0, pos + delimiter.length());
        }
    }
    std::cerr << "########### After TEXT #########\n" << my_text << std::endl;

    MyReadFile.close();
    bin_count = nums[0];
    std::cerr << "BIN COUNT " << bin_count << std::endl;
    if (bin_count != 0) {
        std::cerr << "Creating bins " << std::endl;

        nums.erase(nums.begin());
        bin_hierarchy = new BinHierarchy(bin_count, nums);
    } else {
        bin_hierarchy = nullptr;
    }
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    clock_period = clock_period * 1000;

}

std::shared_ptr<SALAM::Value> createClone(const std::shared_ptr<SALAM::Value>& b)
{
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    std::shared_ptr<SALAM::Value> clone = b->clone();
    return clone;
}

void
LLVMInterface::ActiveFunction::scheduleBB(std::shared_ptr<SALAM::BasicBlock> bb)
{
    // std::cerr << "Handling BB: " << bb->getIRStub() << std::endl;
    if (bb->getIRString().find("invert") != std::string::npos) {
        std::cerr << "######### Found invert #########" << std::endl;
        found_invert = true;
        std::cerr << "Cycle count invert = " << std::dec << owner->getCycle() << std::endl;
    }
    auto schedulingStart = std::chrono::high_resolution_clock::now();
    DPRINTFR(Runtime,"|---[Schedule BB - UID:%i ]\n", bb->getUID());
    bool needToScheduleBranch = false;
    std::shared_ptr<SALAM::BasicBlock> nextBB;
    auto instruction_list = *(bb->Instructions());
    for (auto inst : instruction_list) {
        std::shared_ptr<SALAM::Instruction> clone_inst = inst->clone();

        DPRINTFR(Runtime, "\t\t Instruction Cloned [UID: %d] \n", inst->getUID());
        if (clone_inst->isBr()) {
            DPRINTFR(Runtime, "\t\t Branch Instruction Found\n");
            auto branch = std::dynamic_pointer_cast<SALAM::Br>(clone_inst);
            if (branch && !(branch->isConditional())) {
                DPRINTFR(Runtime, "\t\t Unconditional Branch, Scheduling Next BB\n");
                nextBB = branch->getTarget();
                DPRINTFR(RuntimeCompute, "\t\t Branching to %s from %s\n", nextBB->getIRStub(), bb->getIRStub());
                needToScheduleBranch = true;
            } else {
                // std::cerr << "Finding dynamic depth 1 ..." << std::endl;
                findDynamicDeps(clone_inst);
                reservation.push_back(clone_inst);
            }
        } else {
            if (clone_inst->isPhi()) {
                DPRINTFR(Runtime, "\t\t Phi Instruction Found\n");
                auto phi = std::dynamic_pointer_cast<SALAM::Phi>(clone_inst);
                if (phi) phi->setPrevBB(previousBB);
            }
            // std::cerr << "Finding dynamic depth 2 ..." << std::endl;

            findDynamicDeps(clone_inst);
            reservation.push_back(clone_inst);

        }
    }
    previousBB = bb;
    auto schedulingStop = std::chrono::high_resolution_clock::now();
    owner->addSchedulingTime(schedulingStop - schedulingStart);
    if (needToScheduleBranch) scheduleBB(nextBB);
}

uint64_t base_address = (uint64_t)0x80cf4240;
uint64_t createMallocBaseAddress(int size)
{
    // std::cerr << "returning base address: " << base_address << std::endl;
    uint64_t startAddr = base_address;
    base_address += size;
    return startAddr;
}

void
LLVMInterface::ActiveFunction::handleMallocCall(int size)
{
    if (caller->getSize() > 0) {
        SALAM::Value callee_return_val(func->getUID());
        callee_return_val.returnReg = std::make_shared<SALAM::PointerRegister>();
        
        const uint64_t addr = createMallocBaseAddress(size);
        callee_return_val.setRegisterValue(addr);
        // auto retOperand = SALAM::Operand(callee_return_val);
        // std::cerr << "callee_return_val: " << callee_return_val.getPtrRegValue()  << func->getUID() << std::endl;
        caller->setRegisterValue(callee_return_val.getPtrRegValue());
        // std::cerr << "caller: " << caller->getPtrRegValue() <<   caller->getUID() << std::endl;
    }
    func->removeInstance();
    // caller->commit();
    returned = true;
}

void handleInstLog(SALAM::Instruction *inst, int id)
{
    return;
    // if (inst)
    //     std::cerr << " ID: " << std::dec << inst->getUID() << " : " << id << "Handling reservation: " << inst->getIRString() << std::endl;
    // else 
    //     std::cerr << "Inst is null" << std::endl;
}

std::string split(std::string &line, std::string delim) {
    std::string token = line.substr(0, line.find(delim));
    line = line.substr(line.find(delim) + 1);
    return token;
}

void LLVMInterface::readAddressMap() {
    std::string line;
    std::ifstream myfile ("/localhome/mha157/gem5-SALAM/addr.txt");
    std::string::size_type sz;   // alias of size_t


    if (myfile.is_open()) {
        while ( getline (myfile, line)) {
            auto token = split(line, ", ");
            // std::cerr << token << " - " << line << std::endl;
            if (!LLVMInterface::address_map.count(token)) {
                LLVMInterface::address_map[token] = std::vector<uint64_t>((uint64_t)std::atol(line.c_str()));
            } else {
                LLVMInterface::address_map[token].push_back((uint64_t) std::stoul(line));
            }
        }
        myfile.close();
    }
}

void LLVMInterface::ActiveFunction::handlePop(SALAM::Instruction *inst) {
    std::cerr << "Launching pop " << inst->getIRStub() << " count: " << inst->push_pop_count << "\n";
    owner->bin_hierarchy->pop(inst->push_pop_count);
    std::cerr << "CycleCounts: " << owner->getCycle() << std::endl;
    // if (!addrs.empty()) {
    //     for (auto addr : addrs) {
    //         MemoryRequest * mem_req = new MemoryRequest(addr, 8);
    //         SALAM::Load* li = new SALAM::Load(last_id+1, llvm::Instruction::Load, 0);
    //         std::shared_ptr<SALAM::Instruction> loadInst = li->clone();

    //         last_id++;
    //         readQueue.insert({loadInst->getUID(), (loadInst)});
    //         readQueueMap.insert({mem_req, loadInst->getUID()});
    //         owner->launchRead(mem_req, this);
    //     }
    // }
}

void
LLVMInterface::ActiveFunction::handlePushPopDependency(std::shared_ptr<SALAM::Instruction> inst) {
    auto queue_iter = reservation.rbegin();
    while ((queue_iter != reservation.rend())) {
        auto queued_inst = *queue_iter;
        // Look at each instruction in runtime queue once
        if (queued_inst->is_push_req || queued_inst->is_pop_req || queued_inst->is_read || queued_inst->is_write) {
            inst->addRuntimeDependency(queued_inst);
            queued_inst->addRuntimeUser(inst);
            // std::cerr << "Adding push-pop dependency: " << inst->getIRString() << " -> " << queued_inst->getIRString() << std::endl;
            // break;
        }
        queue_iter++;
    }
}
void
LLVMInterface::ActiveFunction::handleReadWriteDependency(std::shared_ptr<SALAM::Instruction> inst) {
    auto queue_iter = reservation.rbegin();
    while ((queue_iter != reservation.rend())) {
        auto queued_inst = *queue_iter;
        // Look at each instruction in runtime queue once
        if (queued_inst->is_push_req || queued_inst->is_pop_req) {
            if (std::next(queue_iter) != reservation.rend()) {
                auto next_inst = *(std::next(queue_iter));
                inst->addRuntimeDependency(next_inst);
                next_inst->addRuntimeUser(inst);
            } else if (std::prev(queue_iter) != reservation.rbegin()) {
                auto prev_inst = *(std::prev(queue_iter));
                inst->addRuntimeDependency(prev_inst);
                prev_inst->addRuntimeUser(inst);
            }
            inst->addRuntimeDependency(queued_inst);
            queued_inst->addRuntimeUser(inst);
            // std::cerr << "Adding read-write dependency: " << inst->getIRString() << " -> " << queued_inst->getIRString() << std::endl;
            break;
        }
        queue_iter++;
    }
}


void
LLVMInterface::ActiveFunction::processQueues()
{
    // std::cerr << "Processing reservation queue " << func->getIRStub() << std::endl;
    auto queueStart = std::chrono::high_resolution_clock::now();
    DPRINTFR(Runtime,"\t\t  |-[Process Queues]--------\n");
    // ccprintf(std::cerr, "\t\t[Runtime Queue Status] Reservation:%d, Compute:%d, Read:%d, Write:%d\n",
    //          reservation.size(), computeQueue.size(), readQueue.size(), writeQueue.size());

    // First pass, computeQueue is empty
    for (auto queue_iter = computeQueue.begin(); queue_iter != computeQueue.end();) {
        DPRINTFR(Runtime, "\n\t\t %s \n\t\t %s%s%s%d%s \n",
        " |-[Compute Queue]--------------",
        " | Instruction: ", llvm::Instruction::getOpcodeName((queue_iter->second)->getOpode()),
        " | UID[", (queue_iter->first), "]"
        );
        handleInstLog(queue_iter->second.get(), 0);

        if ((queue_iter->second)->commit()) {
            handleInstLog(queue_iter->second.get(), 1);
            (queue_iter->second)->reset();
            queue_iter = computeQueue.erase(queue_iter);
        } else ++queue_iter;
        // std::cerr << "Compute Queue: " << reservation.size() << std::endl;
    }
    if (isMalloc(func.get())) {

        auto alloc_size = func->getArguments()->at(0).get()->getIntRegValue();
        // std::cerr << "Processing Malloc " << func->getIRStub() << " alloc size: " << alloc_size << std::endl;
        handleMallocCall(alloc_size);
        // func->removeInstance();
        caller->commit();
        // return;
    } else if (isRealloc(func.get())) {
        auto alloc_size = func->getArguments()->at(1).get()->getIntRegValue();
        // std::cerr << "Processing Realloc " << func->getIRStub() << " Realloc size: " << alloc_size << std::endl;
        handleMallocCall(alloc_size);
        // func->removeInstance();
        caller->commit();
    } else if (canReturn()) {
        // Handle function return
        DPRINTFR(Runtime, "[[Function Return]]\n\n");
        if (caller != nullptr) {
            // Signal the calling instruction
            if (caller->getSize() > 0) {
                // std::cerr << "Returning from function: " << func->getIRStub() << std::endl;
                auto retInst = reservation.front();
                auto retOperand = retInst->getOperands()->front();
                caller->setRegisterValue(retOperand.getOpRegister());
            }
            func->removeInstance();
            caller->commit();
        }
        returned = true;
        return;
    } else if (lockstepReady()) {
        // TODO: Look into for_each here
        for (auto queue_iter = reservation.begin(); queue_iter != reservation.end();) {
            // std::cerr << "lockstepReady: " << reservation.size() << std::endl;

            if (owner->debug())
                DPRINTFR(Runtime, "Debug Breakpoint");
            auto inst = *queue_iter;
            if (owner->bin_hierarchy != nullptr) {
                // Making sure that previous pushes are complete
                if (inst->is_push_req) {
                    std::cerr << "Launching push " << inst->getIRStub() << "count: " << inst->push_pop_count << "\n";
                    uint32_t count = inst->push_pop_count;
                    owner->bin_hierarchy->push(count);
                    queue_iter = reservation.erase(queue_iter);
                    continue;
                } else if (inst->is_pop_req) {
                    handlePop(inst.get());
                    queue_iter = reservation.erase(queue_iter);
                    continue;
                } else if (inst->is_read) {
                    std::cerr << "Launching read " << inst->getIRStub() << "\n";
                    inst->commit();
                    queue_iter = reservation.erase(queue_iter);
                    continue;
                } else if (inst->isStore() && inst->is_write) {
                    std::cerr << "Launching write " << inst->getIRString() << "\n";
                    inst->commit();
                    queue_iter = reservation.erase(queue_iter);
                    continue;
                } 
            } 
       

            DPRINTFR(Runtime, "\n\t\t %s \n\t\t %s%s%s%d%s \n",
                " |-[Reserve Queue]--------------",
                " | Instruction: ", llvm::Instruction::getOpcodeName((inst)->getOpode()),
                " | UID[", (inst)->getUID(), "]"
                );
            if ((inst)->isReturn() == false) {
                if ((inst)->isTerminator() && reservation.size() >= scheduling_threshold) {
                    handleInstLog(queue_iter->get(), 5);

                    ++queue_iter;
                } else if (((inst)->ready()) && !uidActive((inst)->getUID())) {
                    if ((inst)->isLoad()) {

                        // if (inst->getIRString().find('\'') != std::string::npos) {
                        //     std::cerr << "Skipping load " << inst->getIRStub() << "\n";
                        //     inst->commit();
                        //     queue_iter = reservation.erase(queue_iter);
                        //     continue;
                        // }
                    handleInstLog(queue_iter->get(), 6);

                        // RAW protection to ensure a writeback finishes before reading that location
                        if (inst->isLoadingInternal()) {
            handleInstLog(queue_iter->get(), 7);

                            launchRead(inst);
                            DPRINTFR(Runtime, "\t\t  |-Erase From Queue: %s - UID[%i]\n", llvm::Instruction::getOpcodeName((*queue_iter)->getOpode()), (*queue_iter)->getUID());
                            queue_iter = reservation.erase(queue_iter);
                        } else if (!writeActive(inst->getPtrOperandValue(0))) {
            handleInstLog(queue_iter->get(), 8);
                            launchRead(inst);
                            DPRINTFR(Runtime, "\t\t  |-Erase From Queue: %s - UID[%i]\n", llvm::Instruction::getOpcodeName((*queue_iter)->getOpode()), (*queue_iter)->getUID());
                            queue_iter = reservation.erase(queue_iter);
                        } else {
            handleInstLog(queue_iter->get(), 9);

                            auto activeWrite = getActiveWrite(inst->getPtrOperandValue(0));
                            inst->addRuntimeDependency(activeWrite);
                            activeWrite->addRuntimeUser(inst);
                            ++queue_iter;

                        }
                    } else if ((inst)->isStore()) {
            handleInstLog(queue_iter->get(), 10);

                        // queue_iter = reservation.erase(queue_iter);
                        // WAR Protection to insure reading finishes before a write
                        // if (!readActive(inst->getPtrOperandValue(1))) {
                launchWrite(inst);
                queue_iter = reservation.erase(queue_iter);
                        // } else {
                        //     auto activeRead = getActiveRead(inst->getPtrOperandValue(1));
                        //     inst->addRuntimeDependency(activeRead);
                        //     activeRead->addRuntimeUser(inst);
                        //     ++queue_iter;
                        // }
                    } else if ((inst)->isLatchingBrExiting() && ((reservation.size() > 1) || !queuesClear())) {
                        ++queue_iter;
            handleInstLog(queue_iter->get(), 11);

                    } else if ((inst)->isTerminator()) {
            handleInstLog(queue_iter->get(), 12);
                        (inst)->launch();
                        auto nextBB = inst->getTarget();
                        DPRINTFR(RuntimeCompute, "\t\t Branching to %s from %s\n",
                        nextBB->getIRStub(), previousBB->getIRStub());
                        scheduleBB(nextBB);
                        DPRINTFR(Runtime, "\t\t  | Branch Scheduled: %s - UID[%i]\n", llvm::Instruction::getOpcodeName((inst)->getOpode()), (inst)->getUID());
                        (inst)->commit();
                        DPRINTFR(Runtime, "\t\t  |-Erase From Queue: %s - UID[%i]\n", llvm::Instruction::getOpcodeName((*queue_iter)->getOpode()), (*queue_iter)->getUID());
                        queue_iter = reservation.erase(queue_iter);
                    } else if ((*queue_iter)->isCall()) {
            handleInstLog(queue_iter->get(), 13);

                        auto callInst = std::dynamic_pointer_cast<SALAM::Call>(inst);
                        // std::cerr << "Caller: " << inst->getIRStub() << std::endl;

                        assert(callInst);
                        auto calleeValue = callInst->getCalleeValue();
                        // std::cerr << "Callee: " << calleeValue->getIRStub() << std::endl;
                        auto const_callee = std::dynamic_pointer_cast<SALAM::Constant>(calleeValue);

                        auto callee = std::dynamic_pointer_cast<SALAM::Function>(calleeValue);
                        if (const_callee) {
                            // std::cerr << "Can't launch" << std::endl;

                            computeQueue.insert({(inst)->getUID(), inst});
                            DPRINTFR(Runtime, "\t\t  |-Erase From Queue: %s - UID[%i]\n", llvm::Instruction::getOpcodeName((*queue_iter)->getOpode()), (*queue_iter)->getUID());
                            queue_iter = reservation.erase(queue_iter);
                        }
                        // assert(callee);
                        else if (callee && callee->canLaunch()) {
            handleInstLog(queue_iter->get(), 14);

                            // std::cerr << "Can launch" << std::endl;
                            owner->launchFunction(callee, callInst);
                            computeQueue.insert({(inst)->getUID(), inst});
                            // DPRINTFR(Runtime, "\t\t  |-Erase From Queue: %s - UID[%i]\n", llvm::Instruction::getOpcodeName((*queue_iter)->getOpode()), (*queue_iter)->getUID());
                            queue_iter = reservation.erase(queue_iter);
                            // queue_iter++;
                            // std::cerr << "Launch done" << std::endl;
                        } else {
            handleInstLog(queue_iter->get(), 15);

                            ++queue_iter;
                        }
                    } else {
                        auto computeStart = std::chrono::high_resolution_clock::now();
                        if (!(inst)->launch()) {
            handleInstLog(queue_iter->get(), 16);

                            DPRINTFR(Runtime, "\t\t  | Added to Compute Queue: %s - UID[%i]\n", llvm::Instruction::getOpcodeName((inst)->getOpode()), (inst)->getUID());
                            computeQueue.insert({(inst)->getUID(), inst});
                        } else {
            handleInstLog(queue_iter->get(), 17);


                        }
                        auto computeStop = std::chrono::high_resolution_clock::now();
                        owner->addComputeTime(computeStop-computeStart);
                        DPRINTFR(Runtime, "\t\t  |-Erase From Queue: %s - UID[%i]\n", llvm::Instruction::getOpcodeName((*queue_iter)->getOpode()), (*queue_iter)->getUID());
                        queue_iter = reservation.erase(queue_iter);

                    }
                } else {
            handleInstLog(queue_iter->get(), 18);
                    ++queue_iter;
                }
            } else {
            handleInstLog(queue_iter->get(), 19);

                ++queue_iter;
            }
        }
    }
    auto queueStop = std::chrono::high_resolution_clock::now();
    owner->addQueueTime(queueStop-queueStart);

}



/*********************************************************************************************
 CN Scheduling

 As CNs are scheduled they are added to an in-flight queue depending on operation type.
 Loads and Stores are maintained in separate queues, and are committed by the comm_interface.
 Branch and phi instructions evaluate and commit immediately. All other CN types are added to
 an in-flight compute queue.

 Each tick we must first check our in-flight compute queue. Each node should have its cycle
 count incremented, and should commit if max cycle is reached.

 New CNs are added to the reservation table whenever a new BB is encountered. This may occur
 during device init, or when a br op commits. For each CN in a BB we reset the CN, evaluate
 if it is a phi or uncond br, and add it to our reservation table otherwise.
*********************************************************************************************/
void
LLVMInterface::tick()
{
    auto tickStart = std::chrono::high_resolution_clock::now();

    if(debug()) DPRINTF(LLVMInterface, "\n%s\n%s %d\n%s\n",
        "********************************************************************************",
        "   Cycle", cycle,
        "********************************************************************************");
    cycle++;

    // Process Queues in Active Functions
    for (auto func_iter = activeFunctions.begin(); func_iter != activeFunctions.end();) {
        func_iter->processQueues();
        if (!(func_iter->hasReturned())) {
            func_iter++;
        } else {
            func_iter = activeFunctions.erase(func_iter);
        }
    }
    if (activeFunctions.empty()) {
        // We are finished executing all functions. Signal completion to the CommInterface
        running = false;
        std::cerr << "Cycle count end = " << std::dec << getCycle() << std::endl;
        finalize();
        return;
    }
    //////////////// Schedule Next Cycle ////////////////////////
    if (running && !tickEvent.scheduled()) {
        schedule(tickEvent, curTick() + clock_period);// * process_delay);
    }
    auto tickStop = std::chrono::high_resolution_clock::now();
    simTime = simTime + (tickStop - tickStart);
}


/*********************************************************************************************
- findDynamicDeps(std::list<std::shared_ptr<SALAM::Instructions>, std::shared_ptr<SALAM::Instruction>)
- only parse queue once for each instruction until all dependencies are found
- include self in dependency list
- Register dynamicUser/dynamicDependencies std::deque<std::shared_ptr<SALAM::Instructon> >
*********************************************************************************************/
void // Add third argument, previous BB
LLVMInterface::ActiveFunction::findDynamicDeps(std::shared_ptr<SALAM::Instruction> inst)
{
    // if (DTRACE(Trace)) DPRINTFR(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // DPRINTFR(Runtime, "Linking Dynamic Dependencies [%s]\n", llvm::Instruction::getOpcodeName(inst->getOpode()));
    // The list of UIDs for any dependencies we want to find
    //std::deque<uint64_t> dep_uids = inst->runtimeInitialize();
    std::vector<uint64_t> dep_uids = inst->runtimeInitialize();
    // assert(inst->getDependencyCount() == 0);

    // // An instruction is a runtime dependency for itself since multiple
    // // instances of the same instruction shouldn't execute simultaneously
    // // dep_uids.push_back(inst->getUID());
    if (owner->bin_hierarchy != nullptr) {
        if (inst->is_push_req || inst->is_pop_req)
            handlePushPopDependency(inst);
        if (inst->is_read || inst->is_write)
            handleReadWriteDependency(inst);
    }
    // Find dependencies currently in queues
    if (inst->isLoad()) {
        // std::cerr << "Load: " << inst->getUID() << " " << inst->getIRString() << std::endl;
        auto queue_iter = reservation.rbegin();
        while ((queue_iter != reservation.rend())) {
            auto queued_inst = *queue_iter;
            // Look at each instruction in runtime queue once
            if (inst->isLoad() && queued_inst->isStore()) {
                inst->addRuntimeDependency(queued_inst);
                queued_inst->addRuntimeUser(inst);
            }

            queue_iter++;

        }
        // std::cerr << "Done with load ...\n";
    }

    // Reverse search the reservation queue because we want to link only the last instance of each dep
    auto queue_iter = reservation.rbegin();
    while ((queue_iter != reservation.rend()) && !dep_uids.empty()) {
        auto queued_inst = *queue_iter;
        // Look at each instruction in runtime queue once
        for (auto dep_it = dep_uids.begin(); dep_it != dep_uids.end();) {
            // Check if any of the instruction to be scheduled dependencies match the current instruction from queue
            if (queued_inst->getUID() == *dep_it) {
                // If dependency found, create two way link
                inst->addRuntimeDependency(queued_inst);
                queued_inst->addRuntimeUser(inst);
                dep_it = dep_uids.erase(dep_it);
            } else {
                dep_it++;
            }

        }
        queue_iter++;
    }

    // The other queues do not need to be reverse-searched since only 1 instance of any instruction can exist in them
    // Check the compute queue
    for (auto dep_it = dep_uids.begin(); dep_it != dep_uids.end();) {
        auto queue_iter = computeQueue.find(*dep_it);
        if (queue_iter != computeQueue.end()) {

            auto queued_inst = queue_iter->second;            
            if (inst->isLoad()) {
                // std::cerr << "Queue1: " << queued_inst->getUID() << " " << queued_inst->getIRString() << std::endl;
            }
            inst->addRuntimeDependency(queued_inst);
            queued_inst->addRuntimeUser(inst);
            dep_it = dep_uids.erase(dep_it);
        } else {
            dep_it++;
        }
    }

    // Check the memory read queue
    for (auto dep_it = dep_uids.begin(); dep_it != dep_uids.end();) {
        auto queue_iter = readQueue.find(*dep_it);
        if (queue_iter != readQueue.end()) {
            auto queued_inst = queue_iter->second;
            if (inst->isLoad()) {
                // std::cerr << "Queue2: " << queued_inst->getUID() << " " << queued_inst->getIRString() << std::endl;
            }
            inst->addRuntimeDependency(queued_inst);
            queued_inst->addRuntimeUser(inst);
            dep_it = dep_uids.erase(dep_it);
        } else {
            dep_it++;
        }
    }

    // if (!dep_uids.empty()) {
    //     // Check the memory read queue
    //     // for (auto queued_read : readQueue) {
    //     for (auto rq_it = readQueue.begin(); rq_it != readQueue.end(); rq_it++) {
    //         auto queued_read = *rq_it;
    //         auto queued_inst = queued_read.second;
    //         // Look at each instruction in runtime queue once
    //         for (auto dep_it = dep_uids.begin(); dep_it != dep_uids.end();) {
    //             // Check if any of the instruction to be scheduled dependencies match the current instruction from queue
    //             if (queued_inst->getUID() == *dep_it) {
    //                 // If dependency found, create two way link
    //                 inst->addRuntimeDependency(queued_inst);
    //                 queued_inst->addRuntimeUser(inst);
    //                 dep_it = dep_uids.erase(dep_it);
    //             } else {
    //                 dep_it++;
    //             }
    //         }
    //         if (dep_uids.empty()) break;
    //     }
    // }
    // Check the memory write queue
    for (auto dep_it = dep_uids.begin(); dep_it != dep_uids.end();) {
        auto queue_iter = writeQueue.find(*dep_it);
        if (queue_iter != writeQueue.end()) {
            auto queued_inst = queue_iter->second;
            if (inst->isLoad()) {
                // std::cerr << "Queue3: " << queued_inst->getUID() << " " << queued_inst->getIRString() << std::endl;
            }
            inst->addRuntimeDependency(queued_inst);
            queued_inst->addRuntimeUser(inst);
            dep_it = dep_uids.erase(dep_it);
        } else {
            dep_it++;
        }
    }

    // if (!dep_uids.empty()) {
    //     // Check the memory write queue
    //     // for (auto queued_write : writeQueue) {
    //     for (auto wq_it = writeQueue.begin(); wq_it != writeQueue.end(); wq_it++) {
    //         auto queued_write = *wq_it;
    //         auto queued_inst = queued_write.second;
    //         // Look at each instruction in runtime queue once
    //         for (auto dep_it = dep_uids.begin(); dep_it != dep_uids.end();) {
    //             // Check if any of the instruction to be scheduled dependencies match the current instruction from queue
    //             if (queued_inst->getUID() == *dep_it) {
    //                 // If dependency found, create two way link
    //                 inst->addRuntimeDependency(queued_inst);
    //                 queued_inst->addRuntimeUser(inst);
    //                 dep_it = dep_uids.erase(dep_it);
    //             } else {
    //                 dep_it++;
    //             }
    //         }
    //         if (dep_uids.empty()) break;
    //     }
    // }

    // Fetch values for resolved dependencies, static elements, and immediate values
    if (!dep_uids.empty()) {
        for (auto resolved : dep_uids) {
            // If this dependency exists, then lock value into operand
            inst->setOperandValue(resolved);
        }
    }
}

void
LLVMInterface::dumpModule(llvm::Module *M) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    M->print(llvm::outs(), nullptr);
    for (const llvm::Function &F : *M) {
        for (const llvm::BasicBlock &BB : F) {
            for (const llvm::Instruction &I : BB) {
                I.print(llvm::outs());
            }
        }
    }
}


bool functionNameContains(llvm::Function *func_iter, std::string name) {
    return func_iter->getName().find(name) != std::string::npos;
}

void
LLVMInterface::constructStaticGraph() {
    // readAddressMap();

/*********************************************************************************************
 Constructing the Static CDFG

 Parses LLVM file and creates the CDFG passed to our runtime simulation engine.
*********************************************************************************************/
    auto parseStart = std::chrono::high_resolution_clock::now();
    bool dbg = debug();
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    if (dbg) DPRINTF(LLVMInterface, "Constructing Static Dependency Graph\n");

    llvm::StringRef file = filename;
    std::unique_ptr<llvm::LLVMContext> context(new llvm::LLVMContext());
    std::unique_ptr<llvm::SMDiagnostic> error(new llvm::SMDiagnostic());
    std::unique_ptr<llvm::Module> m;
    std::unique_ptr<llvm::DominatorTree> dt(new llvm::DominatorTree());
    std::unique_ptr<llvm::LoopInfoBase<llvm::BasicBlock, llvm::Loop>> loopInfo(new llvm::LoopInfoBase<llvm::BasicBlock, llvm::Loop>());
    // value_map["%16"] = 20;
    m = llvm::parseIRFile(file, *error, *context);
    if(!m) panic("Error reading Module");

    // Construct the LLVM::Value to SALAM::Value map
    uint64_t valueID = 0;
    SALAM::irvmap vmap;
    // Generate SALAM::Values for llvm::GlobalVariables
    DPRINTF(LLVMInterface, "Instantiate SALAM::GlobalConstants\n");
    for (auto glob_iter = m->global_begin(); glob_iter != m->global_end(); glob_iter++) {
        llvm::GlobalVariable &glb = *glob_iter;
        llvm::errs() << "global var: " << glob_iter->getName() << "\n";

        std::shared_ptr<SALAM::GlobalConstant> sglb = std::make_shared<SALAM::GlobalConstant>(valueID);
        if (!sglb.get()) {
            llvm::errs() << "global var err: " << glob_iter->getName() << "\n";
            continue;
        }
        values.push_back(sglb);
        vmap.insert(SALAM::irvmaptype(&glb, sglb));
        valueID++;
    }

    // Generate SALAM::Functions
    DPRINTF(LLVMInterface, "Instantiate SALAM::Functions\n");
    for (auto func_iter = m->begin(); func_iter != m->end(); func_iter++) {
        llvm::Function &func = *func_iter;
        std::shared_ptr<SALAM::Function> sfunc = std::make_shared<SALAM::Function>(valueID);
        // if the function is malloc, don't skip it since I'm gonna handle it differently
        if ((!sfunc.get() || func_iter->isDeclaration()) && !(functionNameContains(&func, "malloc") || functionNameContains(&func, "realloc"))) {
        // if ((!sfunc.get() || func_iter->isDeclaration())) {
            llvm::errs() << "function err: " << func_iter->getName() << "\n";
            continue;
        }
        values.push_back(sfunc);
        functions.push_back(sfunc);
        vmap.insert(SALAM::irvmaptype(&func, sfunc));
        valueID++;
        // Generate args for SALAM:Functions
        DPRINTF(LLVMInterface, "Instantiate SALAM::Functions::Arguments\n");
        for (auto arg_iter = func.arg_begin(); arg_iter != func.arg_end(); arg_iter++) {
            llvm::Argument &arg = *arg_iter;
            std::shared_ptr<SALAM::Argument> sarg = std::make_shared<SALAM::Argument>(valueID);

            values.push_back(sarg);
            vmap.insert(SALAM::irvmaptype(&arg, sarg));
            valueID++;
        }
        // Generate SALAM::BasicBlocks
        DPRINTF(LLVMInterface, "Instantiate SALAM::Functions::BasicBlocks\n");
        for (auto bb_iter = func.begin(); bb_iter != func.end(); bb_iter++) {
            llvm::BasicBlock &bb = *bb_iter;
            std::shared_ptr<SALAM::BasicBlock> sbb = std::make_shared<SALAM::BasicBlock>(valueID);
            values.push_back(sbb);
            vmap.insert(SALAM::irvmaptype(&bb, sbb));
            valueID++;
            //Generate SALAM::Instructions
            DPRINTF(LLVMInterface, "Instantiate SALAM::Functions::BasicBlocks::Instructions\n");
            for (auto inst_iter = bb.begin(); inst_iter != bb.end(); inst_iter++) {
                llvm::Instruction &inst = *inst_iter;

                std::shared_ptr<SALAM::Instruction> sinst = createInstruction(&inst, valueID);
                sinst->is_reverse = bb_iter->getName().contains("invert");
                sinst->is_unwrap = llvm::isa<llvm::BinaryOperator>(inst_iter) && inst_iter->getNameOrAsOperand().find("_unwrap") != std::string::npos;
                sinst.get()->name_or_as_operand = inst.getNameOrAsOperand();
                if (!sinst.get()) {
                    llvm::errs() << "instruction err: " << inst_iter->getName() << "\n";
                    continue;
                }
                if (inst_iter->hasMetadata("push")) {
                    llvm::errs() << "push inst: " << inst_iter->getName() << "\n";
                    sinst->is_push_req = true;
                    auto *N = inst_iter->getMetadata("size");
                    auto *S = llvm::dyn_cast<llvm::MDString>(N->getOperand(0));
                    sinst->push_pop_count = stoi(S->getString().str());
                }
                if (inst_iter->hasMetadata("pop")) {
                    llvm::errs() << "pop inst: " << inst_iter->getName() << "\n";
                    sinst->is_pop_req = true;
                    auto *N = inst_iter->getMetadata("size");
                    auto *S = llvm::dyn_cast<llvm::MDString>(N->getOperand(0));
                    sinst->push_pop_count = std::stoi(S->getString().str());
                }
                sinst->is_write = inst_iter->hasMetadata("write");
                sinst->is_read = inst_iter->hasMetadata("read");
                values.push_back(sinst);
                vmap.insert(SALAM::irvmaptype(&inst, sinst));

                valueID++;
            }
        }
    }

    // Use value map to initialize SALAM::Values
    DPRINTF(LLVMInterface, "Initialize SALAM::GlobalConstants\n");
    for (auto glob_iter = m->global_begin(); glob_iter != m->global_end(); glob_iter++) {
        llvm::GlobalVariable &glb = *glob_iter;
        llvm::errs() << "global var: " << glob_iter->getName() << "\n";
        std::shared_ptr<SALAM::Value> glbval = vmap.find(&glb)->second;
        assert(glbval);
        std::shared_ptr<SALAM::GlobalConstant> sglb = std::dynamic_pointer_cast<SALAM::GlobalConstant>(glbval);
        if (!sglb.get()) {
            llvm::errs() << "sglobal var err: " << glob_iter->getName() << "\n";
            continue;
        }
        assert(sglb);
        sglb->initialize(&glb, &vmap, &values);

    }
    // Functions will initialize BasicBlocks, which will initialize Instructions
    DPRINTF(LLVMInterface, "Initialize SALAM::Functions\n");
    for (auto func_iter = m->begin(); func_iter != m->end(); func_iter++) {
        llvm::errs() << "function: " << func_iter->getName() << "\n";

        llvm::Function &func = *func_iter;
        if (vmap.find(&func) == vmap.end()) {
            llvm::errs() << "function not found: " << func_iter->getName() << "\n";
            continue;
        }
        std::shared_ptr<SALAM::Value> funcval = vmap.find(&func)->second;

        assert(funcval);
        std::shared_ptr<SALAM::Function> sfunc = std::dynamic_pointer_cast<SALAM::Function>(funcval);
        if (!sfunc.get()) {
            llvm::errs() << "sfunction err: " << func_iter->getName() << "\n";
            continue;
        }
        else {
            llvm::errs() << "sfunction: " << func_iter->getName() << "\n";
        }
        assert(sfunc);
        sfunc->initialize(&func, &vmap, &values, topName);
        // for (auto &bb: func) {
        //     for (auto &inst: bb) {
        //         if (vmap.count(&inst) && value_map.count(inst.getNameOrAsOperand())) {
        //             llvm::errs() << "getting value of " << vmap[&inst].get()->getUID() << " from value map." << "\n";
        //             vmap[&inst].get()->setRegisterValue(value_map[inst.getNameOrAsOperand()]);
        //             vmap[&inst].get()->reading_value_from_map = true;
        //         }
        //     }
        // }
    }
    if (functions.size() == 1) functions.front()->setTop(true);
    // Detect Loop Latches
    for (auto func_iter = m->begin(); func_iter != m->end(); func_iter++) {
        llvm::Function &func = *func_iter;
        if (vmap.find(&func) == vmap.end()) {
            llvm::errs() << "function not found again!: " << func_iter->getName() << "\n";
            continue;
        }
        else {
            llvm::errs() << "function found: " << func_iter->getName() << "\n";
            if (func_iter->isDeclaration()) {
                llvm::errs() << "function is declaration: " << func_iter->getName() << "\n";
                continue;
            }
        }
        dt->recalculate(func);
        loopInfo->releaseMemory();
        loopInfo->analyze(*dt);
        for (auto loop=loopInfo->begin(); loop!=loopInfo->end(); ++loop) {
            if (llvm::BasicBlock *exBB = (*loop)->getExitingBlock()) {
                auto latchingBr = exBB->getTerminator();
                auto mapIt = vmap.find(latchingBr);
                if (mapIt != vmap.end()) {
                    auto salamValue = mapIt->second;
                    if (std::shared_ptr<SALAM::Br> sBr =
                        std::dynamic_pointer_cast<SALAM::Br>(salamValue)) {
                            sBr->setLatching(true);
                        }
                }
            }
        }
    }
    auto parseStop = std::chrono::high_resolution_clock::now();
    llvm::errs() << "Done\n";

    setupTime = parseStop - parseStart;
}

void
LLVMInterface::launchRead(MemoryRequest * memReq) {
    comm->enqueueRead(memReq);
}

void
LLVMInterface::launchRead(MemoryRequest * memReq, ActiveFunction * func) {
    globalReadQueue.insert({memReq, func});
    comm->enqueueRead(memReq);
}

void
LLVMInterface::ActiveFunction::launchRead(std::shared_ptr<SALAM::Instruction> readInst) {
    auto rdInst = std::dynamic_pointer_cast<SALAM::Load>(readInst);
    if (rdInst->isLoadingInternal()) {
        rdInst->loadInternal();
    } else {
        auto memReq = (readInst)->createMemoryRequest();
        auto rd_uid = readInst->getUID();
        readQueue.insert({rd_uid, (readInst)});
        readQueueMap.insert({memReq, rd_uid});
        owner->launchRead(memReq, this);
    }
}

void
LLVMInterface::launchWrite(MemoryRequest * memReq, ActiveFunction * func) {
    globalWriteQueue.insert({memReq, func});
    comm->enqueueWrite(memReq);
}

void
LLVMInterface::ActiveFunction::launchWrite(std::shared_ptr<SALAM::Instruction> writeInst) {
    auto memReq = (writeInst)->createMemoryRequest();
    trackWrite(memReq->getAddress(), writeInst);
    auto wr_uid = writeInst->getUID();
    writeQueue.insert({wr_uid, (writeInst)});
    writeQueueMap.insert({memReq, wr_uid});
    owner->launchWrite(memReq, this);
}

void
LLVMInterface::readCommit(MemoryRequest * req) {
/*********************************************************************************************
 Commit Memory Read Request
*********************************************************************************************/
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    auto queue_iter = globalReadQueue.find(req);
    if (queue_iter != globalReadQueue.end()) {
        queue_iter->second->readCommit(req);
        DPRINTFR(Runtime, "Global Read Commit\n");
        // delete queue_iter->first; // The CommInterface will ultimately delete this memory request
        globalReadQueue.erase(queue_iter);
    } else {
        panic("Could not find memory request in global read queue!");
    }
}

void
LLVMInterface::ActiveFunction::readCommit(MemoryRequest * req) {
/*********************************************************************************************
 Commit Memory Read Request
*********************************************************************************************/
    // if (DTRACE(Trace)) DPRINTFR(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    auto map_iter = readQueueMap.find(req);
    if (map_iter != readQueueMap.end()) {
        auto queue_iter = readQueue.find(map_iter->second);
        if (queue_iter != readQueue.end()) {
            auto load_inst = queue_iter->second;
            uint8_t * readBuff = req->getBuffer();
            // std::cerr << "read buff" << readBuff << " " << *readBuff << "\n";
            load_inst->setRegisterValue(readBuff);
            load_inst->compute();
            DPRINTFR(Runtime, "Local Read Commit\n");
            load_inst->commit();
            readQueue.erase(queue_iter);
            readQueueMap.erase(map_iter);
        } else {
            panic("Could not find memory request in read queue for function %u!", func->getUID());
        }
    } else {
        panic("Could not find memory request in read queue for function %u!", func->getUID());
    }
}

void
LLVMInterface::writeCommit(MemoryRequest * req) {
/*********************************************************************************************
 Commit Memory Write Request
*********************************************************************************************/
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    auto queue_iter = globalWriteQueue.find(req);
    if (queue_iter != globalWriteQueue.end()) {
        queue_iter->second->writeCommit(req);
        // delete queue_iter->first; // The CommInterface will ultimately delete this memory request
        globalWriteQueue.erase(queue_iter);
    } else {
        panic("Could not find memory request in global write queue!");
    }
}

void
LLVMInterface::ActiveFunction::writeCommit(MemoryRequest * req) {
/*********************************************************************************************
 Commit Memory Write Request
*********************************************************************************************/
    // if (DTRACE(Trace)) DPRINTFR(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    auto map_iter = writeQueueMap.find(req);
    if (map_iter != writeQueueMap.end()) {
        auto queue_iter = writeQueue.find(map_iter->second);
        if (queue_iter != writeQueue.end()) {
            queue_iter->second->commit();
            Addr addressWritten = map_iter->first->getAddress();
            untrackWrite(addressWritten);
            writeQueue.erase(queue_iter);
            writeQueueMap.erase(map_iter);
        } else {
            panic("Could not find memory request in write queue for function %u!", func->getUID());
        }
    } else {
        panic("Could not find memory request in write queue for function %u!", func->getUID());
    }
}

void
LLVMInterface::initialize() {
/*********************************************************************************************
 Initialize the Runtime Engine

 Calls function that constructs the basic block list, initializes the reservation table and
 read, write, and compute queues. Set all data collection variables to zero.
*********************************************************************************************/
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    DPRINTF(LLVMInterface, "Initializing LLVM Runtime Engine!\n");
    setupTime = std::chrono::seconds(0);
    simTime = std::chrono::seconds(0);
    schedulingTime = std::chrono::seconds(0);
    queueProcessTime = std::chrono::seconds(0);
    computeTime = std::chrono::seconds(0);
    constructStaticGraph();
    timeStart = std::chrono::high_resolution_clock::now();
    DPRINTF(LLVMInterface, "================================================================\n");
    //debug(1);
    launchTopFunction();

    // HW
    // for(const auto& count : hw->opcodes->usage) {
    //     std::cerr << "OpCode[" << count.first << "] - Usage: " << count.second << "\n";
    // }


    // panic("Kill Simulation");
    //if (debug()) DPRINTF(LLVMInterface, "Initializing Reservation Table!\n");
    //if (debug()) DPRINTF(LLVMInterface, "Initializing readQueue Queue!\n");
    //if (debug()) DPRINTF(LLVMInterface, "Initializing writeQueue Queue!\n");
    //if (debug()) DPRINTF(LLVMInterface, "Initializing computeQueue List!\n");
    if (debug()) DPRINTF(LLVMInterface, "\n%s\n%s\n%s\n",
           "*******************************************************************************",
           "*                 Begin Runtime Simulation Computation Engine                 *",
           "*******************************************************************************");
    running = true;
    cycle = 0;
    stalls = 0;
    tick();

}

void
LLVMInterface::debug(uint64_t flags) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // Dump
    for (auto func_iter = functions.begin(); func_iter != functions.end(); func_iter++) {
        // Function Level
        // (*func_iter)->dump();
        for (auto bb_iter = (*func_iter)->getBBList()->begin(); bb_iter != (*func_iter)->getBBList()->end(); bb_iter++) {
            // Basic Block Level
            (*bb_iter)->dump();
            for (auto inst_iter = (*bb_iter)->Instructions()->begin(); inst_iter != (*bb_iter)->Instructions()->end(); inst_iter++) {
                // Instruction Level
                (*inst_iter)->dump();
            }
        }
    }
    // Dump
    // SALAM::Operand test;
    // test.setOp()
}

void
LLVMInterface::startup() {
/*********************************************************************************************
 Initialize communications between gem5 interface and simulator
*********************************************************************************************/
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    comm->registerCompUnit(this);
}

// LLVMInterface*
// LLVMInterfaceParams::create() {
// /*********************************************************************************************
//  Create new interface between the llvm IR and our simulation engine
// *********************************************************************************************/
//     // if (DTRACE(Trace)) DPRINTFR(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
//     return new LLVMInterface(this);
// }

void
LLVMInterface::finalize() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // Simulation Times
    simStop = std::chrono::high_resolution_clock::now();
    simTotal = simStop - timeStart;
    printPerformanceResults();
    functions.clear();
    values.clear();
    comm->finish();
}

void
LLVMInterface::printPerformanceResults() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    Tick cycle_time = clock_period/1000;

    auto setupMS = std::chrono::duration_cast<std::chrono::milliseconds>(setupTime);
    auto setupHours = std::chrono::duration_cast<std::chrono::hours>(setupMS);
    setupMS -= std::chrono::duration_cast<std::chrono::seconds>(setupHours);
    auto setupMins = std::chrono::duration_cast<std::chrono::minutes>(setupMS);
    setupMS -= std::chrono::duration_cast<std::chrono::seconds>(setupMins);
    auto setupSecs = std::chrono::duration_cast<std::chrono::seconds>(setupMS);
    setupMS -= std::chrono::duration_cast<std::chrono::seconds>(setupSecs);

    auto totalMS = std::chrono::duration_cast<std::chrono::milliseconds>(simTotal);
    auto totalHours = std::chrono::duration_cast<std::chrono::hours>(totalMS);
    totalMS -= std::chrono::duration_cast<std::chrono::seconds>(totalHours);
    auto totalMins = std::chrono::duration_cast<std::chrono::minutes>(totalMS);
    totalMS -= std::chrono::duration_cast<std::chrono::seconds>(totalMins);
    auto totalSecs = std::chrono::duration_cast<std::chrono::seconds>(totalMS);
    totalMS -= std::chrono::duration_cast<std::chrono::seconds>(totalSecs);
    
    auto simMS = std::chrono::duration_cast<std::chrono::milliseconds>(simTime);
    auto simHours = std::chrono::duration_cast<std::chrono::hours>(simMS);
    simMS -= std::chrono::duration_cast<std::chrono::seconds>(simHours);
    auto simMins = std::chrono::duration_cast<std::chrono::minutes>(simMS);
    simMS -= std::chrono::duration_cast<std::chrono::seconds>(simMins);
    auto simSecs = std::chrono::duration_cast<std::chrono::seconds>(simMS);
    simMS -= std::chrono::duration_cast<std::chrono::seconds>(simSecs);

    auto queueMS = std::chrono::duration_cast<std::chrono::milliseconds>(queueProcessTime);
    auto queueHours = std::chrono::duration_cast<std::chrono::hours>(queueMS);
    queueMS -= std::chrono::duration_cast<std::chrono::seconds>(queueHours);
    auto queueMins = std::chrono::duration_cast<std::chrono::minutes>(queueMS);
    queueMS -= std::chrono::duration_cast<std::chrono::seconds>(queueMins);
    auto queueSecs = std::chrono::duration_cast<std::chrono::seconds>(queueMS);
    queueMS -= std::chrono::duration_cast<std::chrono::seconds>(queueSecs);

    auto schedMS = std::chrono::duration_cast<std::chrono::milliseconds>(schedulingTime);
    auto schedHours = std::chrono::duration_cast<std::chrono::hours>(schedMS);
    schedMS -= std::chrono::duration_cast<std::chrono::seconds>(schedHours);
    auto schedMins = std::chrono::duration_cast<std::chrono::minutes>(schedMS);
    schedMS -= std::chrono::duration_cast<std::chrono::seconds>(schedMins);
    auto schedSecs = std::chrono::duration_cast<std::chrono::seconds>(schedMS);
    schedMS -= std::chrono::duration_cast<std::chrono::seconds>(schedSecs);

    auto computeMS = std::chrono::duration_cast<std::chrono::milliseconds>(computeTime);
    auto computeHours = std::chrono::duration_cast<std::chrono::hours>(computeMS);
    computeMS -= std::chrono::duration_cast<std::chrono::seconds>(computeHours);
    auto computeMins = std::chrono::duration_cast<std::chrono::minutes>(computeMS);
    computeMS -= std::chrono::duration_cast<std::chrono::seconds>(computeMins);
    auto computeSecs = std::chrono::duration_cast<std::chrono::seconds>(computeMS);
    computeMS -= std::chrono::duration_cast<std::chrono::seconds>(computeSecs);

/*********************************************************************************************
 Prints usage statistics of how many times each instruction was accessed during runtime
*********************************************************************************************/
    std::cout << "********************************************************************************" << std::endl;
    std::cout << name() << std::endl;
    std::cout << "   ========= Performance Analysis =============" << std::endl;
    std::cout << "   Setup Time:                      " << setupHours.count() << "h " << setupMins.count() << "m " << setupSecs.count() << "s " << setupMS.count() << "ms" << std::endl;
    std::cout << "   Simulation Time (Total):         " << totalHours.count() << "h " << totalMins.count() << "m " << totalSecs.count() << "s " << totalMS.count() << "ms" << std::endl;
    std::cout << "   Simulation Time (Active):        " << simHours.count() << "h " << simMins.count() << "m " << simSecs.count() << "s " << simMS.count() << "ms" << std::endl;
    std::cout << "        Queue Processing Time:      " << queueHours.count() << "h " << queueMins.count() << "m " << queueSecs.count() << "s " << queueMS.count() << "ms" << std::endl;
    std::cout << "             Scheduling Time:       " << schedHours.count() << "h " << schedMins.count() << "m " << schedSecs.count() << "s " << schedMS.count() << "ms" << std::endl;
    std::cout << "             Computation Time:      " << computeHours.count() << "h " << computeMins.count() << "m " << computeSecs.count() << "s " << computeMS.count() << "ms" << std::endl;
    std::cout << "   System Clock:                    " << 1.0/(cycle_time) << "GHz" << std::endl;
    std::cout << "   Runtime:                         " << cycle << " cycles" << std::endl;
    std::cout << "   Runtime:                         " << (cycle*cycle_time*(1e-3)) << " us" << std::endl;
    std::cout << "   Stalls:                          " << stalls << " cycles" << std::endl;
    std::cout << "   Executed Nodes:                  " << (cycle-stalls-1) << " cycles" << std::endl;
    if (bin_hierarchy) {
        std::cout << "   Bins Total DRAM Writes:                  " << bin_hierarchy->spilled_count << std::endl;
        std::cout << "   Bins Total DRAM Reads:                  " << bin_hierarchy->fetched_count << std::endl;
    }
    std::cout << std::endl;
}

void
LLVMInterface::dumpQueues() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // std::cout << "*********************************************************\n"
    //           << "Compute Queue\n"
    //           << "*********************************************************\n";
    // for (auto compute : computeQueue) {
    //     std::cout << compute->_LLVMLine << std::endl;
    // }
    // std::cout << "*********************************************************\n"
    //           << "Read Queue\n"
    //           << "*********************************************************\n";
    // for (auto read : readQueue) {
    //     std::cout << read->_LLVMLine << std::endl;
    // }
    // std::cout << "*********************************************************\n"
    //           << "Write Queue\n"
    //           << "*********************************************************\n";
    // for (auto write : writeQueue) {
    //     std::cout << write->_LLVMLine << std::endl;
    // }
    // std::cout << "*********************************************************\n"
    //           << "Reservation Queue\n"
    //           << "*********************************************************\n";
    // for (auto reserved : reservation) {
    //     std::cout << reserved->_LLVMLine << std::endl;
    // }
    // std::cout << "*********************************************************\n"
    //           << "End of queue dump\n"
    //           << "*********************************************************\n";
}

void
LLVMInterface::launchFunction(std::shared_ptr<SALAM::Function> callee,
                              std::shared_ptr<SALAM::Instruction> caller) {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    // Add the callee to our list of active functions
    activeFunctions.push_back(ActiveFunction(this, callee, caller));
    activeFunctions.back().launch();
}

void
LLVMInterface::launchTopFunction() {
    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    for (auto it = functions.begin(); it != functions.end(); it++) {
        cprintf("Function: %s\n", *it);
        if ((*it)->isTop()) {
            // Launch the top level function
            launchFunction((*it), nullptr);
            return;
        }
    }
    // Fallback if no function was marked as the top-level
    panic("No function marked as top-level. Set the top_name parameter for your LLVMInterface to the name of the top-level function\n");
}


void LLVMInterface::ActiveFunction::launch() {
    // if (DTRACE(Trace)) DPRINTFR(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    DPRINTFR(LLVMInterface, "Launching Function: %s\n", func->getIRStub());
    // func->value_dump();
    // Fetch the arguments
    std::vector<std::shared_ptr<SALAM::Value>> funcArgs = *(func->getArguments());
    if (func->isTop()) {
        // We need to fetch argument values from the memory mapped registers
        DPRINTFR(LLVMInterface, "Connecting CommInterface\n");
        CommInterface * comm = owner->getCommInterface();
        DPRINTFR(LLVMInterface, "Connecting HWInterface\n");
        hw = owner->getHWInterface();

        unsigned argOffset = 0;
        for (auto arg : funcArgs) {
            uint64_t argSizeInBytes = arg->getSizeInBytes();
            uint64_t regValue = comm->getGlobalVar(argOffset, argSizeInBytes);
            arg->setRegisterValue(regValue);
            argOffset += argSizeInBytes;
        }
    } else {
        llvm::errs() << "Launching Function: " << func->getIRStub()  << " Caller : " << caller->getIRStub() << "\n";

        // We need to fetch argument values from the calling function
        //std::deque<SALAM::Operand> callerArgs = *caller->getOperands();
        std::vector<SALAM::Operand> callerArgs = *caller->getOperands();
        // If it's malloc, handle it differently

        if (funcArgs.size() != callerArgs.size())
            panic("Function expects %d args. Got %d args.", funcArgs.size(), callerArgs.size());
        for (auto i = 0; i < callerArgs.size(); i++) {
            funcArgs.at(i)->setRegisterValue(callerArgs.at(i).getOpRegister());
        }
    }

    func->addInstance();
    // Schedule the first BB
    if (!isMalloc(func.get()) && !isRealloc(func.get()))
        scheduleBB(func->entry());

}

std::shared_ptr<SALAM::Instruction>
LLVMInterface::createInstruction(llvm::Instruction * inst, uint64_t id) {
    last_id = id;
    int cost = 0;
    if (cost_map.count(inst->getNameOrAsOperand())) {
        cost = cost_map.at(inst->getNameOrAsOperand());
    }

    // if (DTRACE(Trace)) DPRINTF(Runtime, "Trace: %s \n", __PRETTY_FUNCTION__);
    uint64_t OpCode = inst->Instruction::getOpcode();
    // if (DTRACE(Trace)) DPRINTF(LLVMInterface, "Switch OpCode [%d]\n", OpCode);
    // HW
    hw->opcodes->update_usage(OpCode);
    switch(OpCode) {
        case llvm::Instruction::Ret : return SALAM::createRetInst(id, OpCode, cost + hw->cycle_counts->ret_inst); break;
        case llvm::Instruction::Br: return SALAM::createBrInst(id, OpCode, cost + hw->cycle_counts->br_inst); break;
        case llvm::Instruction::Switch: return SALAM::createSwitchInst(id, OpCode, cost + hw->cycle_counts->switch_inst); break;
        case llvm::Instruction::Add: return SALAM::createAddInst(id, OpCode, cost + hw->cycle_counts->add_inst); break;
        case llvm::Instruction::FAdd: return SALAM::createFAddInst(id, OpCode, cost + hw->cycle_counts->fadd_inst); break;
        case llvm::Instruction::Sub: return SALAM::createSubInst(id, OpCode, cost + hw->cycle_counts->sub_inst); break;
        case llvm::Instruction::FSub: return SALAM::createFSubInst(id, OpCode, cost + hw->cycle_counts->fsub_inst); break;
        case llvm::Instruction::Mul: return SALAM::createMulInst(id, OpCode, cost + hw->cycle_counts->mul_inst); break;
        case llvm::Instruction::FMul: return SALAM::createFMulInst(id, OpCode, cost + hw->cycle_counts->fmul_inst); break;
        case llvm::Instruction::UDiv: return SALAM::createUDivInst(id, OpCode, cost + hw->cycle_counts->udiv_inst); break;
        case llvm::Instruction::SDiv: return SALAM::createSDivInst(id, OpCode, cost + hw->cycle_counts->sdiv_inst); break;
        case llvm::Instruction::FDiv: return SALAM::createFDivInst(id, OpCode, cost + hw->cycle_counts->fdiv_inst); break;
        case llvm::Instruction::URem: return SALAM::createURemInst(id, OpCode, cost + hw->cycle_counts->urem_inst); break;
        case llvm::Instruction::SRem: return SALAM::createSRemInst(id, OpCode, cost + hw->cycle_counts->srem_inst); break;
        case llvm::Instruction::FRem: return SALAM::createFRemInst(id, OpCode, cost + hw->cycle_counts->frem_inst); break;
        case llvm::Instruction::Shl: return SALAM::createShlInst(id, OpCode, cost + hw->cycle_counts->shl_inst); break;
        case llvm::Instruction::LShr: return SALAM::createLShrInst(id, OpCode, cost + hw->cycle_counts->lshr_inst); break;
        case llvm::Instruction::AShr: return SALAM::createAShrInst(id, OpCode, cost + hw->cycle_counts->ashr_inst); break;
        case llvm::Instruction::And: return SALAM::createAndInst(id, OpCode, cost + hw->cycle_counts->and_inst); break;
        case llvm::Instruction::Or: return SALAM::createOrInst(id, OpCode, cost + hw->cycle_counts->or_inst); break;
        case llvm::Instruction::Xor: return SALAM::createXorInst(id, OpCode, cost + hw->cycle_counts->xor_inst); break;
        case llvm::Instruction::Load: return SALAM::createLoadInst(id, OpCode, 0); break;
        case llvm::Instruction::Store: return SALAM::createStoreInst(id, OpCode, cost + hw->cycle_counts->store_inst); break;
        case llvm::Instruction::GetElementPtr : return SALAM::createGetElementPtrInst(id, OpCode, cost + hw->cycle_counts->gep_inst); break;
        case llvm::Instruction::Trunc: return SALAM::createTruncInst(id, OpCode, cost + hw->cycle_counts->trunc_inst); break;
        case llvm::Instruction::ZExt: return SALAM::createZExtInst(id, OpCode, cost + hw->cycle_counts->zext_inst); break;
        case llvm::Instruction::SExt: return SALAM::createSExtInst(id, OpCode, cost + hw->cycle_counts->sext_inst); break;
        case llvm::Instruction::FPToUI: return SALAM::createFPToUIInst(id, OpCode, cost + hw->cycle_counts->fptoui_inst); break;
        case llvm::Instruction::FPToSI: return SALAM::createFPToSIInst(id, OpCode, cost + hw->cycle_counts->fptosi_inst); break;
        case llvm::Instruction::UIToFP: return SALAM::createUIToFPInst(id, OpCode, cost + hw->cycle_counts->uitofp_inst); break;
        case llvm::Instruction::SIToFP: return SALAM::createSIToFPInst(id, OpCode, cost + hw->cycle_counts->sitofp_inst); break;
        case llvm::Instruction::FPTrunc: return SALAM::createFPTruncInst(id, OpCode, cost + hw->cycle_counts->fptrunc_inst); break;
        case llvm::Instruction::FPExt: return SALAM::createFPExtInst(id, OpCode, cost + hw->cycle_counts->fpext_inst); break;
        case llvm::Instruction::PtrToInt: return SALAM::createPtrToIntInst(id, OpCode, cost + hw->cycle_counts->ptrtoint_inst); break;
        case llvm::Instruction::IntToPtr: return SALAM::createIntToPtrInst(id, OpCode, cost + hw->cycle_counts->inttoptr_inst); break;
        case llvm::Instruction::BitCast: return SALAM::createBitCastInst(id, OpCode, cost + hw->cycle_counts->bitcast_inst); break;
        case llvm::Instruction::ICmp: return SALAM::createICmpInst(id, OpCode, cost + hw->cycle_counts->icmp_inst); break;
        case llvm::Instruction::FCmp: return SALAM::createFCmpInst(id, OpCode, cost + hw->cycle_counts->fcmp_inst); break;
        case llvm::Instruction::PHI: return SALAM::createPHIInst(id, OpCode, cost + hw->cycle_counts->phi_inst); break;
        case llvm::Instruction::Call: return SALAM::createCallInst(id, OpCode, cost + hw->cycle_counts->call_inst); break;
        case llvm::Instruction::Select: return SALAM::createSelectInst(id, OpCode, cost + hw->cycle_counts->select_inst); break;
        default: {
            warn("Tried to create instance of undefined instruction type!"); 
            llvm::errs() <<  *inst << " is not a valid instruction type\n";
            return SALAM::createBadInst(id, OpCode, 1); break;
        }
    }
}
